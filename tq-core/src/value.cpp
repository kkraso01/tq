#include "tq/value.hpp"
#include <sstream>
#include <iomanip>
#include <cctype>

namespace tq {

// Constructors
Value::Value() : type_(Type::Null), data_(std::monostate{}) {}

Value::Value(bool b) : type_(Type::Boolean), data_(b) {}

Value::Value(double d) : type_(Type::Number), data_(d) {}

Value::Value(int i) : type_(Type::Number), data_(static_cast<double>(i)) {}

Value::Value(const std::string& s) : type_(Type::String), data_(s) {}

Value::Value(std::string&& s) : type_(Type::String), data_(std::move(s)) {}

Value::Value(const char* s) : type_(Type::String), data_(std::string(s)) {}

Value::Value(std::vector<Value>&& arr) : type_(Type::Array), data_(std::move(arr)) {}

Value::Value(const std::vector<Value>& arr) : type_(Type::Array), data_(arr) {}

Value::Value(std::map<std::string, Value>&& obj) : type_(Type::Object), data_(std::move(obj)) {}

Value::Value(const std::map<std::string, Value>& obj) : type_(Type::Object), data_(obj) {}

// Rule of 5
Value::Value(const Value& other) : type_(other.type_), data_(other.data_) {}

Value::Value(Value&& other) noexcept : type_(other.type_), data_(std::move(other.data_)) {
    other.type_ = Type::Null;
    other.data_ = std::monostate{};
}

Value& Value::operator=(const Value& other) {
    if (this != &other) {
        type_ = other.type_;
        data_ = other.data_;
    }
    return *this;
}

Value& Value::operator=(Value&& other) noexcept {
    if (this != &other) {
        type_ = other.type_;
        data_ = std::move(other.data_);
        other.type_ = Type::Null;
        other.data_ = std::monostate{};
    }
    return *this;
}

Value::~Value() = default;

// Getters
bool Value::as_boolean() const {
    if (type_ != Type::Boolean) {
        throw std::runtime_error("Value is not a boolean");
    }
    return std::get<bool>(data_);
}

double Value::as_number() const {
    if (type_ != Type::Number) {
        throw std::runtime_error("Value is not a number");
    }
    return std::get<double>(data_);
}

const std::string& Value::as_string() const {
    if (type_ != Type::String) {
        throw std::runtime_error("Value is not a string");
    }
    return std::get<std::string>(data_);
}

const std::vector<Value>& Value::as_array() const {
    if (type_ != Type::Array) {
        throw std::runtime_error("Value is not an array");
    }
    return std::get<std::vector<Value>>(data_);
}

std::vector<Value>& Value::as_array() {
    if (type_ != Type::Array) {
        throw std::runtime_error("Value is not an array");
    }
    return std::get<std::vector<Value>>(data_);
}

const std::map<std::string, Value>& Value::as_object() const {
    if (type_ != Type::Object) {
        throw std::runtime_error("Value is not an object");
    }
    return std::get<std::map<std::string, Value>>(data_);
}

std::map<std::string, Value>& Value::as_object() {
    if (type_ != Type::Object) {
        throw std::runtime_error("Value is not an object");
    }
    return std::get<std::map<std::string, Value>>(data_);
}

// Safe access
const Value* Value::get(const std::string& key) const {
    if (type_ != Type::Object) return nullptr;
    const auto& obj = std::get<std::map<std::string, Value>>(data_);
    auto it = obj.find(key);
    return (it != obj.end()) ? &it->second : nullptr;
}

Value* Value::get(const std::string& key) {
    if (type_ != Type::Object) return nullptr;
    auto& obj = std::get<std::map<std::string, Value>>(data_);
    auto it = obj.find(key);
    return (it != obj.end()) ? &it->second : nullptr;
}

const Value* Value::get(size_t index) const {
    if (type_ != Type::Array) return nullptr;
    const auto& arr = std::get<std::vector<Value>>(data_);
    return (index < arr.size()) ? &arr[index] : nullptr;
}

Value* Value::get(size_t index) {
    if (type_ != Type::Array) return nullptr;
    auto& arr = std::get<std::vector<Value>>(data_);
    return (index < arr.size()) ? &arr[index] : nullptr;
}

// TOON serialization
// Helper to escape TOON strings
static std::string escape_toon_string(const std::string& s) {
    // Check if string needs quoting
    bool needs_quotes = false;
    
    if (s.empty()) {
        needs_quotes = true;
    } else if (s == "true" || s == "false" || s == "null") {
        needs_quotes = true;
    } else {
        // Check if it looks like a number
        try {
            size_t pos = 0;
            std::stod(s, &pos);
            if (pos == s.length()) {
                needs_quotes = true;
            }
        } catch (...) {
            // Not a number
        }
        
        // Check for special characters
        for (char c : s) {
            if (c == ':' || c == '\n' || c == '\r' || c == '\t' || c == '"' || c == '\\' || c < 32) {
                needs_quotes = true;
                break;
            }
        }
    }
    
    if (!needs_quotes) {
        return s;
    }
    
    // Quote and escape
    std::string result = "\"";
    for (char c : s) {
        switch (c) {
            case '\\': result += "\\\\"; break;
            case '"': result += "\\\""; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default: result += c; break;
        }
    }
    result += "\"";
    return result;
}

std::string Value::to_toon(int indent_size, int current_depth) const {
    std::ostringstream oss;
    std::string indent_str(current_depth * indent_size, ' ');
    std::string child_indent_str((current_depth + 1) * indent_size, ' ');
    
    switch (type_) {
        case Type::Null:
            oss << "null";
            break;
            
        case Type::Boolean:
            oss << (std::get<bool>(data_) ? "true" : "false");
            break;
            
        case Type::Number: {
            double val = std::get<double>(data_);
            // Check for integer values
            if (val == static_cast<long long>(val)) {
                oss << static_cast<long long>(val);
            } else {
                oss << val;
            }
            break;
        }
            
        case Type::String:
            oss << escape_toon_string(std::get<std::string>(data_));
            break;
            
        case Type::Array: {
            const auto& arr = std::get<std::vector<Value>>(data_);
            
            if (arr.empty()) {
                oss << "[0]:";
            } else {
                // Check if all elements are primitives
                bool all_primitives = true;
                for (const auto& elem : arr) {
                    if (!elem.is_null() && !elem.is_boolean() && !elem.is_number() && !elem.is_string()) {
                        all_primitives = false;
                        break;
                    }
                }
                
                if (all_primitives) {
                    // Inline array
                    oss << "[" << arr.size() << "]: ";
                    for (size_t i = 0; i < arr.size(); ++i) {
                        if (i > 0) oss << ", ";
                        oss << arr[i].to_toon(indent_size, 0);
                    }
                } else {
                    // List array
                    oss << "[" << arr.size() << "]:\n";
                    for (const auto& elem : arr) {
                        oss << child_indent_str << "- ";
                        if (elem.is_object()) {
                            // Inline first field if possible
                            const auto& obj = elem.as_object();
                            if (!obj.empty()) {
                                auto it = obj.begin();
                                oss << escape_toon_string(it->first) << ": " 
                                    << it->second.to_toon(indent_size, 0);
                                ++it;
                                
                                // Additional fields on new lines
                                for (; it != obj.end(); ++it) {
                                    oss << "\n" << std::string((current_depth + 2) * indent_size, ' ')
                                        << escape_toon_string(it->first) << ": "
                                        << it->second.to_toon(indent_size, 0);
                                }
                            }
                        } else {
                            oss << elem.to_toon(indent_size, 0);
                        }
                        oss << "\n";
                    }
                    // Remove trailing newline
                    std::string result = oss.str();
                    if (!result.empty() && result.back() == '\n') {
                        result.pop_back();
                    }
                    return result;
                }
            }
            break;
        }
            
        case Type::Object: {
            const auto& obj = std::get<std::map<std::string, Value>>(data_);
            
            if (current_depth == 0 && !obj.empty()) {
                // Root object - no braces
                bool first = true;
                for (const auto& [key, val] : obj) {
                    if (!first) oss << "\n";
                    first = false;
                    
                    oss << escape_toon_string(key) << ": ";
                    
                    if (val.is_object() && !val.as_object().empty()) {
                        oss << "\n";
                        const auto& child_obj = val.as_object();
                        for (const auto& [child_key, child_val] : child_obj) {
                            oss << child_indent_str << escape_toon_string(child_key) << ": "
                                << child_val.to_toon(indent_size, 0) << "\n";
                        }
                        // Remove trailing newline
                        std::string result = oss.str();
                        if (!result.empty() && result.back() == '\n') {
                            result.pop_back();
                        }
                        return result;
                    } else if (val.is_array()) {
                        oss << val.to_toon(indent_size, current_depth);
                    } else {
                        oss << val.to_toon(indent_size, 0);
                    }
                }
            } else {
                // Empty object or nested
                oss << "";
            }
            break;
        }
    }
    
    return oss.str();
}

} // namespace tq
