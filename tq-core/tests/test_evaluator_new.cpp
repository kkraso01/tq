#include <iostream>
#include <cassert>
#include <cmath>
#include "tq/lexer.hpp"
#include "tq/parser.hpp"
#include "tq/evaluator.hpp"
#include "tq/value.hpp"

using namespace tq;

// Test helpers
Value parse_and_eval(const std::string& query, const Value& data) {
    Lexer lexer(query);
    auto tokens = lexer.tokenize();
    Parser parser(tokens);
    auto q = parser.parse();
    Evaluator evaluator;
    auto results = evaluator.eval(q.root, data);
    return results.empty() ? Value() : results[0];
}

void test_identity() {
    std::cout << "Testing identity operator..." << std::endl;
    Value data(42.0);
    auto result = parse_and_eval(".", data);
    assert(result.is_number() && result.as_number() == 42.0);
    std::cout << " Identity operator works" << std::endl;
}

void test_field_access() {
    std::cout << "Testing field access..." << std::endl;
    std::map<std::string, Value> obj;
    obj["name"] = Value("Alice");
    obj["age"] = Value(30.0);
    Value data(obj);
    
    auto result = parse_and_eval(".name", data);
    assert(result.is_string() && result.as_string() == "Alice");
    std::cout << " Field access works" << std::endl;
}

void test_array_index() {
    std::cout << "Testing array indexing..." << std::endl;
    std::vector<Value> arr = {Value(10.0), Value(20.0), Value(30.0)};
    Value data(arr);
    
    auto result = parse_and_eval(".[1]", data);
    assert(result.is_number() && result.as_number() == 20.0);
    std::cout << " Array indexing works" << std::endl;
}

void test_pipe_operator() {
    std::cout << "Testing pipe operator..." << std::endl;
    std::vector<Value> arr = {Value(1.0), Value(2.0), Value(3.0)};
    Value data(arr);
    
    Lexer lexer(".[0] | . + 5");
    auto tokens = lexer.tokenize();
    Parser parser(tokens);
    auto q = parser.parse();
    Evaluator evaluator;
    auto results = evaluator.eval(q.root, data);
    
    assert(!results.empty() && results[0].is_number() && results[0].as_number() == 6.0);
    std::cout << " Pipe operator works" << std::endl;
}

void test_arithmetic() {
    std::cout << "Testing arithmetic operations..." << std::endl;
    Value data(10.0);
    
    auto add = parse_and_eval(". + 5", data);
    assert(add.is_number() && add.as_number() == 15.0);
    
    auto sub = parse_and_eval(". - 3", data);
    assert(sub.is_number() && sub.as_number() == 7.0);
    
    auto mul = parse_and_eval(". * 2", data);
    assert(mul.is_number() && mul.as_number() == 20.0);
    
    auto div = parse_and_eval(". / 5", data);
    assert(div.is_number() && div.as_number() == 2.0);
    
    std::cout << " Arithmetic operations work" << std::endl;
}

void test_comparison() {
    std::cout << "Testing comparison operators..." << std::endl;
    
    Value five(5.0);
    auto eq = parse_and_eval(". == 5", five);
    assert(eq.is_boolean() && eq.as_boolean() == true);
    
    auto ne = parse_and_eval(". != 5", five);
    assert(ne.is_boolean() && ne.as_boolean() == false);
    
    auto lt = parse_and_eval(". < 10", five);
    assert(lt.is_boolean() && lt.as_boolean() == true);
    
    auto gt = parse_and_eval(". > 3", five);
    assert(gt.is_boolean() && gt.as_boolean() == true);
    
    std::cout << " Comparison operators work" << std::endl;
}

void test_type_builtin() {
    std::cout << "Testing type function..." << std::endl;
    
    auto num_type = parse_and_eval("type", Value(42.0));
    assert(num_type.is_string() && num_type.as_string() == "number");
    
    auto str_type = parse_and_eval("type", Value("hello"));
    assert(str_type.is_string() && str_type.as_string() == "string");
    
    auto arr_type = parse_and_eval("type", Value(std::vector<Value>()));
    assert(arr_type.is_string() && arr_type.as_string() == "array");
    
    std::cout << " type function works" << std::endl;
}

