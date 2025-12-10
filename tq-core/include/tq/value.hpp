#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <variant>
#include <stdexcept>

namespace tq {

class Value {
public:
    enum class Type {
        Null,
        Boolean,
        Number,
        String,
        Array,
        Object
    };

    // Constructors
    Value();  // null
    explicit Value(bool b);
    explicit Value(double d);
    explicit Value(int i);
    explicit Value(const std::string& s);
    explicit Value(std::string&& s);
    explicit Value(const char* s);
    
    // Array constructor
    explicit Value(std::vector<Value>&& arr);
    explicit Value(const std::vector<Value>& arr);
    
    // Object constructor
    explicit Value(std::map<std::string, Value>&& obj);
    explicit Value(const std::map<std::string, Value>& obj);

    // Rule of 5
    Value(const Value& other);
    Value(Value&& other) noexcept;
    Value& operator=(const Value& other);
    Value& operator=(Value&& other) noexcept;
    ~Value();

    // Type checking
    Type type() const { return type_; }
    bool is_null() const { return type_ == Type::Null; }
    bool is_boolean() const { return type_ == Type::Boolean; }
    bool is_number() const { return type_ == Type::Number; }
    bool is_string() const { return type_ == Type::String; }
    bool is_array() const { return type_ == Type::Array; }
    bool is_object() const { return type_ == Type::Object; }

    // Getters (throw if wrong type)
    bool as_boolean() const;
    double as_number() const;
    const std::string& as_string() const;
    const std::vector<Value>& as_array() const;
    std::vector<Value>& as_array();
    const std::map<std::string, Value>& as_object() const;
    std::map<std::string, Value>& as_object();

    // Safe access for objects
    const Value* get(const std::string& key) const;
    Value* get(const std::string& key);
    
    // Safe access for arrays
    const Value* get(size_t index) const;
    Value* get(size_t index);

    // Serialize to TOON string
    std::string to_toon(int indent_size = 2, int current_depth = 0) const;

private:
    Type type_;
    
    // Use variant for efficient storage
    std::variant<
        std::monostate,  // null
        bool,            // boolean
        double,          // number
        std::string,     // string
        std::vector<Value>,  // array
        std::map<std::string, Value>  // object
    > data_;
};

} // namespace tq
