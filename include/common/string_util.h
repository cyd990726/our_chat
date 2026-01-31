#ifndef OURCHAT_STRING_UTIL_H
#define OURCHAT_STRING_UTIL_H

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace ourchat {

class StringUtil {
public:
    static std::string Trim(const std::string& str);
    static std::string TrimLeft(const std::string& str);
    static std::string TrimRight(const std::string& str);
    
    static std::string ToLower(const std::string& str);
    static std::string ToUpper(const std::string& str);
    
    static std::vector<std::string> Split(const std::string& str, char delimiter);
    static std::vector<std::string> Split(const std::string& str, const std::string& delimiter);
    
    static std::string Join(const std::vector<std::string>& parts, const std::string& separator);
    
    static bool StartsWith(const std::string& str, const std::string& prefix);
    static bool EndsWith(const std::string& str, const std::string& suffix);
    
    static bool Contains(const std::string& str, const std::string& substr);
    
    static std::string Replace(const std::string& str, const std::string& old_str, const std::string& new_str);
    
    static std::string Repeat(const std::string& str, int count);
    
    static bool IsEmpty(const std::string& str);
    
    static std::string RandomString(int length);
    
    static std::string ToString(int value);
    static std::string ToString(int64_t value);
    static std::string ToString(double value);
    
    static int ToInt(const std::string& str);
    static int64_t ToInt64(const std::string& str);
    static double ToDouble(const std::string& str);
    
    static std::string EncodeBase64(const std::string& data);
    static std::string DecodeBase64(const std::string& data);
    
    static std::string EscapeHtml(const std::string& str);
    static std::string UnescapeHtml(const std::string& str);
};

} // namespace ourchat

#endif // OURCHAT_STRING_UTIL_H
