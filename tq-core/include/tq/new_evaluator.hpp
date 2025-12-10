#pragma once

#include "ast.hpp"
#include "value.hpp"
#include <vector>
#include <map>
#include <string>
#include <functional>

namespace tq {

// Built-in function type
using BuiltinFunc = std::function<std::vector<Value>(const std::vector<std::vector<Value>>&)>;

class NewEvaluator {
public:
    NewEvaluator();
    
    // Evaluate expression against data, returning multiple results (jq stream semantics)
    std::vector<Value> eval(const ExprPtr& expr, const Value& data);
    
private:
    // Built-in functions registry
    std::map<std::string, BuiltinFunc> builtins_;
    
    // Variable environment (for as-patterns, reduce, etc.)
    std::map<std::string, Value> vars_;
    
    // Register all built-in functions
    void register_builtins();
    
    // Expression evaluation
    std::vector<Value> eval_identity(const Value& data);
    std::vector<Value> eval_field(const ExprPtr& expr, const Value& data);
    std::vector<Value> eval_index(const ExprPtr& expr, const Value& data);
    std::vector<Value> eval_slice(const ExprPtr& expr, const Value& data);
    std::vector<Value> eval_iterator(const Value& data);
    std::vector<Value> eval_recursive_descent(const Value& data);
    std::vector<Value> eval_pipe(const ExprPtr& expr, const Value& data);
    std::vector<Value> eval_comma(const ExprPtr& expr, const Value& data);
    std::vector<Value> eval_binary_op(const ExprPtr& expr, const Value& data);
    std::vector<Value> eval_unary_op(const ExprPtr& expr, const Value& data);
    std::vector<Value> eval_if(const ExprPtr& expr, const Value& data);
    std::vector<Value> eval_try(const ExprPtr& expr, const Value& data);
    std::vector<Value> eval_function_call(const ExprPtr& expr, const Value& data);
    std::vector<Value> eval_array_literal(const ExprPtr& expr, const Value& data);
    std::vector<Value> eval_object_literal(const ExprPtr& expr, const Value& data);
    std::vector<Value> eval_reduce(const ExprPtr& expr, const Value& data);
    std::vector<Value> eval_foreach(const ExprPtr& expr, const Value& data);
    
    // Helpers
    bool is_truthy(const Value& val);
    Value apply_arithmetic(TokenType op, const Value& left, const Value& right);
    bool apply_comparison(TokenType op, const Value& left, const Value& right);
    int compare_values(const Value& a, const Value& b);
    
    // Built-in functions
    std::vector<Value> builtin_select(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_map(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_add(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_length(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_keys(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_values(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_type(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_sort(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_reverse(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_group_by(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_unique(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_min(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_max(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_has(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_to_entries(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_from_entries(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_split(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_join(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_tostring(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_tonumber(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_startswith(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_endswith(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_floor(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_ceil(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_round(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_sqrt(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_empty(const std::vector<std::vector<Value>>& args);
    // ... many more
};

} // namespace tq
