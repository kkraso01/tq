#include "tq/parser.hpp"
#include <stdexcept>

namespace tq {

Parser::Parser(std::vector<Token> tokens) : tokens_(std::move(tokens)), pos_(0) {}

const Token& Parser::current() const {
    return pos_ < tokens_.size() ? tokens_[pos_] : tokens_.back();
}

const Token& Parser::peek(size_t offset) const {
    size_t peek_pos = pos_ + offset;
    return peek_pos < tokens_.size() ? tokens_[peek_pos] : tokens_.back();
}

void Parser::advance() {
    if (!is_at_end()) ++pos_;
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

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) {
        Token tok = current();
        advance();
        return tok;
    }
    throw ParseError(message);
}

Query Parser::parse() {
    if (is_at_end()) {
        return Query(Expr::identity_expr());
    }
    
    ExprPtr expr = parse_expression();
    
    if (!is_at_end()) {
        throw ParseError("Unexpected tokens after expression");
    }
    
    return Query(expr);
}

// Parse full expression (lowest precedence)
ExprPtr Parser::parse_expression() {
    return parse_pipe();
}

// Pipe: expr | expr
ExprPtr Parser::parse_pipe() {
    ExprPtr left = parse_comma();
    
    while (match(TokenType::Pipe)) {
        ExprPtr right = parse_comma();
        auto pipe_expr = std::make_shared<Expr>(ExprType::Pipe);
        pipe_expr->left = left;
        pipe_expr->right = right;
        left = pipe_expr;
    }
    
    return left;
}

// Comma: expr, expr
ExprPtr Parser::parse_comma() {
    ExprPtr left = parse_assignment();
    
    while (match(TokenType::Comma)) {
        ExprPtr right = parse_assignment();
        auto comma_expr = std::make_shared<Expr>(ExprType::Comma);
        comma_expr->left = left;
        comma_expr->right = right;
        left = comma_expr;
    }
    
    return left;
}

// Assignment: path = expr, path |= expr, path += expr, etc.
ExprPtr Parser::parse_assignment() {
    ExprPtr left = parse_or();
    
    if (match(TokenType::Assign) || match(TokenType::UpdateAssign) ||
        match(TokenType::PlusAssign) || match(TokenType::MinusAssign) ||
        match(TokenType::StarAssign) || match(TokenType::SlashAssign) ||
        match(TokenType::AltAssign)) {
        
        TokenType assign_op = tokens_[pos_ - 1].type;
        ExprPtr right = parse_expression();
        
        auto assign_expr = std::make_shared<Expr>(ExprType::Assignment);
        assign_expr->op = assign_op;
        assign_expr->left = left;
        assign_expr->right = right;
        return assign_expr;
    }
    
    return left;
}

// Or: expr or expr
ExprPtr Parser::parse_or() {
    ExprPtr left = parse_and();
    
    while (match(TokenType::Or)) {
        ExprPtr right = parse_and();
        auto or_expr = std::make_shared<Expr>(ExprType::BinaryOp);
        or_expr->op = TokenType::Or;
        or_expr->left = left;
        or_expr->right = right;
        left = or_expr;
    }
    
    return left;
}

// And: expr and expr
ExprPtr Parser::parse_and() {
    ExprPtr left = parse_equality();
    
    while (match(TokenType::And)) {
        ExprPtr right = parse_equality();
        auto and_expr = std::make_shared<Expr>(ExprType::BinaryOp);
        and_expr->op = TokenType::And;
        and_expr->left = left;
        and_expr->right = right;
        left = and_expr;
    }
    
    return left;
}

// Equality: expr == expr, expr != expr
ExprPtr Parser::parse_equality() {
    ExprPtr left = parse_comparison();
    
    while (match(TokenType::Equal) || match(TokenType::NotEqual)) {
        TokenType op = tokens_[pos_ - 1].type;
        ExprPtr right = parse_comparison();
        auto eq_expr = std::make_shared<Expr>(ExprType::BinaryOp);
        eq_expr->op = op;
        eq_expr->left = left;
        eq_expr->right = right;
        left = eq_expr;
    }
    
    return left;
}

// Comparison: expr < expr, expr <= expr, etc.
ExprPtr Parser::parse_comparison() {
    ExprPtr left = parse_alternative();
    
    while (match(TokenType::Less) || match(TokenType::LessEqual) ||
           match(TokenType::Greater) || match(TokenType::GreaterEqual)) {
        TokenType op = tokens_[pos_ - 1].type;
        ExprPtr right = parse_alternative();
        auto cmp_expr = std::make_shared<Expr>(ExprType::BinaryOp);
        cmp_expr->op = op;
        cmp_expr->left = left;
        cmp_expr->right = right;
        left = cmp_expr;
    }
    
    return left;
}

