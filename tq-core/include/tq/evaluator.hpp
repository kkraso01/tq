#pragma once

#include "value.hpp"
#include "parser.hpp"
#include <vector>

namespace tq {

class Evaluator {
public:
    Evaluator() = default;
    
    // Evaluate full query (with pipes) against data
    std::vector<Value> evaluate(const Query& query, const Value& data);
    
    // Evaluate simple query (no pipes) - legacy
    std::vector<Value> evaluate_simple(const std::vector<Step>& steps, const Value& data);
    
private:
    // Helper: apply one step to a single value, return results
    std::vector<Value> apply_step(const Step& step, const Value& current);
};

} // namespace tq
