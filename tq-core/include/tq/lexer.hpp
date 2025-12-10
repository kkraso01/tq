#pragma once

#include <string>
#include <vector>
#include <stdexcept>

namespace tq {

enum class TokenType {
    // Structural
    Dot,           // .
    DoubleDot,     // ..
    Identifier,    // field name
    LeftBracket,   // [
    RightBracket,  // ]
    LeftBrace,     // {
    RightBrace,    // }
    LeftParen,     // (
    RightParen,    // )
    Pipe,          // |
    Colon,         // :
    Semicolon,     // ;
    Comma,         // ,
    Question,      // ?
    
    // Literals
    Number,        // number literal
    String,        // string literal
    True,          // true
    False,         // false
    Null,          // null
    
    // Arithmetic
    Plus,          // +
    Minus,         // -
    Star,          // *
    Slash,         // /
    Percent,       // %
    
    // Comparison
    Equal,         // ==
    NotEqual,      // !=
    Less,          // <
    LessEqual,     // <=
    Greater,       // >
    GreaterEqual,  // >=
    
    // Logical
    And,           // and
    Or,            // or
    Not,           // not
    
    // Alternative
    Alternative,   // //
    
    // Assignment
    Assign,        // =
    UpdateAssign,  // |=
    PlusAssign,    // +=
    MinusAssign,   // -=
    StarAssign,    // *=
    SlashAssign,   // /=
    AltAssign,     // //=
    
    // Keywords
    If,            // if
    Then,          // then
    Else,          // else
    Elif,          // elif
    End,           // end
    As,            // as
    Def,           // def
    Try,           // try
    Catch,         // catch
    Reduce,        // reduce
    Foreach,       // foreach
    While,         // while
    Until,         // until
    
    // Function names (built-ins)
    Select,        // select
    Map,           // map
    Empty,         // empty
    Error,         // error
    
    Eof            // end of input
};

struct Token {
    TokenType type;
    std::string value;  // only used for Identifier
    size_t position;
    
    Token(TokenType t, std::string v = "", size_t pos = 0)
        : type(t), value(std::move(v)), position(pos) {}
};

class Lexer {
public:
    explicit Lexer(std::string query);
    
    std::vector<Token> tokenize();
    
private:
    std::string query_;
    size_t pos_;
    
    char current() const;
    char peek(size_t offset = 1) const;
    void advance();
    bool is_at_end() const;
    
    void skip_whitespace();
    std::string read_identifier();
    std::string read_number();
    std::string read_string();
    TokenType check_keyword(const std::string& id);
};

class LexerError : public std::runtime_error {
public:
    explicit LexerError(const std::string& msg) : std::runtime_error(msg) {}
};

} // namespace tq
