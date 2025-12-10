#include "tq/parser.hpp"

namespace tq {

Parser::Parser(std::vector<Token> tokens) : tokens_(std::move(tokens)), pos_(0) {}

const Token& Parser::current() const {
    return tokens_[pos_];
}

const Token& Parser::peek(size_t offset) const {
    size_t peek_pos = pos_ + offset;
    return (peek_pos >= tokens_.size()) ? tokens_.back() : tokens_[peek_pos];
}

void Parser::advance() {
    if (!is_at_end()) {
        ++pos_;
    }
}

bool Parser::is_at_end() const {
    return current().type == TokenType::Eof;
}

bool Parser::check(TokenType type) const {
    return current().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

void Parser::parse_dot_expression(std::vector<Step>& steps) {
    if (!match(TokenType::Dot)) {
        throw ParseError("Expected '.' at start of expression");
    }
    
    // Check for . followed by [ (identity + bracket)
    if (check(TokenType::LeftBracket)) {
        // Just . followed by [, don't add field step
        return;
    }
    
    if (check(TokenType::Identifier)) {
        std::string field = current().value;
        advance();
        steps.emplace_back(Step::Type::Field, field);
    } else {
        throw ParseError("Expected field name after '.'");
    }
}

void Parser::parse_bracket_suffix(std::vector<Step>& steps) {
    if (!match(TokenType::LeftBracket)) {
        throw ParseError("Expected '['");
    }
    
    // Check for empty brackets []
    if (check(TokenType::RightBracket)) {
        advance();
        steps.emplace_back(Step::Type::Iterate);
        return;
    }
    
    // Must be index or slice
    if (!check(TokenType::Number)) {
        throw ParseError("Expected number or ']' inside brackets");
    }
    
    int first_num = std::stoi(current().value);
    advance();
    
    // Check for slice [start:end]
    if (check(TokenType::Colon)) {
        advance();
        
        Step slice_step(Step::Type::Slice);
        slice_step.slice_start = first_num;
        
        if (check(TokenType::Number)) {
            slice_step.slice_end = std::stoi(current().value);
            slice_step.has_slice_end = true;
            advance();
        } else {
            slice_step.has_slice_end = false;
        }
        
        if (!match(TokenType::RightBracket)) {
            throw ParseError("Expected ']' after slice");
        }
        
        steps.push_back(slice_step);
    } else {
        // Simple index [n]
        if (!match(TokenType::RightBracket)) {
            throw ParseError("Expected ']' after index");
        }
        
        Step index_step(Step::Type::Index);
        index_step.index = first_num;
        steps.push_back(index_step);
    }
}

std::vector<Step> Parser::parse_simple() {
    std::vector<Step> steps;
    
    // Parse: .field[].otherfield[] ...
    while (!is_at_end() && !check(TokenType::Pipe)) {
        if (check(TokenType::Dot)) {
            parse_dot_expression(steps);
            
            // Optional bracket suffix
            if (check(TokenType::LeftBracket)) {
                parse_bracket_suffix(steps);
            }
        } else if (check(TokenType::LeftBracket)) {
            parse_bracket_suffix(steps);
        } else {
            throw ParseError("Unexpected token: expected '.' or '['");
        }
    }
    
    return steps;
}

Query Parser::parse() {
    Query query;
    
    // Parse segments separated by pipes
    while (!is_at_end()) {
        auto segment = parse_simple();
        if (!segment.empty()) {
            query.pipeline.push_back(std::move(segment));
        }
        
        if (match(TokenType::Pipe)) {
            // Continue to next segment
            if (is_at_end()) {
                throw ParseError("Expected expression after '|'");
            }
        }
    }
    
    if (query.pipeline.empty()) {
        throw ParseError("Empty query");
    }
    
    return query;
}

} // namespace tq