void test_length_builtin() {
    std::cout << "Testing length function..." << std::endl;
    
    auto str_len = parse_and_eval("length", Value("hello"));
    assert(str_len.is_number() && str_len.as_number() == 5.0);
    
    std::vector<Value> arr = {Value(1.0), Value(2.0), Value(3.0)};
    auto arr_len = parse_and_eval("length", Value(arr));
    assert(arr_len.is_number() && arr_len.as_number() == 3.0);
    
    std::cout << " length function works" << std::endl;
}

void test_math_functions() {
    std::cout << "Testing math functions..." << std::endl;
    
    auto floor_val = parse_and_eval("floor", Value(3.7));
    assert(floor_val.is_number() && floor_val.as_number() == 3.0);
    
    auto ceil_val = parse_and_eval("ceil", Value(3.2));
    assert(ceil_val.is_number() && ceil_val.as_number() == 4.0);
    
    auto round_val = parse_and_eval("round", Value(3.5));
    assert(round_val.is_number() && round_val.as_number() == 4.0);
    
    auto abs_val = parse_and_eval("abs", Value(-5.0));
    assert(abs_val.is_number() && abs_val.as_number() == 5.0);
    
    auto sqrt_val = parse_and_eval("sqrt", Value(16.0));
    assert(sqrt_val.is_number() && sqrt_val.as_number() == 4.0);
    
    std::cout << " Math functions work" << std::endl;
}

void test_string_functions() {
    std::cout << "Testing string functions..." << std::endl;
    
    auto downcase = parse_and_eval("ascii_downcase", Value("HELLO"));
    assert(downcase.is_string() && downcase.as_string() == "hello");
    
    auto upcase = parse_and_eval("ascii_upcase", Value("hello"));
    assert(upcase.is_string() && upcase.as_string() == "HELLO");
    
    std::cout << " String functions work" << std::endl;
}

void test_array_functions() {
    std::cout << "Testing array functions..." << std::endl;
    
    std::vector<Value> arr = {Value(3.0), Value(1.0), Value(2.0)};
    Value data(arr);
    
    auto sorted = parse_and_eval("sort", data);
    assert(sorted.is_array());
    const auto& sorted_arr = sorted.as_array();
    assert(sorted_arr[0].as_number() == 1.0);
    assert(sorted_arr[1].as_number() == 2.0);
    assert(sorted_arr[2].as_number() == 3.0);
    
    auto reversed = parse_and_eval("reverse", Value(arr));
    assert(reversed.is_array());
    const auto& rev_arr = reversed.as_array();
    assert(rev_arr[0].as_number() == 2.0);
    
    std::cout << " Array functions work" << std::endl;
}

void test_if_then_else() {
    std::cout << "Testing if-then-else..." << std::endl;
    
    Value data(5.0);
    auto result = parse_and_eval("if . > 3 then \"big\" else \"small\" end", data);
    assert(result.is_string() && result.as_string() == "big");
    
    Value data2(2.0);
    auto result2 = parse_and_eval("if . > 3 then \"big\" else \"small\" end", data2);
    assert(result2.is_string() && result2.as_string() == "small");
    
    std::cout << " if-then-else works" << std::endl;
}

void test_logical_operators() {
    std::cout << "Testing logical operators..." << std::endl;
    
    Value data_true(true);
    auto and_result = parse_and_eval("true and false", data_true);
    assert(and_result.is_boolean() && and_result.as_boolean() == false);
    
    auto or_result = parse_and_eval("false or true", data_true);
    assert(or_result.is_boolean() && or_result.as_boolean() == true);
    
    std::cout << " Logical operators work" << std::endl;
}

void test_map_function() {
    std::cout << "Testing map function..." << std::endl;
    
    std::vector<Value> arr = {Value(1.0), Value(2.0), Value(3.0)};
    Value data(arr);
    
    Lexer lexer("map(. + 1)");
    auto tokens = lexer.tokenize();
    Parser parser(tokens);
    auto q = parser.parse();
    Evaluator evaluator;
    auto results = evaluator.eval(q.root, data);
    
    assert(!results.empty() && results[0].is_array());
    const auto& mapped = results[0].as_array();
    assert(mapped[0].as_number() == 2.0);
    assert(mapped[1].as_number() == 3.0);
    assert(mapped[2].as_number() == 4.0);
    
    std::cout << " map function works" << std::endl;
}

