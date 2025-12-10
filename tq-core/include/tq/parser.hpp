#pragma once

#include "lexer.hpp"
#include <vector>
#include <memory>
#include <stdexcept>

namespace tq {

struct Step {
    enum class Type {
        Field,     // .field
        Iterate,   // []
        Index,     // [n]
        Slice,     // [start:end]
        BinOp      // binary operation
    };
    
    Type type;
    std::string field_name;  // only for Field type
    int index;               // for Index type
    int slice_start;         // for Slice type
    int slice_end;           // for Slice type
    bool has_slice_end;      // for Slice type
    
    // For binary operations
    TokenType op_type;       // Operation type
    std::shared_ptr<Step> left;
    std::shared_ptr<Step> right;
    
    explicit Step(Type t, std::string name = "")
        : type(t), field_name(std::move(name)), index(0), 
          slice_start(0), slice_end(0), has_slice_end(false),
          op_type(TokenType::Eof), left(nullptr), right(nullptr) {}
};

// Represents a pipeline of query expressions
struct Query {
    std::vector<std::vector<Step>> pipeline;  // Each element is one segment of the pipe
};

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);
    
    Query parse();
    std::vector<Step> parse_simple();  // Legacy: parse without pipes
    
private:
    std::vector<Token> tokens_;
    size_t pos_;
    
    const Token& current() const;
    const Token& peek(size_t offset = 1) const;
    void advance();
    bool is_at_end() const;
    bool check(TokenType type) const;
    bool match(TokenType type);
    
    void parse_dot_expression(std::vector<Step>& steps);
    void parse_bracket_suffix(std::vector<Step>& steps);
};

class ParseError : public std::runtime_error {
public:
    explicit ParseError(const std::string& msg) : std::runtime_error(msg) {}
};

} // namespace tq
