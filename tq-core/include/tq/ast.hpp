#pragma once

#include "lexer.hpp"
#include "value.hpp"
#include <vector>
#include <memory>
#include <string>
#include <variant>

namespace tq {

// Forward declarations
struct Expr;
using ExprPtr = std::shared_ptr<Expr>;

// Expression types
enum class ExprType {
    // Literals
    Null,
    Boolean,
    Number,
    String,
    Array,
    Object,
    
    // Path operations
    Identity,           // .
    Field,              // .foo
    OptionalField,      // .foo?
    Index,              // .[0]
    Slice,              // .[1:5]
    Iterator,           // .[]
    RecursiveDescent,   // ..
    
    // Operators
    Pipe,               // |
    Comma,              // ,
    BinaryOp,           // +, -, *, /, %, ==, !=, <, <=, >, >=, and, or, //
    UnaryOp,            // -, not
    
    // Conditionals
    If,                 // if-then-else
    Try,                // try-catch
    
    // Functions
    FunctionCall,       // func(args)
    
    // Assignment
    Assignment,         // .foo = value, .foo |= expr, etc.
    
    // Iteration
    Reduce,             // reduce
    Foreach,            // foreach
    
    // Variables
    Variable,           // $var
    AsPattern,          // expr as $var | ...
    
    // Function definition
    FunctionDef         // def name(args): body;
};

// Base expression
struct Expr {
    ExprType type;
    
    // Literal values
    bool bool_val = false;
    double num_val = 0.0;
    std::string str_val;
    std::vector<ExprPtr> array_elements;
    std::vector<std::pair<std::string, ExprPtr>> object_fields;
    
    // Path operations
    std::string field_name;
    bool optional = false;
    int index_val = 0;
    int slice_start = 0;
    int slice_end = 0;
    bool has_slice_end = false;
    
    // Binary/Unary operations
    TokenType op = TokenType::Eof;
    ExprPtr left;
    ExprPtr right;
    ExprPtr operand;
    
    // Conditionals
    ExprPtr condition;
    ExprPtr then_branch;
    ExprPtr else_branch;
    std::vector<std::pair<ExprPtr, ExprPtr>> elif_branches; // (condition, body) pairs
    
    // Function call
    std::string func_name;
    std::vector<ExprPtr> args;
    
    // Variable
    std::string var_name;
    ExprPtr var_expr;     // For as-pattern
    ExprPtr body_expr;    // For as-pattern body
    
    // Reduce/Foreach
    ExprPtr reduce_iter_expr;   // expression to iterate over (e.g. .[])
    ExprPtr init_expr;          // initial accumulator
    ExprPtr update_expr;        // update expression
    ExprPtr extract_expr;       // optional extract expression
    
    // Function definition
    std::vector<std::string> param_names;
    ExprPtr func_body;
    
    explicit Expr(ExprType t) : type(t) {}
    
    static ExprPtr null_expr() { return std::make_shared<Expr>(ExprType::Null); }
    static ExprPtr bool_expr(bool val) {
        auto e = std::make_shared<Expr>(ExprType::Boolean);
        e->bool_val = val;
        return e;
    }
    static ExprPtr number_expr(double val) {
        auto e = std::make_shared<Expr>(ExprType::Number);
        e->num_val = val;
        return e;
    }
    static ExprPtr string_expr(std::string val) {
        auto e = std::make_shared<Expr>(ExprType::String);
        e->str_val = std::move(val);
        return e;
    }
    static ExprPtr identity_expr() {
        return std::make_shared<Expr>(ExprType::Identity);
    }
    static ExprPtr field_expr(std::string name, bool opt = false) {
        auto e = std::make_shared<Expr>(opt ? ExprType::OptionalField : ExprType::Field);
        e->field_name = std::move(name);
        return e;
    }
    static ExprPtr iterator_expr() {
        return std::make_shared<Expr>(ExprType::Iterator);
    }
    static ExprPtr recursive_descent_expr() {
        return std::make_shared<Expr>(ExprType::RecursiveDescent);
    }
};

// Query - a complete jq expression
struct Query {
    ExprPtr root;
    
    Query() : root(nullptr) {}
    explicit Query(ExprPtr r) : root(std::move(r)) {}
};

} // namespace tq
