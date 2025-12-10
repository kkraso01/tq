#include "tq/lexer.hpp"
#include <cctype>
#include <unordered_map>

namespace tq {

static const std::unordered_map<std::string, TokenType> KEYWORDS = {
    {"true", TokenType::True},
    {"false", TokenType::False},
    {"null", TokenType::Null},
    {"and", TokenType::And},
    {"or", TokenType::Or},
    {"not", TokenType::Not},
    {"if", TokenType::If},
    {"then", TokenType::Then},
    {"else", TokenType::Else},
    {"elif", TokenType::Elif},
    {"end", TokenType::End},
    {"as", TokenType::As},
    {"def", TokenType::Def},
    {"try", TokenType::Try},
    {"catch", TokenType::Catch},
    {"reduce", TokenType::Reduce},
    {"foreach", TokenType::Foreach},
    {"while", TokenType::While},
    {"until", TokenType::Until},
    {"select", TokenType::Select},
    {"map", TokenType::Map},
    {"empty", TokenType::Empty},
    {"error", TokenType::Error}
};

Lexer::Lexer(std::string query) : query_(std::move(query)), pos_(0) {}

char Lexer::current() const {
    return is_at_end() ? '\0' : query_[pos_];
}

char Lexer::peek(size_t offset) const {
    size_t peek_pos = pos_ + offset;
    return (peek_pos >= query_.size()) ? '\0' : query_[peek_pos];
}

void Lexer::advance() {
    if (!is_at_end()) {
        ++pos_;
    }
}

bool Lexer::is_at_end() const {
    return pos_ >= query_.size();
}

void Lexer::skip_whitespace() {
    while (!is_at_end() && std::isspace(static_cast<unsigned char>(current()))) {
        advance();
    }
}

std::string Lexer::read_identifier() {
    std::string result;
    
    // Identifiers: alphanumeric + underscore
    while (!is_at_end()) {
        char c = current();
        if (std::isalnum(static_cast<unsigned char>(c)) || c == '_') {
            result += c;
            advance();
        } else {
            break;
        }
    }
    
    return result;
}

std::string Lexer::read_number() {
    std::string result;
    bool has_minus = false;
    
    // Handle negative numbers
    if (current() == '-') {
        result += '-';
        advance();
        has_minus = true;
    }
    
    // Read integer part
    while (!is_at_end() && std::isdigit(static_cast<unsigned char>(current()))) {
        result += current();
        advance();
    }
    
    // Read decimal part
    if (current() == '.' && std::isdigit(static_cast<unsigned char>(peek()))) {
        result += current();
        advance();
        while (!is_at_end() && std::isdigit(static_cast<unsigned char>(current()))) {
            result += current();
            advance();
        }
    }
    
    // Read exponent
    if ((current() == 'e' || current() == 'E')) {
        result += current();
        advance();
        if (current() == '+' || current() == '-') {
            result += current();
            advance();
        }
        while (!is_at_end() && std::isdigit(static_cast<unsigned char>(current()))) {
            result += current();
            advance();
        }
    }
    
    if (result.empty() || (has_minus && result.size() == 1)) {
        throw LexerError("Invalid number at position " + std::to_string(pos_));
    }
    
    return result;
}

std::string Lexer::read_string() {
    std::string result;
    advance(); // skip opening quote
    
    while (!is_at_end() && current() != '"') {
        if (current() == '\\') {
            advance();
            if (is_at_end()) {
                throw LexerError("Unterminated string escape");
            }
            switch (current()) {
                case 'n': result += '\n'; break;
                case 't': result += '\t'; break;
                case 'r': result += '\r'; break;
                case '\\': result += '\\'; break;
                case '"': result += '"'; break;
                case '/': result += '/'; break;
                default:
                    throw LexerError("Invalid escape sequence: \\" + std::string(1, current()));
            }
            advance();
        } else {
            result += current();
            advance();
        }
    }
    
    if (is_at_end()) {
        throw LexerError("Unterminated string");
    }
    
    advance(); // skip closing quote
    return result;
}

TokenType Lexer::check_keyword(const std::string& id) {
    auto it = KEYWORDS.find(id);
    if (it != KEYWORDS.end()) {
        return it->second;
    }
    return TokenType::Identifier;
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    
    while (!is_at_end()) {
        skip_whitespace();
        
        if (is_at_end()) break;
        
        char c = current();
        size_t token_pos = pos_;
        
        switch (c) {
            case '.':
                advance();
                if (current() == '.') {
                    tokens.emplace_back(TokenType::DoubleDot, "", token_pos);
                    advance();
                } else {
                    tokens.emplace_back(TokenType::Dot, "", token_pos);
                }
                break;
                
            case '[':
                tokens.emplace_back(TokenType::LeftBracket, "", token_pos);
                advance();
                break;
                
            case ']':
                tokens.emplace_back(TokenType::RightBracket, "", token_pos);
                advance();
                break;
                
            case '{':
                tokens.emplace_back(TokenType::LeftBrace, "", token_pos);
                advance();
                break;
                
            case '}':
                tokens.emplace_back(TokenType::RightBrace, "", token_pos);
                advance();
                break;
                
            case '(':
                tokens.emplace_back(TokenType::LeftParen, "", token_pos);
                advance();
                break;
                
            case ')':
                tokens.emplace_back(TokenType::RightParen, "", token_pos);
                advance();
                break;
                
            case '|':
                advance();
                if (current() == '=') {
                    tokens.emplace_back(TokenType::UpdateAssign, "", token_pos);
                    advance();
                } else {
                    tokens.emplace_back(TokenType::Pipe, "", token_pos);
                }
                break;
                
            case ':':
                tokens.emplace_back(TokenType::Colon, "", token_pos);
                advance();
                break;
                
            case ';':
                tokens.emplace_back(TokenType::Semicolon, "", token_pos);
                advance();
                break;
                
            case ',':
                tokens.emplace_back(TokenType::Comma, "", token_pos);
                advance();
                break;
                
            case '?':
                tokens.emplace_back(TokenType::Question, "", token_pos);
                advance();
                break;
                
            case '+':
                advance();
                if (current() == '=') {
                    tokens.emplace_back(TokenType::PlusAssign, "", token_pos);
                    advance();
                } else {
                    tokens.emplace_back(TokenType::Plus, "", token_pos);
                }
                break;
                
            case '*':
                advance();
                if (current() == '=') {
                    tokens.emplace_back(TokenType::StarAssign, "", token_pos);
                    advance();
                } else {
                    tokens.emplace_back(TokenType::Star, "", token_pos);
                }
                break;
                
            case '%':
                tokens.emplace_back(TokenType::Percent, "", token_pos);
                advance();
                break;
                
            case '/':
                advance();
                if (current() == '/') {
                    advance();
                    if (current() == '=') {
                        tokens.emplace_back(TokenType::AltAssign, "", token_pos);
                        advance();
                    } else {
                        tokens.emplace_back(TokenType::Alternative, "", token_pos);
                    }
                } else if (current() == '=') {
                    tokens.emplace_back(TokenType::SlashAssign, "", token_pos);
                    advance();
                } else {
                    tokens.emplace_back(TokenType::Slash, "", token_pos);
                }
                break;
                
            case '=':
                advance();
                if (current() == '=') {
                    tokens.emplace_back(TokenType::Equal, "", token_pos);
                    advance();
                } else {
                    tokens.emplace_back(TokenType::Assign, "", token_pos);
                }
                break;
                
            case '!':
                advance();
                if (current() == '=') {
                    tokens.emplace_back(TokenType::NotEqual, "", token_pos);
                    advance();
                } else {
                    throw LexerError("Unexpected '!' at position " + std::to_string(token_pos));
                }
                break;
                
            case '<':
                advance();
                if (current() == '=') {
                    tokens.emplace_back(TokenType::LessEqual, "", token_pos);
                    advance();
                } else {
                    tokens.emplace_back(TokenType::Less, "", token_pos);
                }
                break;
                
            case '>':
                advance();
                if (current() == '=') {
                    tokens.emplace_back(TokenType::GreaterEqual, "", token_pos);
                    advance();
                } else {
                    tokens.emplace_back(TokenType::Greater, "", token_pos);
                }
                break;
                
            case '-':
                advance();
                if (current() == '=') {
                    tokens.emplace_back(TokenType::MinusAssign, "", token_pos);
                    advance();
                } else if (std::isdigit(static_cast<unsigned char>(current()))) {
                    // Negative number
                    pos_--; // back up to include the minus
                    std::string num = read_number();
                    tokens.emplace_back(TokenType::Number, num, token_pos);
                } else {
                    tokens.emplace_back(TokenType::Minus, "", token_pos);
                }
                break;
                
            case '"':
                {
                    std::string str = read_string();
                    tokens.emplace_back(TokenType::String, str, token_pos);
                }
                break;
                
            default:
                if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
                    std::string id = read_identifier();
                    TokenType type = check_keyword(id);
                    tokens.emplace_back(type, type == TokenType::Identifier ? id : "", token_pos);
                } else if (std::isdigit(static_cast<unsigned char>(c))) {
                    std::string num = read_number();
                    tokens.emplace_back(TokenType::Number, num, token_pos);
                } else {
                    throw LexerError("Unexpected character '" + std::string(1, c) + 
                                   "' at position " + std::to_string(token_pos));
                }
        }
    }
    
    tokens.emplace_back(TokenType::Eof, "", pos_);
    return tokens;
}

} // namespace tq
