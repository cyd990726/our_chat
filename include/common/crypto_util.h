#ifndef OURCHAT_CRYPTO_UTIL_H
#define OURCHAT_CRYPTO_UTIL_H

#include <string>

namespace ourchat {

class CryptoUtil {
public:
    static std::string MD5Hash(const std::string& data);
    static std::string SHA256Hash(const std::string& data);
    static std::string HMAC_SHA256(const std::string& data, const std::string& key);
    
    static std::string EncryptAES(const std::string& data, const std::string& key);
    static std::string DecryptAES(const std::string& data, const std::string& key);
    
    static std::string GenerateToken(int length = 32);
    static std::string GenerateSessionId();
    
    static bool ValidatePassword(const std::string& password, const std::string& hash);
    static std::string HashPassword(const std::string& password);
    
    static std::string GenerateUUID();
};

} // namespace ourchat

#endif // OURCHAT_CRYPTO_UTIL_H