// Alternative: expr // expr
ExprPtr Parser::parse_alternative() {
    ExprPtr left = parse_additive();
    
    while (match(TokenType::Alternative)) {
        ExprPtr right = parse_additive();
        auto alt_expr = std::make_shared<Expr>(ExprType::BinaryOp);
        alt_expr->op = TokenType::Alternative;
        alt_expr->left = left;
        alt_expr->right = right;
        left = alt_expr;
    }
    
    return left;
}

// Additive: expr + expr, expr - expr
ExprPtr Parser::parse_additive() {
    ExprPtr left = parse_multiplicative();
    
    while (match(TokenType::Plus) || match(TokenType::Minus)) {
        TokenType op = tokens_[pos_ - 1].type;
        ExprPtr right = parse_multiplicative();
        auto add_expr = std::make_shared<Expr>(ExprType::BinaryOp);
        add_expr->op = op;
        add_expr->left = left;
        add_expr->right = right;
        left = add_expr;
    }
    
    return left;
}

// Multiplicative: expr * expr, expr / expr, expr % expr
ExprPtr Parser::parse_multiplicative() {
    ExprPtr left = parse_unary();
    
    while (match(TokenType::Star) || match(TokenType::Slash) || match(TokenType::Percent)) {
        TokenType op = tokens_[pos_ - 1].type;
        ExprPtr right = parse_unary();
        auto mul_expr = std::make_shared<Expr>(ExprType::BinaryOp);
        mul_expr->op = op;
        mul_expr->left = left;
        mul_expr->right = right;
        left = mul_expr;
    }
    
    return left;
}

// Unary: not expr, -expr
ExprPtr Parser::parse_unary() {
    if (match(TokenType::Not) || match(TokenType::Minus)) {
        TokenType op = tokens_[pos_ - 1].type;
        ExprPtr operand = parse_unary();
        auto unary_expr = std::make_shared<Expr>(ExprType::UnaryOp);
        unary_expr->op = op;
        unary_expr->operand = operand;
        return unary_expr;
    }
    
    return parse_postfix();
}

// Postfix: expr[...], expr.field, expr?, etc.
ExprPtr Parser::parse_postfix() {
    ExprPtr expr = parse_primary();
    
    while (true) {
        if (check(TokenType::LeftBracket)) {
            expr = parse_index_or_slice(expr);
        } else if (check(TokenType::Dot)) {
            // Check if it's .field or just standalone .
            if (peek().type == TokenType::Identifier) {
                expr = parse_field_access(expr);
            } else {
                break;
            }
        } else if (match(TokenType::Question)) {
            // Optional access
            expr->optional = true;
        } else {
            break;
        }
    }
    
    return expr;
}

// Primary expressions
ExprPtr Parser::parse_primary() {
    // Literals
    if (match(TokenType::Null)) {
        return Expr::null_expr();
    }
    
    if (match(TokenType::True)) {
        return Expr::bool_expr(true);
    }
    
    if (match(TokenType::False)) {
        return Expr::bool_expr(false);
    }
    
    if (match(TokenType::Number)) {
        double val = std::stod(tokens_[pos_ - 1].value);
        return Expr::number_expr(val);
    }
    
    if (match(TokenType::String)) {
        return Expr::string_expr(tokens_[pos_ - 1].value);
    }
    
    // Array literal
    if (check(TokenType::LeftBracket)) {
        return parse_array_literal();
    }
    
    // Object literal
    if (check(TokenType::LeftBrace)) {
        return parse_object_literal();
    }
    
    // Parenthesized expression
    if (check(TokenType::LeftParen)) {
        return parse_parenthesized();
    }
    
    // Conditionals
    if (check(TokenType::If)) {
        return parse_if();
    }
    
    if (check(TokenType::Try)) {
        return parse_try();
    }
    
    // Iteration constructs
    if (check(TokenType::Reduce)) {
        return parse_reduce();
    }
    
    if (check(TokenType::Foreach)) {
        return parse_foreach();
    }
    
    // Identity or field access
    if (match(TokenType::Dot)) {
        if (match(TokenType::Dot)) {
            // .. recursive descent
            return Expr::recursive_descent_expr();
        } else if (check(TokenType::Identifier)) {
            std::string field = current().value;
            advance();
            return Expr::field_expr(field);
        } else if (check(TokenType::LeftBracket)) {
            // .[] or .[index]
            return Expr::identity_expr();
        } else {
            // Just . (identity)
            return Expr::identity_expr();
        }
    }
    
    // Function call or identifier
    if (check(TokenType::Identifier)) {
        std::string name = current().value;
        advance();
        
        if (check(TokenType::LeftParen)) {
            return parse_function_call(name);
        }
        
        // Treat as built-in function without parens
        auto call_expr = std::make_shared<Expr>(ExprType::FunctionCall);
        call_expr->func_name = name;
        return call_expr;
    }
    
    // Format functions (@base64, @uri, @csv, etc.)
    if (match(TokenType::Format)) {
        std::string format_name = tokens_[pos_ - 1].value;
        auto call_expr = std::make_shared<Expr>(ExprType::FunctionCall);
        call_expr->func_name = "@" + format_name;
        return call_expr;
    }
    
    // Built-in function names
    if (match(TokenType::Select) || match(TokenType::Map) || 
        match(TokenType::Empty) || match(TokenType::Error)) {
        std::string func_name = tokens_[pos_ - 1].value.empty() ? 
                               (tokens_[pos_ - 1].type == TokenType::Select ? "select" :
                                tokens_[pos_ - 1].type == TokenType::Map ? "map" :
                                tokens_[pos_ - 1].type == TokenType::Empty ? "empty" : "error") :
                               tokens_[pos_ - 1].value;
        
        if (check(TokenType::LeftParen)) {
            return parse_function_call(func_name);
        }
        
        auto call_expr = std::make_shared<Expr>(ExprType::FunctionCall);
        call_expr->func_name = func_name;
        return call_expr;
    }
    
    throw ParseError("Unexpected token: " + std::to_string(static_cast<int>(current().type)));
}

