#include "../../../include/common/string_util.h"
#include <random>
#include <stdexcept>

namespace ourchat {

std::string StringUtil::Trim(const std::string& str) {
    if (str.empty()) return str;
    
    size_t start = 0;
    while (start < str.length() && std::isspace(str[start])) start++;
    
    size_t end = str.length();
    while (end > start && std::isspace(str[end - 1])) end--;
    
    return str.substr(start, end - start);
}

std::string StringUtil::TrimLeft(const std::string& str) {
    if (str.empty()) return str;
    
    size_t start = 0;
    while (start < str.length() && std::isspace(str[start])) start++;
    
    return str.substr(start);
}

std::string StringUtil::TrimRight(const std::string& str) {
    if (str.empty()) return str;
    
    size_t end = str.length();
    while (end > 0 && std::isspace(str[end - 1])) end--;
    
    return str.substr(0, end);
}

std::string StringUtil::ToLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), 
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

std::string StringUtil::ToUpper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return result;
}

std::vector<std::string> StringUtil::Split(const std::string& str, char delimiter) {
    std::vector<std::string> parts;
    std::stringstream ss(str);
    std::string part;
    
    while (std::getline(ss, part, delimiter)) {
        parts.push_back(part);
    }
    
    return parts;
}

std::vector<std::string> StringUtil::Split(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> parts;
    size_t start = 0;
    size_t pos = 0;
    
    while ((pos = str.find(delimiter, start)) != std::string::npos) {
        parts.push_back(str.substr(start, pos - start));
        start = pos + delimiter.length();
    }
    
    parts.push_back(str.substr(start));
    return parts;
}

std::string StringUtil::Join(const std::vector<std::string>& parts, const std::string& separator) {
    std::string result;
    for (size_t i = 0; i < parts.size(); i++) {
        result += parts[i];
        if (i < parts.size() - 1) {
            result += separator;
        }
    }
    return result;
}

bool StringUtil::StartsWith(const std::string& str, const std::string& prefix) {
    if (str.length() < prefix.length()) return false;
    return str.compare(0, prefix.length(), prefix) == 0;
}

bool StringUtil::EndsWith(const std::string& str, const std::string& suffix) {
    if (str.length() < suffix.length()) return false;
    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}

bool StringUtil::Contains(const std::string& str, const std::string& substr) {
    return str.find(substr) != std::string::npos;
}

std::string StringUtil::Replace(const std::string& str, const std::string& old_str, const std::string& new_str) {
    if (old_str.empty()) return str;
    
    std::string result = str;
    size_t pos = 0;
    while ((pos = result.find(old_str, pos)) != std::string::npos) {
        result.replace(pos, old_str.length(), new_str);
        pos += new_str.length();
    }
    return result;
}

std::string StringUtil::Repeat(const std::string& str, int count) {
    if (count <= 0) return "";
    return std::string(count, ' ').replace(0, str.length(), str);
}

bool StringUtil::IsEmpty(const std::string& str) {
    return str.empty();
}

std::string StringUtil::RandomString(int length) {
    static const std::string chars = 
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, chars.length() - 1);
    
    std::string result;
    result.reserve(length);
    for (int i = 0; i < length; i++) {
        result += chars[dis(gen)];
    }
    return result;
}

std::string StringUtil::ToString(int value) {
    return std::to_string(value);
}

std::string StringUtil::ToString(int64_t value) {
    return std::to_string(value);
}

std::string StringUtil::ToString(double value) {
    return std::to_string(value);
}

int StringUtil::ToInt(const std::string& str) {
    return std::stoi(str);
}

int64_t StringUtil::ToInt64(const std::string& str) {
    return std::stoll(str);
}

double StringUtil::ToDouble(const std::string& str) {
    return std::stod(str);
}

std::string StringUtil::EncodeBase64(const std::string& data) {
    static const char* base64_chars = 
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    
    std::string result;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    
    for (char c : data) {
        char_array_3[i++] = static_cast<unsigned char>(c);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            
            for (i = 0; i < 4; i++) {
                result += base64_chars[char_array_4[i]];
            }
            i = 0;
        }
    }
    
    if (i) {
        for (j = i; j < 3; j++) {
            char_array_3[j] = '\0';
        }
        
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;
        
        for (j = 0; j < i + 1; j++) {
            result += base64_chars[char_array_4[j]];
        }
        
        while ((i++ < 3)) {
            result += '=';
        }
    }
    
    return result;
}

std::string StringUtil::DecodeBase64(const std::string& data) {
    static int tab[] = {
        62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1,
        -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
        10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
        -1, -1, -1, -1, -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36,
        37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
    };
    
    std::string result;
    int in_len = data.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    
    while (in_len-- && (data[in_] != '=')) {
        if (tab[data[in_]] < 0) {
            in_++;
            continue;
        }
        
        char_array_4[i++] = static_cast<unsigned char>(tab[data[in_]]);
        in_++;
        
        if (i == 4) {
            for (i = 0; i < 4; i++) {
                char_array_4[i] = static_cast<unsigned char>(tab[char_array_4[i]]);
            }
            
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
            
            for (i = 0; i < 3; i++) {
                result += char_array_3[i];
            }
            i = 0;
        }
    }
    
    if (i) {
        for (j = 0; j < i; j++) {
            char_array_4[j] = static_cast<unsigned char>(tab[char_array_4[j]]);
        }
        
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        
        for (j = 0; j < i - 1; j++) {
            result += char_array_3[j];
        }
    }
    
    return result;
}

std::string StringUtil::EscapeHtml(const std::string& str) {
    std::string result;
    for (char c : str) {
        switch (c) {
            case '<': result += "&lt;"; break;
            case '>': result += "&gt;"; break;
            case '&': result += "&amp;"; break;
            case '"': result += "&quot;"; break;
            case '\'': result += "&#39;"; break;
            default: result += c; break;
        }
    }
    return result;
}

std::string StringUtil::UnescapeHtml(const std::string& str) {
    std::string result = str;
    size_t pos = 0;
    
    auto replace_all = [&](const std::string& from, const std::string& to) {
        while ((pos = result.find(from, pos)) != std::string::npos) {
            result.replace(pos, from.length(), to);
            pos += to.length();
        }
    };
    
    replace_all("&lt;", "<");
    replace_all("&gt;", ">");
    replace_all("&amp;", "&");
    replace_all("&quot;", "\"");
    replace_all("&#39;", "'");
    
    return result;
}

} // namespace ourchat
