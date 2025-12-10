#include "tq/value.hpp"
#include <iostream>
#include <cassert>

using namespace tq;

void test_null() {
    Value v;
    assert(v.is_null());
    std::cout << " test_null passed\n";
}

void test_boolean() {
    Value t(true);
    Value f(false);
    assert(t.is_boolean());
    assert(t.as_boolean() == true);
    assert(f.as_boolean() == false);
    std::cout << " test_boolean passed\n";
}

void test_number() {
    Value v1(42);
    Value v2(3.14);
    assert(v1.is_number());
    assert(v1.as_number() == 42.0);
    assert(v2.as_number() == 3.14);
    std::cout << " test_number passed\n";
}

void test_string() {
    Value v("hello");
    assert(v.is_string());
    assert(v.as_string() == "hello");
    std::cout << " test_string passed\n";
}

void test_array() {
    std::vector<Value> arr;
    arr.push_back(Value(1));
    arr.push_back(Value(2));
    arr.push_back(Value(3));
    
    Value v(std::move(arr));
    assert(v.is_array());
    assert(v.as_array().size() == 3);
    assert(v.as_array()[0].as_number() == 1.0);
    std::cout << " test_array passed\n";
}

void test_object() {
    std::map<std::string, Value> obj;
    obj["name"] = Value("Alice");
    obj["age"] = Value(30);
    
    Value v(std::move(obj));
    assert(v.is_object());
    assert(v.as_object().size() == 2);
    assert(v.as_object()["name"].as_string() == "Alice");
    std::cout << " test_object passed\n";
}

int main() {
    try {
        test_null();
        test_boolean();
        test_number();
        test_string();
        test_array();
        test_object();
        
        std::cout << "\nAll Value tests passed!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << "\n";
        return 1;
    }
}
