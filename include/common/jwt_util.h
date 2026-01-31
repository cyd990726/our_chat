#ifndef OURCHAT_JWT_UTIL_H
#define OURCHAT_JWT_UTIL_H

#include <string>
#include <chrono>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <iomanip>
#include <sstream>

namespace ourchat {

class JWTUtil {
public:
    static std::string GenerateToken(int64_t user_id, const std::string& secret, int expires_in_seconds = 3600) {
        auto now = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
        
        std::string header = Base64UrlEncode("{\"alg\":\"HS256\",\"typ\":\"JWT\"}");
        std::string payload = Base64UrlEncode("{\"user_id\":" + std::to_string(user_id) + 
                                              ",\"iat\":" + std::to_string(now) + 
                                              ",\"exp\":" + std::to_string(now + expires_in_seconds) + "}");
        
        std::string signature = Sign(header + "." + payload, secret);
        
        return header + "." + payload + "." + signature;
    }
    
    static bool ValidateToken(const std::string& token, int64_t& user_id, const std::string& secret) {
        auto parts = Split(token, '.');
        if (parts.size() != 3) return false;
        
        std::string expected_signature = Sign(parts[0] + "." + parts[1], secret);
        if (expected_signature != parts[2]) return false;
        
        std::string payload = Base64UrlDecode(parts[1]);
        auto pos = payload.find("\"user_id\":");
        if (pos == std::string::npos) return false;
        
        pos += 10;
        size_t end = payload.find_first_of(",}", pos);
        try {
            user_id = std::stoll(payload.substr(pos, end - pos));
        } catch (...) {
            return false;
        }
        
        auto exp_pos = payload.find("\"exp\":");
        if (exp_pos != std::string::npos) {
            exp_pos += 5;
            size_t exp_end = payload.find_first_of(",}", exp_pos);
            int64_t exp = std::stoll(payload.substr(exp_pos, exp_end - exp_pos));
            auto now = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
            if (now > exp) return false;
        }
        
        return true;
    }
    
private:
    static std::string Sign(const std::string& data, const std::string& key) {
        unsigned char hash[EVP_MAX_MD_SIZE];
        unsigned int hash_len;
        
        HMAC(EVP_sha256(), key.data(), key.length(), 
             reinterpret_cast<const unsigned char*>(data.data()), data.length(),
             hash, &hash_len);
        
        return Base64UrlEncode(std::string(reinterpret_cast<char*>(hash), hash_len));
    }
    
    static std::string Base64UrlEncode(const std::string& input) {
        BIO* b64 = BIO_new(BIO_f_base64());
        BIO* bio = BIO_new(BIO_s_mem());
        bio = BIO_push(b64, bio);
        
        BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
        BIO_write(bio, input.data(), input.length());
        BIO_flush(bio);
        
        BUF_MEM* buffer_ptr;
        BIO_get_mem_ptr(bio, &buffer_ptr);
        
        std::string result(buffer_ptr->data, buffer_ptr->length);
        BIO_free_all(bio);
        
        for (char& c : result) {
            if (c == '+') c = '-';
            else if (c == '/') c = '_';
            else if (c == '=') c = '\0';
        }
        
        return result;
    }
    
    static std::string Base64UrlDecode(const std::string& input) {
        std::string temp = input;
        for (char& c : temp) {
            if (c == '-') c = '+';
            else if (c == '_') c = '/';
        }
        
        while (temp.length() % 4) temp += '=';
        
        BIO* b64 = BIO_new(BIO_f_base64());
        BIO* bio = BIO_new_mem_buf(temp.data(), temp.length());
        bio = BIO_push(b64, bio);
        
        BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
        
        char buffer[4096];
        int len = BIO_read(bio, buffer, sizeof(buffer));
        
        BIO_free_all(bio);
        
        return std::string(buffer, len);
    }
    
    static std::vector<std::string> Split(const std::string& s, char delim) {
        std::vector<std::string> parts;
        std::stringstream ss(s);
        std::string part;
        while (std::getline(ss, part, delim)) {
            parts.push_back(part);
        }
        return parts;
    }
};

}

#endif // OURCHAT_JWT_UTIL_H
