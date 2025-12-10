#include "tq/lexer.hpp"
#include <iostream>
#include <cassert>

using namespace tq;

void test_simple_field() {
    Lexer lexer(".name");
    auto tokens = lexer.tokenize();
    
    assert(tokens.size() == 3);  // Dot, Identifier, End
    assert(tokens[0].type == TokenType::Dot);
    assert(tokens[1].type == TokenType::Identifier);
    assert(tokens[1].value == "name");
    assert(tokens[2].type == TokenType::Eof);
    
    std::cout << " test_simple_field passed\n";
}

void test_nested_fields() {
    Lexer lexer(".user.email");
    auto tokens = lexer.tokenize();
    
    assert(tokens.size() == 5);  // Dot, Id, Dot, Id, End
    assert(tokens[0].type == TokenType::Dot);
    assert(tokens[1].value == "user");
    assert(tokens[2].type == TokenType::Dot);
    assert(tokens[3].value == "email");
    assert(tokens[4].type == TokenType::Eof);
    
    std::cout << " test_nested_fields passed\n";
}

void test_brackets() {
    Lexer lexer(".items[]");
    auto tokens = lexer.tokenize();
    
    assert(tokens.size() == 5);  // Dot, Id, [, ], End
    assert(tokens[0].type == TokenType::Dot);
    assert(tokens[1].value == "items");
    assert(tokens[2].type == TokenType::LeftBracket);
    assert(tokens[3].type == TokenType::RightBracket);
    assert(tokens[4].type == TokenType::Eof);
    
    std::cout << " test_brackets passed\n";
}

void test_complex() {
    Lexer lexer(".users[].email");
    auto tokens = lexer.tokenize();
    
    assert(tokens.size() == 7);  // Dot, Id, [, ], Dot, Id, End
    assert(tokens[1].value == "users");
    assert(tokens[5].value == "email");
    
    std::cout << " test_complex passed\n";
}

int main() {
    try {
        test_simple_field();
        test_nested_fields();
        test_brackets();
        test_complex();
        
        std::cout << "\nAll Lexer tests passed!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << "\n";
        return 1;
    }
}
