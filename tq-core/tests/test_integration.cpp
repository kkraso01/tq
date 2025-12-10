#include "tq/tq.hpp"
#include <iostream>
#include <cassert>

using namespace tq;

void test_simple_query() {
    std::string toon = "name: Alice\nage: 30";
    auto results = query(".name", toon);
    
    assert(results.size() == 1);
    assert(results[0] == "Alice");
    
    std::cout << " test_simple_query passed\n";
}

void test_array_query() {
    std::string toon = "users[2]:\n  - email: a@b.com\n  - email: c@d.com";
    auto results = query(".users[].email", toon);
    
    assert(results.size() == 2);
    assert(results[0] == "a@b.com");
    assert(results[1] == "c@d.com");
    
    std::cout << " test_array_query passed\n";
}

void test_nested_query() {
    std::string toon = "data:\n  user:\n    name: Bob";
    auto results = query(".data.user.name", toon);
    
    assert(results.size() == 1);
    assert(results[0] == "Bob");
    
    std::cout << " test_nested_query passed\n";
}

void test_empty_result() {
    std::string toon = "name: Alice";
    auto results = query(".nonexistent", toon);
    
    assert(results.size() == 0);
    
    std::cout << " test_empty_result passed\n";
}

int main() {
    try {
        test_simple_query();
        test_array_query();
        test_nested_query();
        test_empty_result();
        
        std::cout << "\nAll Integration tests passed!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << "\n";
        return 1;
    }
}