ExprPtr Parser::parse_index_or_slice(ExprPtr base) {
    consume(TokenType::LeftBracket, "Expected '['");
    
    // Check for .[]
    if (match(TokenType::RightBracket)) {
        auto iter_expr = std::make_shared<Expr>(ExprType::Iterator);
        auto pipe_expr = std::make_shared<Expr>(ExprType::Pipe);
        pipe_expr->left = base;
        pipe_expr->right = iter_expr;
        return pipe_expr;
    }
    
    // Parse index or slice
    ExprPtr index_expr = parse_expression();
    
    if (match(TokenType::Colon)) {
        // Slice
        auto slice_expr = std::make_shared<Expr>(ExprType::Slice);
        
        // Get start from index_expr (must be a number literal for now)
        if (index_expr->type == ExprType::Number) {
            slice_expr->slice_start = static_cast<int>(index_expr->num_val);
        } else {
            throw ParseError("Slice start must be a number");
        }
        
        if (!check(TokenType::RightBracket)) {
            ExprPtr end_expr = parse_expression();
            if (end_expr->type == ExprType::Number) {
                slice_expr->slice_end = static_cast<int>(end_expr->num_val);
                slice_expr->has_slice_end = true;
            } else {
                throw ParseError("Slice end must be a number");
            }
        }
        
        consume(TokenType::RightBracket, "Expected ']'");
        
        auto pipe_expr = std::make_shared<Expr>(ExprType::Pipe);
        pipe_expr->left = base;
        pipe_expr->right = slice_expr;
        return pipe_expr;
    }
    
    consume(TokenType::RightBracket, "Expected ']'");
    
    // Simple index
    auto idx_expr = std::make_shared<Expr>(ExprType::Index);
    if (index_expr->type == ExprType::Number) {
        idx_expr->index_val = static_cast<int>(index_expr->num_val);
    } else {
        throw ParseError("Index must be a number");
    }
    
    auto pipe_expr = std::make_shared<Expr>(ExprType::Pipe);
    pipe_expr->left = base;
    pipe_expr->right = idx_expr;
    return pipe_expr;
}

ExprPtr Parser::parse_field_access(ExprPtr base) {
    consume(TokenType::Dot, "Expected '.'");
    Token field_tok = consume(TokenType::Identifier, "Expected field name");
    
    auto field_expr = Expr::field_expr(field_tok.value);
    
    auto pipe_expr = std::make_shared<Expr>(ExprType::Pipe);
    pipe_expr->left = base;
    pipe_expr->right = field_expr;
    return pipe_expr;
}

ExprPtr Parser::parse_function_call(const std::string& name) {
    consume(TokenType::LeftParen, "Expected '('");
    
    auto call_expr = std::make_shared<Expr>(ExprType::FunctionCall);
    call_expr->func_name = name;
    
    if (!check(TokenType::RightParen)) {
        do {
            call_expr->args.push_back(parse_expression());
        } while (match(TokenType::Semicolon));
    }
    
    consume(TokenType::RightParen, "Expected ')'");
    return call_expr;
}

