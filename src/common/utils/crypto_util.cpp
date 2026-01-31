#include "../../../include/common/crypto_util.h"
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <sstream>
#include <iomanip>
#include <random>
#include <cstring>
#include <uuid/uuid.h>

namespace ourchat {

std::string CryptoUtil::MD5Hash(const std::string& data) {
    unsigned char hash[MD5_DIGEST_LENGTH];
    ::MD5(reinterpret_cast<const unsigned char*>(data.c_str()), data.length(), hash);
    
    std::stringstream ss;
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}

std::string CryptoUtil::SHA256Hash(const std::string& data) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    ::SHA256(reinterpret_cast<const unsigned char*>(data.c_str()), data.length(), hash);
    
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}

std::string CryptoUtil::HMAC_SHA256(const std::string& data, const std::string& key) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;
    
    HMAC(EVP_sha256(), 
         key.c_str(), key.length(),
         reinterpret_cast<const unsigned char*>(data.c_str()), data.length(),
         hash, &hash_len);
    
    std::stringstream ss;
    for (unsigned int i = 0; i < hash_len; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}

std::string CryptoUtil::EncryptAES(const std::string& data, const std::string& key) {
    if (key.length() != 32) {
        return "";
    }
    
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return "";
    
    unsigned char iv[AES_BLOCK_SIZE];
    RAND_bytes(iv, AES_BLOCK_SIZE);
    
    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, 
                          reinterpret_cast<const unsigned char*>(key.c_str()), iv) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }
    
    int out_len1 = data.length() + AES_BLOCK_SIZE;
    std::string out(out_len1, '\0');
    unsigned char* out_data = reinterpret_cast<unsigned char*>(&out[0]);
    
    if (EVP_EncryptUpdate(ctx, out_data, &out_len1, 
                         reinterpret_cast<const unsigned char*>(data.c_str()), data.length()) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }
    
    int out_len2 = 0;
    if (EVP_EncryptFinal_ex(ctx, out_data + out_len1, &out_len2) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }
    
    out.resize(out_len1 + out_len2);
    EVP_CIPHER_CTX_free(ctx);
    
    return std::string(reinterpret_cast<char*>(iv), AES_BLOCK_SIZE) + out;
}

std::string CryptoUtil::DecryptAES(const std::string& data, const std::string& key) {
    if (key.length() != 32 || data.length() < AES_BLOCK_SIZE) {
        return "";
    }
    
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return "";
    
    unsigned char iv[AES_BLOCK_SIZE];
    std::memcpy(iv, data.c_str(), AES_BLOCK_SIZE);
    
    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, 
                          reinterpret_cast<const unsigned char*>(key.c_str()), iv) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }
    
    int out_len1 = data.length() - AES_BLOCK_SIZE;
    std::string out(out_len1, '\0');
    unsigned char* out_data = reinterpret_cast<unsigned char*>(&out[0]);
    
    if (EVP_DecryptUpdate(ctx, out_data, &out_len1, 
                         reinterpret_cast<const unsigned char*>(data.c_str()) + AES_BLOCK_SIZE, 
                         data.length() - AES_BLOCK_SIZE) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }
    
    int out_len2 = 0;
    if (EVP_DecryptFinal_ex(ctx, out_data + out_len1, &out_len2) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }
    
    out.resize(out_len1 + out_len2);
    EVP_CIPHER_CTX_free(ctx);
    
    return out;
}

std::string CryptoUtil::GenerateToken(int length) {
    static const char* chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, strlen(chars) - 1);
    
    std::string token;
    token.reserve(length);
    for (int i = 0; i < length; i++) {
        token += chars[dis(gen)];
    }
    return token;
}

std::string CryptoUtil::GenerateSessionId() {
    return GenerateToken(32);
}

bool CryptoUtil::ValidatePassword(const std::string& password, const std::string& hash) {
    return HashPassword(password) == hash;
}

std::string CryptoUtil::HashPassword(const std::string& password) {
    return SHA256Hash(password);
}

std::string CryptoUtil::GenerateUUID() {
    uuid_t uuid;
    uuid_generate(uuid);
    
    char uuid_str[37];
    uuid_unparse(uuid, uuid_str);
    
    return std::string(uuid_str);
}

} // namespace ourchat
