#include "tq/evaluator.hpp"
#include "tq/parser.hpp"
#include "tq/lexer.hpp"
#include "tq/value.hpp"
#include <iostream>
#include <cassert>

using namespace tq;

void test_field_access() {
    std::map<std::string, Value> obj;
    obj["name"] = Value("Alice");
    obj["age"] = Value(30);
    Value data(std::move(obj));
    
    Lexer lexer(".name");
    Parser parser(lexer.tokenize());
    auto steps = parser.parse();
    
    Evaluator eval;
    auto results = eval.evaluate(steps, data);
    
    assert(results.size() == 1);
    assert(results[0].is_string());
    assert(results[0].as_string() == "Alice");
    
    std::cout << " test_field_access passed\n";
}

void test_nested_field_access() {
    std::map<std::string, Value> inner;
    inner["email"] = Value("alice@example.com");
    
    std::map<std::string, Value> obj;
    obj["user"] = Value(std::move(inner));
    Value data(std::move(obj));
    
    Lexer lexer(".user.email");
    Parser parser(lexer.tokenize());
    auto steps = parser.parse();
    
    Evaluator eval;
    auto results = eval.evaluate(steps, data);
    
    assert(results.size() == 1);
    assert(results[0].is_string());
    assert(results[0].as_string() == "alice@example.com");
    
    std::cout << " test_nested_field_access passed\n";
}

void test_array_iteration() {
    std::vector<Value> arr;
    arr.push_back(Value(1));
    arr.push_back(Value(2));
    arr.push_back(Value(3));
    
    std::map<std::string, Value> obj;
    obj["items"] = Value(std::move(arr));
    Value data(std::move(obj));
    
    Lexer lexer(".items[]");
    Parser parser(lexer.tokenize());
    auto steps = parser.parse();
    
    Evaluator eval;
    auto results = eval.evaluate(steps, data);
    
    assert(results.size() == 3);
    assert(results[0].as_number() == 1.0);
    assert(results[1].as_number() == 2.0);
    assert(results[2].as_number() == 3.0);
    
    std::cout << " test_array_iteration passed\n";
}

void test_array_field_fanout() {
    std::map<std::string, Value> user1;
    user1["name"] = Value("Alice");
    user1["email"] = Value("alice@example.com");
    
    std::map<std::string, Value> user2;
    user2["name"] = Value("Bob");
    user2["email"] = Value("bob@example.com");
    
    std::vector<Value> users;
    users.push_back(Value(std::move(user1)));
    users.push_back(Value(std::move(user2)));
    
    std::map<std::string, Value> obj;
    obj["users"] = Value(std::move(users));
    Value data(std::move(obj));
    
    Lexer lexer(".users[].email");
    Parser parser(lexer.tokenize());
    auto steps = parser.parse();
    
    Evaluator eval;
    auto results = eval.evaluate(steps, data);
    
    assert(results.size() == 2);
    assert(results[0].is_string());
    assert(results[0].as_string() == "alice@example.com");
    assert(results[1].is_string());
    assert(results[1].as_string() == "bob@example.com");
    
    std::cout << " test_array_field_fanout passed\n";
}

int main() {
    try {
        test_field_access();
        test_nested_field_access();
        test_array_iteration();
        test_array_field_fanout();
        
        std::cout << "\nAll Evaluator tests passed!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << "\n";
        return 1;
    }
}