ExprPtr Parser::parse_array_literal() {
    consume(TokenType::LeftBracket, "Expected '['");
    
    auto arr_expr = std::make_shared<Expr>(ExprType::Array);
    
    if (!check(TokenType::RightBracket)) {
        do {
            arr_expr->array_elements.push_back(parse_expression());
        } while (match(TokenType::Comma));
    }
    
    consume(TokenType::RightBracket, "Expected ']'");
    return arr_expr;
}

ExprPtr Parser::parse_object_literal() {
    consume(TokenType::LeftBrace, "Expected '{'");
    
    auto obj_expr = std::make_shared<Expr>(ExprType::Object);
    
    if (!check(TokenType::RightBrace)) {
        do {
            // Parse key
            std::string key;
            if (check(TokenType::Identifier)) {
                key = current().value;
                advance();
            } else if (check(TokenType::String)) {
                key = current().value;
                advance();
            } else if (check(TokenType::LeftParen)) {
                // Computed key: (expr)
                advance();
                ExprPtr key_expr = parse_expression();
                consume(TokenType::RightParen, "Expected ')'");
                // For now, we'll store it as a special marker
                // In full implementation, this would need special handling
                key = "<computed>";
            } else {
                throw ParseError("Expected object key");
            }
            
            consume(TokenType::Colon, "Expected ':'");
            ExprPtr value = parse_expression();
            
            obj_expr->object_fields.push_back({key, value});
        } while (match(TokenType::Comma));
    }
    
    consume(TokenType::RightBrace, "Expected '}'");
    return obj_expr;
}

ExprPtr Parser::parse_parenthesized() {
    consume(TokenType::LeftParen, "Expected '('");
    ExprPtr expr = parse_expression();
    consume(TokenType::RightParen, "Expected ')'");
    return expr;
}

ExprPtr Parser::parse_if() {
    consume(TokenType::If, "Expected 'if'");
    
    auto if_expr = std::make_shared<Expr>(ExprType::If);
    if_expr->condition = parse_expression();
    
    consume(TokenType::Then, "Expected 'then'");
    if_expr->then_branch = parse_expression();
    
    // Handle elif branches
    while (match(TokenType::Elif)) {
        ExprPtr elif_cond = parse_expression();
        consume(TokenType::Then, "Expected 'then'");
        ExprPtr elif_body = parse_expression();
        if_expr->elif_branches.push_back({elif_cond, elif_body});
    }
    
    if (match(TokenType::Else)) {
        if_expr->else_branch = parse_expression();
    }
    
    consume(TokenType::End, "Expected 'end'");
    return if_expr;
}

ExprPtr Parser::parse_try() {
    consume(TokenType::Try, "Expected 'try'");
    
    auto try_expr = std::make_shared<Expr>(ExprType::Try);
    try_expr->left = parse_expression();
    
    if (match(TokenType::Catch)) {
        try_expr->right = parse_expression();
    }
    
    return try_expr;
}

ExprPtr Parser::parse_reduce() {
    consume(TokenType::Reduce, "Expected 'reduce'");
    
    auto reduce_expr = std::make_shared<Expr>(ExprType::Reduce);
    reduce_expr->reduce_iter_expr = parse_expression();
    
    consume(TokenType::As, "Expected 'as'");
    // Parse variable pattern (simplified - just $var for now)
    // Full implementation would handle destructuring
    
    consume(TokenType::LeftParen, "Expected '('");
    reduce_expr->init_expr = parse_expression();
    consume(TokenType::Semicolon, "Expected ';'");
    reduce_expr->update_expr = parse_expression();
    consume(TokenType::RightParen, "Expected ')'");
    
    return reduce_expr;
}

ExprPtr Parser::parse_foreach() {
    consume(TokenType::Foreach, "Expected 'foreach'");
    
    auto foreach_expr = std::make_shared<Expr>(ExprType::Foreach);
    foreach_expr->reduce_iter_expr = parse_expression();
    
    consume(TokenType::As, "Expected 'as'");
    // Parse variable pattern
    
    consume(TokenType::LeftParen, "Expected '('");
    foreach_expr->init_expr = parse_expression();
    consume(TokenType::Semicolon, "Expected ';'");
    foreach_expr->update_expr = parse_expression();
    
    if (match(TokenType::Semicolon)) {
        foreach_expr->extract_expr = parse_expression();
    }
    
    consume(TokenType::RightParen, "Expected ')'");
    
    return foreach_expr;
}

ExprPtr Parser::parse_function_def() {
    // Simplified - full implementation would handle def statements
    throw ParseError("Function definitions not yet implemented");
}

} // namespace tq
