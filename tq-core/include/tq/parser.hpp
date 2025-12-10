#pragma once

#include "lexer.hpp"
#include "ast.hpp"
#include <memory>
#include <stdexcept>

namespace tq {

// Forward declare ParseError if not already defined
class ParseError : public std::runtime_error {
public:
    explicit ParseError(const std::string& msg) : std::runtime_error(msg) {}
};

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);
    
    // Parse complete query
    Query parse();
    
private:
    std::vector<Token> tokens_;
    size_t pos_;
    
    // Token navigation
    const Token& current() const;
    const Token& peek(size_t offset = 1) const;
    void advance();
    bool is_at_end() const;
    bool check(TokenType type) const;
    bool match(TokenType type);
    Token consume(TokenType type, const std::string& message);
    
    // Expression parsing (precedence climbing)
    ExprPtr parse_expression();
    ExprPtr parse_pipe();
    ExprPtr parse_comma();
    ExprPtr parse_assignment();
    ExprPtr parse_or();
    ExprPtr parse_and();
    ExprPtr parse_equality();
    ExprPtr parse_comparison();
    ExprPtr parse_alternative();
    ExprPtr parse_additive();
    ExprPtr parse_multiplicative();
    ExprPtr parse_unary();
    ExprPtr parse_postfix();
    ExprPtr parse_primary();
    
    // Specific constructs
    ExprPtr parse_if();
    ExprPtr parse_try();
    ExprPtr parse_reduce();
    ExprPtr parse_foreach();
    ExprPtr parse_function_def();
    ExprPtr parse_function_call(const std::string& name);
    ExprPtr parse_array_literal();
    ExprPtr parse_object_literal();
    ExprPtr parse_parenthesized();
    
    // Postfix operations
    ExprPtr parse_index_or_slice(ExprPtr base);
    ExprPtr parse_field_access(ExprPtr base);
    ExprPtr parse_optional_access(ExprPtr base);
};

} // namespace tq