void test_select_function() {
    std::cout << "Testing select function..." << std::endl;
    
    std::vector<Value> arr = {Value(1.0), Value(2.0), Value(3.0), Value(4.0)};
    Value data(arr);
    
    Lexer lexer(".[] | select(. > 2)");
    auto tokens = lexer.tokenize();
    Parser parser(tokens);
    auto q = parser.parse();
    Evaluator evaluator;
    auto results = evaluator.eval(q.root, data);
    
    assert(results.size() == 2);
    assert(results[0].as_number() == 3.0);
    assert(results[1].as_number() == 4.0);
    
    std::cout << " select function works" << std::endl;
}

void test_sort_by_function() {
    std::cout << "Testing sort_by function..." << std::endl;
    
    std::vector<Value> arr;
    std::map<std::string, Value> obj1;
    obj1["name"] = Value("Charlie");
    obj1["age"] = Value(30.0);
    arr.push_back(Value(obj1));
    
    std::map<std::string, Value> obj2;
    obj2["name"] = Value("Alice");
    obj2["age"] = Value(25.0);
    arr.push_back(Value(obj2));
    
    Value data(arr);
    
    Lexer lexer("sort_by(.age)");
    auto tokens = lexer.tokenize();
    Parser parser(tokens);
    auto q = parser.parse();
    Evaluator evaluator;
    auto results = evaluator.eval(q.root, data);
    
    assert(!results.empty() && results[0].is_array());
    const auto& sorted = results[0].as_array();
    assert(sorted[0].as_object().at("name").as_string() == "Alice");
    assert(sorted[1].as_object().at("name").as_string() == "Charlie");
    
    std::cout << " sort_by function works" << std::endl;
}

void test_unique_function() {
    std::cout << "Testing unique function..." << std::endl;
    
    std::vector<Value> arr = {Value(1.0), Value(2.0), Value(1.0), Value(3.0), Value(2.0)};
    Value data(arr);
    
    auto result = parse_and_eval("unique", data);
    assert(result.is_array());
    const auto& unique_arr = result.as_array();
    assert(unique_arr.size() == 3);
    assert(unique_arr[0].as_number() == 1.0);
    assert(unique_arr[1].as_number() == 2.0);
    assert(unique_arr[2].as_number() == 3.0);
    
    std::cout << " unique function works" << std::endl;
}

void test_first_last() {
    std::cout << "Testing first/last functions..." << std::endl;
    
    std::vector<Value> arr = {Value(1.0), Value(2.0), Value(3.0)};
    Value data(arr);
    
    auto first = parse_and_eval("first", data);
    assert(first.is_number() && first.as_number() == 1.0);
    
    auto last = parse_and_eval("last", data);
    assert(last.is_number() && last.as_number() == 3.0);
    
    std::cout << " first/last functions work" << std::endl;
}

void test_flatten() {
    std::cout << "Testing flatten function..." << std::endl;
    
    std::vector<Value> inner1 = {Value(1.0), Value(2.0)};
    std::vector<Value> inner2 = {Value(3.0), Value(4.0)};
    std::vector<Value> outer = {Value(inner1), Value(inner2)};
    Value data(outer);
    
    auto result = parse_and_eval("flatten", data);
    assert(result.is_array());
    const auto& flat = result.as_array();
    assert(flat.size() == 4);
    assert(flat[0].as_number() == 1.0);
    assert(flat[3].as_number() == 4.0);
    
    std::cout << " flatten function works" << std::endl;
}

int main() {
    std::cout << "Running TQ Evaluator Test Suite...\n" << std::endl;
    
    try {
        test_identity();
        test_field_access();
        test_array_index();
        test_pipe_operator();
        test_arithmetic();
        test_comparison();
        test_type_builtin();
        test_length_builtin();
        test_math_functions();
        test_string_functions();
        test_array_functions();
        test_if_then_else();
        test_logical_operators();
        test_map_function();
        test_select_function();
        test_sort_by_function();
        test_unique_function();
        test_first_last();
        test_flatten();
        
        std::cout << "\n All tests passed!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << " Test failed with error: " << e.what() << std::endl;
        return 1;
    }
}
