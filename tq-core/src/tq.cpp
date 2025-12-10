#include "tq/tq.hpp"

namespace tq {

std::vector<std::string> query(const std::string& expression, const std::string& data) {
    // Parse the data
    Value data_value = ToonParser::parse(data);
    
    // Tokenize and parse the expression
    Lexer lexer(expression);
    auto tokens = lexer.tokenize();
    
    Parser parser(std::move(tokens));
    auto steps = parser.parse();
    
    // Evaluate
    Evaluator evaluator;
    auto results = evaluator.evaluate(steps, data_value);
    
    // Convert results to TOON strings
    std::vector<std::string> toon_results;
    toon_results.reserve(results.size());
    
    for (const auto& result : results) {
        toon_results.push_back(result.to_toon());
    }
    
    return toon_results;
}

std::vector<Value> query_values(const std::string& expression, const Value& data) {
    // Tokenize and parse the expression
    Lexer lexer(expression);
    auto tokens = lexer.tokenize();
    
    Parser parser(std::move(tokens));
    auto steps = parser.parse();
    
    // Evaluate
    Evaluator evaluator;
    return evaluator.evaluate(steps, data);
}

} // namespace tq
