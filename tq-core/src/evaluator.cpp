#include "tq/evaluator.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace tq {

Evaluator::Evaluator() {
    register_builtins();
}

void Evaluator::register_builtins() {
    // Value-based built-ins
    builtins_["length"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_length(args); };
    builtins_["keys"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_keys(args); };
    builtins_["values"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_values(args); };
    builtins_["type"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_type(args); };
    builtins_["add"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_add(args); };
    builtins_["sort"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_sort(args); };
    builtins_["reverse"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_reverse(args); };
    builtins_["unique"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_unique(args); };
    builtins_["min"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_min(args); };
    builtins_["max"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_max(args); };
    builtins_["empty"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_empty(args); };
    builtins_["tostring"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_tostring(args); };
    builtins_["tonumber"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_tonumber(args); };
    builtins_["floor"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_floor(args); };
    builtins_["sqrt"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_sqrt(args); };
    builtins_["has"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_has(args); };
    builtins_["to_entries"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_to_entries(args); };
    builtins_["from_entries"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_from_entries(args); };
    builtins_["split"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_split(args); };
    builtins_["join"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_join(args); };
    builtins_["startswith"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_startswith(args); };
    builtins_["endswith"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_endswith(args); };
    builtins_["ceil"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_ceil(args); };
    builtins_["round"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_round(args); };
    builtins_["abs"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_abs(args); };
    builtins_["pow"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_pow(args); };
    builtins_["log"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_log(args); };
    builtins_["log10"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_log10(args); };
    builtins_["log2"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_log2(args); };
    builtins_["exp"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_exp(args); };
    builtins_["exp10"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_exp10(args); };
    builtins_["exp2"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_exp2(args); };
    builtins_["sin"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_sin(args); };
    builtins_["cos"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_cos(args); };
    builtins_["tan"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_tan(args); };
    builtins_["asin"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_asin(args); };
    builtins_["acos"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_acos(args); };
    builtins_["atan"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_atan(args); };
    builtins_["ltrimstr"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_ltrimstr(args); };
    builtins_["rtrimstr"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_rtrimstr(args); };
    builtins_["ascii_downcase"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_ascii_downcase(args); };
    builtins_["ascii_upcase"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_ascii_upcase(args); };
    builtins_["contains"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_contains(args); };
    builtins_["index"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_index(args); };
    builtins_["rindex"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_rindex(args); };
    builtins_["inside"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_inside(args); };
    builtins_["indices"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_indices(args); };
    builtins_["first"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_first(args); };
    builtins_["last"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_last(args); };
    builtins_["nth"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_nth(args); };
    builtins_["range"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_range(args); };
    builtins_["flatten"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_flatten(args); };
    builtins_["transpose"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_transpose(args); };
    builtins_["error"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_error(args); };
    builtins_["debug"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_debug(args); };
    builtins_["not"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_not(args); };
    
    // Expression-based built-ins
    expr_builtins_["map"] = [this](Evaluator* eval, const ExprPtr& expr, const Value& data) { 
        return this->builtin_map(expr, data); 
    };
    expr_builtins_["select"] = [this](Evaluator* eval, const ExprPtr& expr, const Value& data) { 
        return this->builtin_select(expr, data); 
    };
    expr_builtins_["sort_by"] = [this](Evaluator* eval, const ExprPtr& expr, const Value& data) { 
        return this->builtin_sort_by(expr, data); 
    };
    expr_builtins_["unique_by"] = [this](Evaluator* eval, const ExprPtr& expr, const Value& data) { 
        return this->builtin_unique_by(expr, data); 
    };
    expr_builtins_["group_by"] = [this](Evaluator* eval, const ExprPtr& expr, const Value& data) { 
        return this->builtin_group_by(expr, data); 
    };
    expr_builtins_["min_by"] = [this](Evaluator* eval, const ExprPtr& expr, const Value& data) { 
        return this->builtin_min_by(expr, data); 
    };
    expr_builtins_["max_by"] = [this](Evaluator* eval, const ExprPtr& expr, const Value& data) { 
        return this->builtin_max_by(expr, data); 
    };
    expr_builtins_["any"] = [this](Evaluator* eval, const ExprPtr& expr, const Value& data) { 
        return this->builtin_any(expr, data); 
    };
    expr_builtins_["all"] = [this](Evaluator* eval, const ExprPtr& expr, const Value& data) { 
        return this->builtin_all(expr, data); 
    };
}

// Main evaluation dispatcher
std::vector<Value> Evaluator::eval(const ExprPtr& expr, const Value& data) {
    if (!expr) {
        return {data};
    }

    switch (expr->type) {

        case ExprType::Null:
            return {Value()};

        case ExprType::Boolean:
            return {Value(expr->bool_val)};

        case ExprType::Number:
            return {Value(expr->num_val)};

        case ExprType::String:
            return {Value(expr->str_val)};

        case ExprType::Identity:
            return eval_identity(data);

        case ExprType::Field:
        case ExprType::OptionalField:
            return eval_field(expr, data);

        case ExprType::Index:
            return eval_index(expr, data);

        case ExprType::Slice:
            return eval_slice(expr, data);

        case ExprType::Iterator:
            return eval_iterator(data);

        case ExprType::RecursiveDescent:
            return eval_recursive_descent(data);

        case ExprType::Pipe:
            return eval_pipe(expr, data);

        case ExprType::Comma:
            return eval_comma(expr, data);

        case ExprType::BinaryOp:
            return eval_binary_op(expr, data);

        case ExprType::UnaryOp:
            return eval_unary_op(expr, data);

        case ExprType::If:
            return eval_if(expr, data);

        case ExprType::Try:
            return eval_try(expr, data);

        case ExprType::FunctionCall:
            return eval_function_call(expr, data);

        case ExprType::Array:
            return eval_array_literal(expr, data);

        case ExprType::Object:
            return eval_object_literal(expr, data);

        case ExprType::Reduce:
            return eval_reduce(expr, data);

        case ExprType::Foreach:
            return eval_foreach(expr, data);

        default:
            throw std::runtime_error("Unsupported expression type");
    }
}

std::vector<Value> Evaluator::eval_identity(const Value& data) {
    return {data};
}

std::vector<Value> Evaluator::eval_field(const ExprPtr& expr, const Value& data) {
    if (!data.is_object()) {
        if (expr->type == ExprType::OptionalField) {
            return {Value()}; // Return null for optional access on non-object
        }
        return {}; // Empty result for required field on non-object
    }
    
    const Value* field_val = data.get(expr->field_name);
    if (field_val) {
        return {*field_val};
    }
    
    if (expr->type == ExprType::OptionalField) {
        return {Value()};
    }
    
    return {Value()}; // jq returns null for missing fields
}

std::vector<Value> Evaluator::eval_index(const ExprPtr& expr, const Value& data) {
    if (!data.is_array()) {
        return {};
    }
    
    const auto& arr = data.as_array();
    int idx = expr->index_val;
    
    // Handle negative indices
    if (idx < 0) {
        idx = static_cast<int>(arr.size()) + idx;
    }
    
    if (idx >= 0 && idx < static_cast<int>(arr.size())) {
        return {arr[idx]};
    }
    
    return {Value()}; // Out of bounds returns null
}

std::vector<Value> Evaluator::eval_slice(const ExprPtr& expr, const Value& data) {
    if (!data.is_array()) {
        return {};
    }
    
    const auto& arr = data.as_array();
    int size = static_cast<int>(arr.size());
    
    int start = expr->slice_start;
    if (start < 0) start = size + start;
    if (start < 0) start = 0;
    if (start > size) start = size;
    
    int end = expr->has_slice_end ? expr->slice_end : size;
    if (end < 0) end = size + end;
    if (end < 0) end = 0;
    if (end > size) end = size;
    if (end < start) end = start;
    
    std::vector<Value> result_arr;
    for (int i = start; i < end; ++i) {
        result_arr.push_back(arr[i]);
    }
    
    return {Value(std::move(result_arr))};
}

std::vector<Value> Evaluator::eval_iterator(const Value& data) {
    std::vector<Value> results;
    
    if (data.is_array()) {
        const auto& arr = data.as_array();
        for (const auto& elem : arr) {
            results.push_back(elem);
        }
    } else if (data.is_object()) {
        const auto& obj = data.as_object();
        for (const auto& [key, val] : obj) {
            results.push_back(val);
        }
    }
    
    return results;
}

std::vector<Value> Evaluator::eval_recursive_descent(const Value& data) {
    std::vector<Value> results;
    results.push_back(data);
    
    // Recursively add all nested values
    std::function<void(const Value&)> recurse = [&](const Value& val) {
        if (val.is_array()) {
            for (const auto& elem : val.as_array()) {
                results.push_back(elem);
                recurse(elem);
            }
        } else if (val.is_object()) {
            for (const auto& [key, v] : val.as_object()) {
                results.push_back(v);
                recurse(v);
            }
        }
    };
    
    recurse(data);
    return results;
}

std::vector<Value> Evaluator::eval_pipe(const ExprPtr& expr, const Value& data) {
    // Evaluate left side first
    std::vector<Value> left_results = eval(expr->left, data);
    
    // Apply right side to each result from left
    std::vector<Value> final_results;
    for (const auto& val : left_results) {
        std::vector<Value> right_results = eval(expr->right, val);
        final_results.insert(final_results.end(), right_results.begin(), right_results.end());
    }
    
    return final_results;
}

std::vector<Value> Evaluator::eval_comma(const ExprPtr& expr, const Value& data) {
    // Comma produces multiple outputs
    std::vector<Value> results;
    
    std::vector<Value> left_results = eval(expr->left, data);
    results.insert(results.end(), left_results.begin(), left_results.end());
    
    std::vector<Value> right_results = eval(expr->right, data);
    results.insert(results.end(), right_results.begin(), right_results.end());
    
    return results;
}

std::vector<Value> Evaluator::eval_binary_op(const ExprPtr& expr, const Value& data) {
    std::vector<Value> left_results = eval(expr->left, data);
    std::vector<Value> right_results = eval(expr->right, data);
    
    if (left_results.empty() || right_results.empty()) {
        return {};
    }
    
    const Value& left_val = left_results[0];
    const Value& right_val = right_results[0];
    
    // Handle short-circuit operators
    if (expr->op == TokenType::And) {
        if (!is_truthy(left_val)) {
            return {Value(false)};
        }
        return {Value(is_truthy(right_val))};
    }
    
    if (expr->op == TokenType::Or) {
        if (is_truthy(left_val)) {
            return {Value(true)};
        }
        return {Value(is_truthy(right_val))};
    }
    
    // Alternative operator: return left if not null/false, else right
    if (expr->op == TokenType::Alternative) {
        if (!left_val.is_null() && !(left_val.is_boolean() && !left_val.as_boolean())) {
            return {left_val};
        }
        return {right_val};
    }
    
    // Arithmetic operators
    if (expr->op == TokenType::Plus || expr->op == TokenType::Minus ||
        expr->op == TokenType::Star || expr->op == TokenType::Slash ||
        expr->op == TokenType::Percent) {
        return {apply_arithmetic(expr->op, left_val, right_val)};
    }
    
    // Comparison operators
    if (expr->op == TokenType::Equal || expr->op == TokenType::NotEqual ||
        expr->op == TokenType::Less || expr->op == TokenType::LessEqual ||
        expr->op == TokenType::Greater || expr->op == TokenType::GreaterEqual) {
        return {Value(apply_comparison(expr->op, left_val, right_val))};
    }
    
    throw std::runtime_error("Unsupported binary operator");
}

std::vector<Value> Evaluator::eval_unary_op(const ExprPtr& expr, const Value& data) {
    std::vector<Value> operand_results = eval(expr->operand, data);
    
    if (operand_results.empty()) {
        return {};
    }
    
    const Value& operand_val = operand_results[0];
    
    if (expr->op == TokenType::Not) {
        return {Value(!is_truthy(operand_val))};
    }
    
    if (expr->op == TokenType::Minus) {
        if (operand_val.is_number()) {
            return {Value(-operand_val.as_number())};
        }
        throw std::runtime_error("Cannot negate non-number");
    }
    
    throw std::runtime_error("Unsupported unary operator");
}

std::vector<Value> Evaluator::eval_if(const ExprPtr& expr, const Value& data) {
    std::vector<Value> cond_results = eval(expr->condition, data);
    
    if (cond_results.empty() || !is_truthy(cond_results[0])) {
        // Check elif branches
        for (const auto& [elif_cond, elif_body] : expr->elif_branches) {
            std::vector<Value> elif_results = eval(elif_cond, data);
            if (!elif_results.empty() && is_truthy(elif_results[0])) {
                return eval(elif_body, data);
            }
        }
        
        // Else branch
        if (expr->else_branch) {
            return eval(expr->else_branch, data);
        }
        return {};
    }
    
    return eval(expr->then_branch, data);
}

std::vector<Value> Evaluator::eval_try(const ExprPtr& expr, const Value& data) {
    try {
        return eval(expr->left, data);
    } catch (...) {
        if (expr->right) {
            return eval(expr->right, data);
        }
        return {};
    }
}

std::vector<Value> Evaluator::eval_function_call(const ExprPtr& expr, const Value& data) {
    // Check for expression-based built-ins first
    auto expr_it = expr_builtins_.find(expr->func_name);
    if (expr_it != expr_builtins_.end()) {
        // Expression-based function expects one expression argument
        if (expr->args.size() != 1) {
            throw std::runtime_error(expr->func_name + " requires exactly one expression argument");
        }
        return expr_it->second(this, expr->args[0], data);
    }
    
    // Regular value-based built-ins
    auto it = builtins_.find(expr->func_name);
    if (it == builtins_.end()) {
        throw std::runtime_error("Unknown function: " + expr->func_name);
    }
    
    // Evaluate arguments
    std::vector<std::vector<Value>> arg_results;
    for (const auto& arg : expr->args) {
        arg_results.push_back(eval(arg, data));
    }
    
    // Add data as implicit first argument
    if (expr->func_name != "empty") {
        arg_results.insert(arg_results.begin(), {data});
    }
    
    return it->second(arg_results);
}

std::vector<Value> Evaluator::eval_array_literal(const ExprPtr& expr, const Value& data) {
    std::vector<Value> result_arr;
    
    for (const auto& elem_expr : expr->array_elements) {
        std::vector<Value> elem_results = eval(elem_expr, data);
        // For array literals, take first result of each element
        if (!elem_results.empty()) {
            result_arr.push_back(elem_results[0]);
        }
    }
    
    return {Value(std::move(result_arr))};
}

std::vector<Value> Evaluator::eval_object_literal(const ExprPtr& expr, const Value& data) {
    std::map<std::string, Value> result_obj;
    
    for (const auto& [key, val_expr] : expr->object_fields) {
        std::vector<Value> val_results = eval(val_expr, data);
        if (!val_results.empty()) {
            result_obj[key] = val_results[0];
        }
    }
    
    return {Value(std::move(result_obj))};
}

std::vector<Value> Evaluator::eval_reduce(const ExprPtr& expr, const Value& data) {
    // Simplified reduce - full implementation would handle variables properly
    throw std::runtime_error("reduce not yet implemented");
}

std::vector<Value> Evaluator::eval_foreach(const ExprPtr& expr, const Value& data) {
    throw std::runtime_error("foreach not yet implemented");
}

// Helper functions

bool Evaluator::is_truthy(const Value& val) {
    if (val.is_null()) return false;
    if (val.is_boolean()) return val.as_boolean();
    return true; // Everything else is truthy in jq
}

Value Evaluator::apply_arithmetic(TokenType op, const Value& left, const Value& right) {
    // String concatenation for +
    if (op == TokenType::Plus) {
        if (left.is_string() && right.is_string()) {
            return Value(left.as_string() + right.as_string());
        }
        if (left.is_array() && right.is_array()) {
            std::vector<Value> result = left.as_array();
            const auto& right_arr = right.as_array();
            result.insert(result.end(), right_arr.begin(), right_arr.end());
            return Value(std::move(result));
        }
    }
    
    // Numeric operations
    if (!left.is_number() || !right.is_number()) {
        throw std::runtime_error("Arithmetic operation on non-numbers");
    }
    
    double l = left.as_number();
    double r = right.as_number();
    
    switch (op) {
        case TokenType::Plus: return Value(l + r);
        case TokenType::Minus: return Value(l - r);
        case TokenType::Star: return Value(l * r);
        case TokenType::Slash:
            if (r == 0.0) throw std::runtime_error("Division by zero");
            return Value(l / r);
        case TokenType::Percent:
            if (r == 0.0) throw std::runtime_error("Modulo by zero");
            return Value(std::fmod(l, r));
        default:
            throw std::runtime_error("Unknown arithmetic operator");
    }
}

bool Evaluator::apply_comparison(TokenType op, const Value& left, const Value& right) {
    int cmp = compare_values(left, right);
    
    switch (op) {
        case TokenType::Equal: return cmp == 0;
        case TokenType::NotEqual: return cmp != 0;
        case TokenType::Less: return cmp < 0;
        case TokenType::LessEqual: return cmp <= 0;
        case TokenType::Greater: return cmp > 0;
        case TokenType::GreaterEqual: return cmp >= 0;
        default:
            throw std::runtime_error("Unknown comparison operator");
    }
}

int Evaluator::compare_values(const Value& a, const Value& b) {
    // Type ordering: null < boolean < number < string < array < object
    auto type_order = [](const Value& v) -> int {
        if (v.is_null()) return 0;
        if (v.is_boolean()) return 1;
        if (v.is_number()) return 2;
        if (v.is_string()) return 3;
        if (v.is_array()) return 4;
        if (v.is_object()) return 5;
        return 6;
    };
    
    int a_type = type_order(a);
    int b_type = type_order(b);
    
    if (a_type != b_type) {
        return a_type - b_type;
    }
    
    // Same type comparison
    if (a.is_null()) return 0;
    if (a.is_boolean()) return a.as_boolean() == b.as_boolean() ? 0 : (a.as_boolean() ? 1 : -1);
    if (a.is_number()) {
        double diff = a.as_number() - b.as_number();
        return diff < 0 ? -1 : (diff > 0 ? 1 : 0);
    }
    if (a.is_string()) return a.as_string().compare(b.as_string());
    
    // Arrays and objects would need recursive comparison
    return 0; // Simplified
}

// Built-in function implementations

std::vector<Value> Evaluator::builtin_select(const std::vector<std::vector<Value>>& args) {
    if (args.size() < 2) throw std::runtime_error("select requires condition");
    
    const Value& data = args[0][0];
    const auto& cond_results = args[1];
    
    if (!cond_results.empty() && is_truthy(cond_results[0])) {
        return {data};
    }
    return {};
}

std::vector<Value> Evaluator::builtin_map(const std::vector<std::vector<Value>>& args) {
    // Simplified - full version would evaluate expression for each element
    throw std::runtime_error("map requires expression evaluation - not yet implemented");
}

std::vector<Value> Evaluator::builtin_length(const std::vector<std::vector<Value>>& args) {
    if (args.empty()) throw std::runtime_error("length requires input");
    
    const Value& val = args[0][0];
    
    if (val.is_array()) {
        return {Value(static_cast<double>(val.as_array().size()))};
    }
    if (val.is_object()) {
        return {Value(static_cast<double>(val.as_object().size()))};
    }
    if (val.is_string()) {
        return {Value(static_cast<double>(val.as_string().length()))};
    }
    if (val.is_null()) {
        return {Value(0.0)};
    }
    
    throw std::runtime_error("length not supported for this type");
}

std::vector<Value> Evaluator::builtin_keys(const std::vector<std::vector<Value>>& args) {
    if (args.empty()) throw std::runtime_error("keys requires input");
    
    const Value& val = args[0][0];
    
    if (val.is_object()) {
        std::vector<Value> keys;
        for (const auto& [key, _] : val.as_object()) {
            keys.push_back(Value(key));
        }
        std::sort(keys.begin(), keys.end(), [this](const Value& a, const Value& b) {
            return compare_values(a, b) < 0;
        });
        return {Value(std::move(keys))};
    }
    
    if (val.is_array()) {
        std::vector<Value> indices;
        for (size_t i = 0; i < val.as_array().size(); ++i) {
            indices.push_back(Value(static_cast<double>(i)));
        }
        return {Value(std::move(indices))};
    }
    
    throw std::runtime_error("keys only works on objects and arrays");
}

std::vector<Value> Evaluator::builtin_values(const std::vector<std::vector<Value>>& args) {
    if (args.empty()) throw std::runtime_error("values requires input");
    
    const Value& val = args[0][0];
    
    if (val.is_object()) {
        std::vector<Value> vals;
        for (const auto& [_, v] : val.as_object()) {
            vals.push_back(v);
        }
        return vals;
    }
    
    if (val.is_array()) {
        return val.as_array();
    }
    
    throw std::runtime_error("values only works on objects and arrays");
}

std::vector<Value> Evaluator::builtin_type(const std::vector<std::vector<Value>>& args) {
    if (args.empty()) throw std::runtime_error("type requires input");
    
    const Value& val = args[0][0];
    
    if (val.is_null()) return {Value("null")};
    if (val.is_boolean()) return {Value("boolean")};
    if (val.is_number()) return {Value("number")};
    if (val.is_string()) return {Value("string")};
    if (val.is_array()) return {Value("array")};
    if (val.is_object()) return {Value("object")};
    
    return {Value("unknown")};
}

std::vector<Value> Evaluator::builtin_add(const std::vector<std::vector<Value>>& args) {
    if (args.empty()) throw std::runtime_error("add requires input");
    
    const Value& val = args[0][0];
    
    if (!val.is_array()) {
        return {val};
    }
    
    const auto& arr = val.as_array();
    if (arr.empty()) {
        return {Value()};
    }
    
    // Detect type from first element
    if (arr[0].is_number()) {
        double sum = 0.0;
        for (const auto& elem : arr) {
            if (elem.is_number()) {
                sum += elem.as_number();
            }
        }
        return {Value(sum)};
    }
    
    if (arr[0].is_string()) {
        std::string result;
        for (const auto& elem : arr) {
            if (elem.is_string()) {
                result += elem.as_string();
            }
        }
        return {Value(result)};
    }
    
    if (arr[0].is_array()) {
        std::vector<Value> result;
        for (const auto& elem : arr) {
            if (elem.is_array()) {
                const auto& sub_arr = elem.as_array();
                result.insert(result.end(), sub_arr.begin(), sub_arr.end());
            }
        }
        return {Value(std::move(result))};
    }
    
    return {Value()};
}

std::vector<Value> Evaluator::builtin_sort(const std::vector<std::vector<Value>>& args) {
    if (args.empty()) throw std::runtime_error("sort requires input");
    
    const Value& val = args[0][0];
    
    if (!val.is_array()) {
        return {val};
    }
    
    std::vector<Value> sorted = val.as_array();
    std::sort(sorted.begin(), sorted.end(), [this](const Value& a, const Value& b) {
        return compare_values(a, b) < 0;
    });
    
    return {Value(std::move(sorted))};
}

std::vector<Value> Evaluator::builtin_reverse(const std::vector<std::vector<Value>>& args) {
    if (args.empty()) throw std::runtime_error("reverse requires input");
    
    const Value& val = args[0][0];
    
    if (!val.is_array()) {
        return {val};
    }
    
    std::vector<Value> reversed = val.as_array();
    std::reverse(reversed.begin(), reversed.end());
    
    return {Value(std::move(reversed))};
}

std::vector<Value> Evaluator::builtin_unique(const std::vector<std::vector<Value>>& args) {
    if (args.empty()) throw std::runtime_error("unique requires input");
    
    const Value& val = args[0][0];
    
    if (!val.is_array()) {
        return {val};
    }
    
    std::vector<Value> unique_vals = val.as_array();
    std::sort(unique_vals.begin(), unique_vals.end(), [this](const Value& a, const Value& b) {
        return compare_values(a, b) < 0;
    });
    
    auto last = std::unique(unique_vals.begin(), unique_vals.end(), [this](const Value& a, const Value& b) {
        return compare_values(a, b) == 0;
    });
    
    unique_vals.erase(last, unique_vals.end());
    
    return {Value(std::move(unique_vals))};
}

std::vector<Value> Evaluator::builtin_min(const std::vector<std::vector<Value>>& args) {
    if (args.empty()) throw std::runtime_error("min requires input");
    
    const Value& val = args[0][0];
    
    if (!val.is_array() || val.as_array().empty()) {
        return {Value()};
    }
    
    const auto& arr = val.as_array();
    Value min_val = arr[0];
    
    for (size_t i = 1; i < arr.size(); ++i) {
        if (compare_values(arr[i], min_val) < 0) {
            min_val = arr[i];
        }
    }
    
    return {min_val};
}

std::vector<Value> Evaluator::builtin_max(const std::vector<std::vector<Value>>& args) {
    if (args.empty()) throw std::runtime_error("max requires input");
    
    const Value& val = args[0][0];
    
    if (!val.is_array() || val.as_array().empty()) {
        return {Value()};
    }
    
    const auto& arr = val.as_array();
    Value max_val = arr[0];
    
    for (size_t i = 1; i < arr.size(); ++i) {
        if (compare_values(arr[i], max_val) > 0) {
            max_val = arr[i];
        }
    }
    
    return {max_val};
}

std::vector<Value> Evaluator::builtin_empty(const std::vector<std::vector<Value>>& args) {
    return {}; // empty returns no results
}

std::vector<Value> Evaluator::builtin_tostring(const std::vector<std::vector<Value>>& args) {
    if (args.empty()) throw std::runtime_error("tostring requires input");
    
    const Value& val = args[0][0];
    
    if (val.is_string()) {
        return {val};
    }
    if (val.is_number()) {
        return {Value(std::to_string(val.as_number()))};
    }
    if (val.is_boolean()) {
        return {Value(val.as_boolean() ? "true" : "false")};
    }
    if (val.is_null()) {
        return {Value("null")};
    }
    
    // For complex types, use TOON serialization
    return {Value(val.to_toon())};
}

std::vector<Value> Evaluator::builtin_tonumber(const std::vector<std::vector<Value>>& args) {
    if (args.empty()) throw std::runtime_error("tonumber requires input");
    
    const Value& val = args[0][0];
    
    if (val.is_number()) {
        return {val};
    }
    if (val.is_string()) {
        try {
            return {Value(std::stod(val.as_string()))};
        } catch (...) {
            throw std::runtime_error("Cannot convert string to number");
        }
    }
    
    throw std::runtime_error("Cannot convert to number");
}

std::vector<Value> Evaluator::builtin_floor(const std::vector<std::vector<Value>>& args) {
    if (args.empty()) throw std::runtime_error("floor requires input");
    
    const Value& val = args[0][0];
    
    if (!val.is_number()) throw std::runtime_error("floor requires number");
    
    return {Value(std::floor(val.as_number()))};
}

std::vector<Value> Evaluator::builtin_sqrt(const std::vector<std::vector<Value>>& args) {
    if (args.empty()) throw std::runtime_error("sqrt requires input");
    
    const Value& val = args[0][0];
    
    if (!val.is_number()) throw std::runtime_error("sqrt requires number");
    
    return {Value(std::sqrt(val.as_number()))};
}

// Placeholder implementations for remaining functions
std::vector<Value> Evaluator::builtin_group_by(const std::vector<std::vector<Value>>& args) {
    // Placeholder for expression-based function
    // group_by requires evaluating an expression on each element
    // This will be enhanced when we support expression parameters
    throw std::runtime_error("group_by not yet implemented - requires expression parameter support");
}

std::vector<Value> Evaluator::builtin_has(const std::vector<std::vector<Value>>& args) {
    if (args.size() < 2 || args[0].empty() || args[1].empty()) {
        return {Value(false)};
    }
    
    const auto& container = args[0][0];
    const auto& key = args[1][0];
    
    if (container.is_object() && key.is_string()) {
        return {Value(container.as_object().find(key.as_string()) != container.as_object().end())};
    } else if (container.is_array() && key.is_number()) {
        int idx = static_cast<int>(key.as_number());
        if (idx < 0) idx = static_cast<int>(container.as_array().size()) + idx;
        return {Value(idx >= 0 && idx < static_cast<int>(container.as_array().size()))};
    }
    
    return {Value(false)};
}

std::vector<Value> Evaluator::builtin_to_entries(const std::vector<std::vector<Value>>& args) {
    if (args.empty() || args[0].empty()) {
        return {Value()};
    }
    
    const auto& val = args[0][0];
    std::vector<Value> entries;
    
    if (val.is_object()) {
        for (const auto& [k, v] : val.as_object()) {
            std::map<std::string, Value> entry;
            entry["key"] = Value(k);
            entry["value"] = v;
            entries.push_back(Value(entry));
        }
    } else if (val.is_array()) {
        const auto& arr = val.as_array();
        for (size_t i = 0; i < arr.size(); i++) {
            std::map<std::string, Value> entry;
            entry["key"] = Value(static_cast<double>(i));
            entry["value"] = arr[i];
            entries.push_back(Value(entry));
        }
    }
    
    return {Value(entries)};
}

std::vector<Value> Evaluator::builtin_from_entries(const std::vector<std::vector<Value>>& args) {
    if (args.empty() || args[0].empty()) {
        return {Value()};
    }
    
    const auto& val = args[0][0];
    if (!val.is_array()) {
        throw std::runtime_error("from_entries requires array");
    }
    
    std::map<std::string, Value> result;
    
    for (const auto& entry : val.as_array()) {
        if (entry.is_object()) {
            const auto& obj = entry.as_object();
            if (obj.find("key") != obj.end() && obj.find("value") != obj.end()) {
                const auto& key = obj.at("key");
                const auto& value = obj.at("value");
                
                if (key.is_string()) {
                    result[key.as_string()] = value;
                } else if (key.is_number()) {
                    result[std::to_string(static_cast<int>(key.as_number()))] = value;
                }
            }
        }
    }
    
    return {Value(result)};
}

std::vector<Value> Evaluator::builtin_split(const std::vector<std::vector<Value>>& args) {
    if (args.size() < 2 || args[0].empty() || args[1].empty()) {
        return {Value()};
    }
    
    const auto& str_val = args[0][0];
    const auto& sep_val = args[1][0];
    
    if (!str_val.is_string() || !sep_val.is_string()) {
        throw std::runtime_error("split requires string arguments");
    }
    
    std::string str = str_val.as_string();
    std::string sep = sep_val.as_string();
    
    std::vector<Value> parts;
    size_t start = 0;
    size_t end = str.find(sep);
    
    while (end != std::string::npos) {
        parts.push_back(Value(str.substr(start, end - start)));
        start = end + sep.length();
        end = str.find(sep, start);
    }
    parts.push_back(Value(str.substr(start)));
    
    return {Value(parts)};
}

std::vector<Value> Evaluator::builtin_join(const std::vector<std::vector<Value>>& args) {
    if (args.size() < 2 || args[0].empty() || args[1].empty()) {
        return {Value()};
    }
    
    const auto& arr_val = args[0][0];
    const auto& sep_val = args[1][0];
    
    if (!arr_val.is_array() || !sep_val.is_string()) {
        throw std::runtime_error("join requires array and separator");
    }
    
    std::string sep = sep_val.as_string();
    std::string result;
    
    const auto& arr = arr_val.as_array();
    for (size_t i = 0; i < arr.size(); i++) {
        if (arr[i].is_string()) {
            result += arr[i].as_string();
        } else {
            result += arr[i].to_toon();
        }
        if (i < arr.size() - 1) {
            result += sep;
        }
    }
    
    return {Value(result)};
}

std::vector<Value> Evaluator::builtin_startswith(const std::vector<std::vector<Value>>& args) {
    if (args.size() < 2 || args[0].empty() || args[1].empty()) {
        return {Value(false)};
    }
    
    const auto& str_val = args[0][0];
    const auto& prefix_val = args[1][0];
    
    if (!str_val.is_string() || !prefix_val.is_string()) {
        return {Value(false)};
    }
    
    return {Value(str_val.as_string().find(prefix_val.as_string()) == 0)};
}

std::vector<Value> Evaluator::builtin_endswith(const std::vector<std::vector<Value>>& args) {
    if (args.size() < 2 || args[0].empty() || args[1].empty()) {
        return {Value(false)};
    }
    
    const auto& str_val = args[0][0];
    const auto& suffix_val = args[1][0];
    
    if (!str_val.is_string() || !suffix_val.is_string()) {
        return {Value(false)};
    }
    
    const std::string& str = str_val.as_string();
    const std::string& suffix = suffix_val.as_string();
    
    if (suffix.length() > str.length()) {
        return {Value(false)};
    }
    
    return {Value(str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0)};
}

std::vector<Value> Evaluator::builtin_ceil(const std::vector<std::vector<Value>>& args) {
    if (args.empty() || args[0].empty()) {
        return {Value()};
    }
    
    const auto& val = args[0][0];
    if (!val.is_number()) {
        throw std::runtime_error("ceil requires number");
    }
    
    return {Value(std::ceil(val.as_number()))};
}

std::vector<Value> Evaluator::builtin_round(const std::vector<std::vector<Value>>& args) {
    if (args.empty() || args[0].empty()) {
        return {Value()};
    }
    
    const auto& val = args[0][0];
    if (!val.is_number()) {
        throw std::runtime_error("round requires number");
    }
    
    return {Value(std::round(val.as_number()))};
}

// ============= EXPRESSION-BASED BUILT-INS =============

std::vector<Value> Evaluator::builtin_map(const ExprPtr& expr, const Value& data) {
    if (!data.is_array()) {
        throw std::runtime_error("map can only be applied to arrays");
    }
    
    std::vector<Value> result_arr;
    for (const auto& elem : data.as_array()) {
        auto mapped_results = eval(expr, elem);
        // map collects all results into an array
        for (const auto& val : mapped_results) {
            result_arr.push_back(val);
        }
    }
    
    return {Value(std::move(result_arr))};
}

std::vector<Value> Evaluator::builtin_select(const ExprPtr& expr, const Value& data) {
    // select filters: if expression evaluates to truthy, return data
    auto results = eval(expr, data);
    
    if (!results.empty() && is_truthy(results[0])) {
        return {data};
    }
    
    return {};  // empty if condition is false
}

std::vector<Value> Evaluator::builtin_sort_by(const ExprPtr& expr, const Value& data) {
    if (!data.is_array()) {
        throw std::runtime_error("sort_by can only be applied to arrays");
    }
    
    auto arr = data.as_array();
    
    // Sort using key extraction with the expression
    std::sort(arr.begin(), arr.end(), [this, expr](const Value& a, const Value& b) {
        auto a_keys = eval(expr, a);
        auto b_keys = eval(expr, b);
        
        if (a_keys.empty() || b_keys.empty()) {
            return false;
        }
        
        return compare_values(a_keys[0], b_keys[0]) < 0;
    });
    
    return {Value(std::move(arr))};
}

std::vector<Value> Evaluator::builtin_unique_by(const ExprPtr& expr, const Value& data) {
    if (!data.is_array()) {
        throw std::runtime_error("unique_by can only be applied to arrays");
    }
    
    auto arr = data.as_array();
    
    // First sort by the expression
    std::sort(arr.begin(), arr.end(), [this, expr](const Value& a, const Value& b) {
        auto a_keys = eval(expr, a);
        auto b_keys = eval(expr, b);
        
        if (a_keys.empty() || b_keys.empty()) {
            return false;
        }
        
        return compare_values(a_keys[0], b_keys[0]) < 0;
    });
    
    // Then remove duplicates
    auto last = std::unique(arr.begin(), arr.end(), [this, expr](const Value& a, const Value& b) {
        auto a_keys = eval(expr, a);
        auto b_keys = eval(expr, b);
        
        if (a_keys.empty() || b_keys.empty()) {
            return false;
        }
        
        return compare_values(a_keys[0], b_keys[0]) == 0;
    });
    
    arr.erase(last, arr.end());
    return {Value(std::move(arr))};
}

std::vector<Value> Evaluator::builtin_group_by(const ExprPtr& expr, const Value& data) {
    if (!data.is_array()) {
        throw std::runtime_error("group_by can only be applied to arrays");
    }
    
    // Create map of key -> elements
    std::map<std::string, std::vector<Value>> groups;
    
    for (const auto& elem : data.as_array()) {
        auto key_results = eval(expr, elem);
        if (!key_results.empty()) {
            // Convert key to string for grouping
            std::string key = key_results[0].to_toon();
            groups[key].push_back(elem);
        }
    }
    
    // Convert groups to array of arrays
    std::vector<Value> result_arr;
    for (const auto& [key, group] : groups) {
        result_arr.push_back(Value(group));
    }
    
    return {Value(std::move(result_arr))};
}

std::vector<Value> Evaluator::builtin_min_by(const ExprPtr& expr, const Value& data) {
    if (!data.is_array() || data.as_array().empty()) {
        return {Value()};
    }
    
    const auto& arr = data.as_array();
    const Value* min_elem = &arr[0];
    Value min_key = eval(expr, arr[0])[0];
    
    for (size_t i = 1; i < arr.size(); ++i) {
        auto key_results = eval(expr, arr[i]);
        if (!key_results.empty()) {
            if (compare_values(key_results[0], min_key) < 0) {
                min_key = key_results[0];
                min_elem = &arr[i];
            }
        }
    }
    
    return {*min_elem};
}

std::vector<Value> Evaluator::builtin_max_by(const ExprPtr& expr, const Value& data) {
    if (!data.is_array() || data.as_array().empty()) {
        return {Value()};
    }
    
    const auto& arr = data.as_array();
    const Value* max_elem = &arr[0];
    Value max_key = eval(expr, arr[0])[0];
    
    for (size_t i = 1; i < arr.size(); ++i) {
        auto key_results = eval(expr, arr[i]);
        if (!key_results.empty()) {
            if (compare_values(key_results[0], max_key) > 0) {
                max_key = key_results[0];
                max_elem = &arr[i];
            }
        }
    }
    
    return {*max_elem};
}

std::vector<Value> Evaluator::builtin_any(const ExprPtr& expr, const Value& data) {
    if (!data.is_array()) {
        throw std::runtime_error("any can only be applied to arrays");
    }
    
    for (const auto& elem : data.as_array()) {
        auto results = eval(expr, elem);
        if (!results.empty() && is_truthy(results[0])) {
            return {Value(true)};
        }
    }
    
    return {Value(false)};
}

std::vector<Value> Evaluator::builtin_all(const ExprPtr& expr, const Value& data) {
    if (!data.is_array()) {
        throw std::runtime_error("all can only be applied to arrays");
    }
    
    for (const auto& elem : data.as_array()) {
        auto results = eval(expr, elem);
        if (results.empty() || !is_truthy(results[0])) {
            return {Value(false)};
        }
    }
    
    return {Value(true)};
}

// ============= MATH FUNCTIONS =============

std::vector<Value> Evaluator::builtin_abs(const std::vector<std::vector<Value>>& args) {
    if (args.empty() || args[0].empty()) {
        return {Value()};
    }
    
    const auto& val = args[0][0];
    if (!val.is_number()) {
        throw std::runtime_error("abs requires a number");
    }
    
    return {Value(std::abs(val.as_number()))};
}

std::vector<Value> Evaluator::builtin_pow(const std::vector<std::vector<Value>>& args) {
    if (args.size() < 2 || args[0].empty() || args[1].empty()) {
        return {Value()};
    }
    
    const auto& base_val = args[0][0];
    const auto& exp_val = args[1][0];
    
    if (!base_val.is_number() || !exp_val.is_number()) {
        throw std::runtime_error("pow requires numbers");
    }
    
    return {Value(std::pow(base_val.as_number(), exp_val.as_number()))};
}

std::vector<Value> Evaluator::builtin_log(const std::vector<std::vector<Value>>& args) {
    if (args.empty() || args[0].empty()) {
        return {Value()};
    }
    
    const auto& val = args[0][0];
    if (!val.is_number()) {
        throw std::runtime_error("log requires a number");
    }
    
    double num = val.as_number();
    if (num <= 0) {
        throw std::runtime_error("log requires positive number");
    }
    
    return {Value(std::log(num))};
}

std::vector<Value> Evaluator::builtin_log10(const std::vector<std::vector<Value>>& args) {
    if (args.empty() || args[0].empty()) {
        return {Value()};
    }
    
    const auto& val = args[0][0];
    if (!val.is_number()) {
        throw std::runtime_error("log10 requires a number");
    }
    
    double num = val.as_number();
    if (num <= 0) {
        throw std::runtime_error("log10 requires positive number");
    }
    
    return {Value(std::log10(num))};
}

std::vector<Value> Evaluator::builtin_log2(const std::vector<std::vector<Value>>& args) {
    if (args.empty() || args[0].empty()) {
        return {Value()};
    }
    
    const auto& val = args[0][0];
    if (!val.is_number()) {
        throw std::runtime_error("log2 requires a number");
    }
    
    double num = val.as_number();
    if (num <= 0) {
        throw std::runtime_error("log2 requires positive number");
    }
    
    return {Value(std::log2(num))};
}

std::vector<Value> Evaluator::builtin_exp(const std::vector<std::vector<Value>>& args) {
    if (args.empty() || args[0].empty()) {
        return {Value()};
    }
    
    const auto& val = args[0][0];
    if (!val.is_number()) {
        throw std::runtime_error("exp requires a number");
    }
    
    return {Value(std::exp(val.as_number()))};
}

std::vector<Value> Evaluator::builtin_exp10(const std::vector<std::vector<Value>>& args) {
    if (args.empty() || args[0].empty()) {
        return {Value()};
    }
    
    const auto& val = args[0][0];
    if (!val.is_number()) {
        throw std::runtime_error("exp10 requires a number");
    }
    
    return {Value(std::pow(10.0, val.as_number()))};
}

std::vector<Value> Evaluator::builtin_exp2(const std::vector<std::vector<Value>>& args) {
    if (args.empty() || args[0].empty()) {
        return {Value()};
    }
    
    const auto& val = args[0][0];
    if (!val.is_number()) {
        throw std::runtime_error("exp2 requires a number");
    }
    
    return {Value(std::exp2(val.as_number()))};
}

std::vector<Value> Evaluator::builtin_sin(const std::vector<std::vector<Value>>& args) {
    if (args.empty() || args[0].empty()) {
        return {Value()};
    }
    
    const auto& val = args[0][0];
    if (!val.is_number()) {
        throw std::runtime_error("sin requires a number");
    }
    
    return {Value(std::sin(val.as_number()))};
}

std::vector<Value> Evaluator::builtin_cos(const std::vector<std::vector<Value>>& args) {
    if (args.empty() || args[0].empty()) {
        return {Value()};
    }
    
    const auto& val = args[0][0];
    if (!val.is_number()) {
        throw std::runtime_error("cos requires a number");
    }
    
    return {Value(std::cos(val.as_number()))};
}

std::vector<Value> Evaluator::builtin_tan(const std::vector<std::vector<Value>>& args) {
    if (args.empty() || args[0].empty()) {
        return {Value()};
    }
    
    const auto& val = args[0][0];
    if (!val.is_number()) {
        throw std::runtime_error("tan requires a number");
    }
    
    return {Value(std::tan(val.as_number()))};
}

std::vector<Value> Evaluator::builtin_asin(const std::vector<std::vector<Value>>& args) {
    if (args.empty() || args[0].empty()) {
        return {Value()};
    }
    
    const auto& val = args[0][0];
    if (!val.is_number()) {
        throw std::runtime_error("asin requires a number");
    }
    
    double num = val.as_number();
    if (num < -1.0 || num > 1.0) {
        throw std::runtime_error("asin requires number in range [-1, 1]");
    }
    
    return {Value(std::asin(num))};
}

std::vector<Value> Evaluator::builtin_acos(const std::vector<std::vector<Value>>& args) {
    if (args.empty() || args[0].empty()) {
        return {Value()};
    }
    
    const auto& val = args[0][0];
    if (!val.is_number()) {
        throw std::runtime_error("acos requires a number");
    }
    
    double num = val.as_number();
    if (num < -1.0 || num > 1.0) {
        throw std::runtime_error("acos requires number in range [-1, 1]");
    }
    
    return {Value(std::acos(num))};
}

std::vector<Value> Evaluator::builtin_atan(const std::vector<std::vector<Value>>& args) {
    if (args.empty() || args[0].empty()) {
        return {Value()};
    }
    
    const auto& val = args[0][0];
    if (!val.is_number()) {
        throw std::runtime_error("atan requires a number");
    }
    
    return {Value(std::atan(val.as_number()))};
}

// ============= STRING FUNCTIONS =============

std::vector<Value> Evaluator::builtin_ltrimstr(const std::vector<std::vector<Value>>& args) {
    if (args.size() < 2 || args[0].empty() || args[1].empty()) {
        return {Value()};
    }
    
    const auto& str_val = args[0][0];
    const auto& prefix_val = args[1][0];
    
    if (!str_val.is_string() || !prefix_val.is_string()) {
        throw std::runtime_error("ltrimstr requires strings");
    }
    
    std::string str = str_val.as_string();
    std::string prefix = prefix_val.as_string();
    
    if (str.find(prefix) == 0) {
        return {Value(str.substr(prefix.length()))};
    }
    
    return {Value(str)};
}

std::vector<Value> Evaluator::builtin_rtrimstr(const std::vector<std::vector<Value>>& args) {
    if (args.size() < 2 || args[0].empty() || args[1].empty()) {
        return {Value()};
    }
    
    const auto& str_val = args[0][0];
    const auto& suffix_val = args[1][0];
    
    if (!str_val.is_string() || !suffix_val.is_string()) {
        throw std::runtime_error("rtrimstr requires strings");
    }
    
    std::string str = str_val.as_string();
    std::string suffix = suffix_val.as_string();
    
    if (str.length() >= suffix.length() && 
        str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0) {
        return {Value(str.substr(0, str.length() - suffix.length()))};
    }
    
    return {Value(str)};
}

std::vector<Value> Evaluator::builtin_ascii_downcase(const std::vector<std::vector<Value>>& args) {
    if (args.empty() || args[0].empty()) {
        return {Value()};
    }
    
    const auto& val = args[0][0];
    if (!val.is_string()) {
        throw std::runtime_error("ascii_downcase requires a string");
    }
    
    std::string str = val.as_string();
    for (char& c : str) {
        if (c >= 'A' && c <= 'Z') {
            c = c - 'A' + 'a';
        }
    }
    
    return {Value(str)};
}

std::vector<Value> Evaluator::builtin_ascii_upcase(const std::vector<std::vector<Value>>& args) {
    if (args.empty() || args[0].empty()) {
        return {Value()};
    }
    
    const auto& val = args[0][0];
    if (!val.is_string()) {
        throw std::runtime_error("ascii_upcase requires a string");
    }
    
    std::string str = val.as_string();
    for (char& c : str) {
        if (c >= 'a' && c <= 'z') {
            c = c - 'a' + 'A';
        }
    }
    
    return {Value(str)};
}

std::vector<Value> Evaluator::builtin_contains(const std::vector<std::vector<Value>>& args) {
    if (args.size() < 2 || args[0].empty() || args[1].empty()) {
        return {Value()};
    }
    
    const auto& haystack = args[0][0];
    const auto& needle = args[1][0];
    
    // String contains substring
    if (haystack.is_string() && needle.is_string()) {
        return {Value(haystack.as_string().find(needle.as_string()) != std::string::npos)};
    }
    
    // Array contains element
    if (haystack.is_array()) {
        for (const auto& elem : haystack.as_array()) {
            if (compare_values(elem, needle) == 0) {
                return {Value(true)};
            }
        }
        return {Value(false)};
    }
    
    // Object contains key
    if (haystack.is_object()) {
        if (needle.is_string()) {
            return {Value(haystack.as_object().find(needle.as_string()) != haystack.as_object().end())};
        }
    }
    
    return {Value(false)};
}

std::vector<Value> Evaluator::builtin_index(const std::vector<std::vector<Value>>& args) {
    if (args.size() < 2 || args[0].empty() || args[1].empty()) {
        return {Value()};
    }
    
    const auto& haystack = args[0][0];
    const auto& needle = args[1][0];
    
    // String index of substring
    if (haystack.is_string() && needle.is_string()) {
        size_t pos = haystack.as_string().find(needle.as_string());
        if (pos != std::string::npos) {
            return {Value(static_cast<double>(pos))};
        }
        return {Value()};
    }
    
    // Array index of element
    if (haystack.is_array()) {
        const auto& arr = haystack.as_array();
        for (size_t i = 0; i < arr.size(); i++) {
            if (compare_values(arr[i], needle) == 0) {
                return {Value(static_cast<double>(i))};
            }
        }
        return {Value()};
    }
    
    return {Value()};
}

std::vector<Value> Evaluator::builtin_rindex(const std::vector<std::vector<Value>>& args) {
    if (args.size() < 2 || args[0].empty() || args[1].empty()) {
        return {Value()};
    }
    
    const auto& haystack = args[0][0];
    const auto& needle = args[1][0];
    
    // String rindex of substring (last occurrence)
    if (haystack.is_string() && needle.is_string()) {
        size_t pos = haystack.as_string().rfind(needle.as_string());
        if (pos != std::string::npos) {
            return {Value(static_cast<double>(pos))};
        }
        return {Value()};
    }
    
    // Array rindex of element (last occurrence)
    if (haystack.is_array()) {
        const auto& arr = haystack.as_array();
        for (int i = static_cast<int>(arr.size()) - 1; i >= 0; i--) {
            if (compare_values(arr[i], needle) == 0) {
                return {Value(static_cast<double>(i))};
            }
        }
        return {Value()};
    }
    
    return {Value()};
}

std::vector<Value> Evaluator::builtin_inside(const std::vector<std::vector<Value>>& args) {
    if (args.size() < 2 || args[0].empty() || args[1].empty()) {
        return {Value()};
    }
    
    const auto& needle = args[0][0];
    const auto& haystack = args[1][0];
    
    // Reverse of contains: check if needle is inside haystack
    // String inside string
    if (needle.is_string() && haystack.is_string()) {
        return {Value(haystack.as_string().find(needle.as_string()) != std::string::npos)};
    }
    
    // Element inside array
    if (haystack.is_array()) {
        for (const auto& elem : haystack.as_array()) {
            if (compare_values(elem, needle) == 0) {
                return {Value(true)};
            }
        }
        return {Value(false)};
    }
    
    // Key inside object
    if (haystack.is_object()) {
        if (needle.is_string()) {
            return {Value(haystack.as_object().find(needle.as_string()) != haystack.as_object().end())};
        }
    }
    
    return {Value(false)};
}

std::vector<Value> Evaluator::builtin_indices(const std::vector<std::vector<Value>>& args) {
    if (args.size() < 2 || args[0].empty() || args[1].empty()) {
        return {Value()};
    }
    
    const auto& haystack = args[0][0];
    const auto& needle = args[1][0];
    
    std::vector<Value> indices;
    
    // String indices of all substring occurrences
    if (haystack.is_string() && needle.is_string()) {
        const std::string& str = haystack.as_string();
        const std::string& substr = needle.as_string();
        
        if (substr.empty()) {
            // All positions for empty string
            for (size_t i = 0; i <= str.length(); i++) {
                indices.push_back(Value(static_cast<double>(i)));
            }
        } else {
            size_t pos = 0;
            while ((pos = str.find(substr, pos)) != std::string::npos) {
                indices.push_back(Value(static_cast<double>(pos)));
                pos += substr.length();
            }
        }
    }
    // Array indices of all element occurrences
    else if (haystack.is_array()) {
        const auto& arr = haystack.as_array();
        for (size_t i = 0; i < arr.size(); i++) {
            if (compare_values(arr[i], needle) == 0) {
                indices.push_back(Value(static_cast<double>(i)));
            }
        }
    }
    
    return {Value(indices)};
}

// ============= ARRAY FUNCTIONS =============

std::vector<Value> Evaluator::builtin_first(const std::vector<std::vector<Value>>& args) {
    if (args.empty() || args[0].empty()) {
        return {};
    }
    
    const auto& val = args[0][0];
    
    // first(array) returns first element
    if (val.is_array()) {
        const auto& arr = val.as_array();
        if (!arr.empty()) {
            return {arr[0]};
        }
        return {};
    }
    
    // first on non-array returns the value itself (like iterator)
    return {val};
}

std::vector<Value> Evaluator::builtin_last(const std::vector<std::vector<Value>>& args) {
    if (args.empty() || args[0].empty()) {
        return {};
    }
    
    const auto& val = args[0][0];
    
    // last(array) returns last element
    if (val.is_array()) {
        const auto& arr = val.as_array();
        if (!arr.empty()) {
            return {arr.back()};
        }
        return {};
    }
    
    // last on non-array returns the value itself
    return {val};
}

std::vector<Value> Evaluator::builtin_nth(const std::vector<std::vector<Value>>& args) {
    if (args.size() < 2 || args[0].empty() || args[1].empty()) {
        return {};
    }
    
    const auto& val = args[0][0];
    const auto& n_val = args[1][0];
    
    if (!n_val.is_number()) {
        throw std::runtime_error("nth requires numeric index");
    }
    
    int n = static_cast<int>(n_val.as_number());
    
    if (val.is_array()) {
        const auto& arr = val.as_array();
        // Handle negative indices
        if (n < 0) {
            n = static_cast<int>(arr.size()) + n;
        }
        
        if (n >= 0 && n < static_cast<int>(arr.size())) {
            return {arr[n]};
        }
        return {};
    }
    
    // nth on non-array returns empty
    return {};
}

std::vector<Value> Evaluator::builtin_range(const std::vector<std::vector<Value>>& args) {
    std::vector<Value> result;
    
    if (args.empty()) {
        return {Value()};
    }
    
    // range(n) generates 0 to n-1
    if (args.size() == 1 && !args[0].empty()) {
        const auto& n_val = args[0][0];
        if (!n_val.is_number()) {
            throw std::runtime_error("range requires numeric arguments");
        }
        
        int n = static_cast<int>(n_val.as_number());
        for (int i = 0; i < n; i++) {
            result.push_back(Value(static_cast<double>(i)));
        }
        return {Value(result)};
    }
    
    // range(from; to) generates from to to-1
    if (args.size() >= 2 && !args[0].empty() && !args[1].empty()) {
        const auto& from_val = args[0][0];
        const auto& to_val = args[1][0];
        
        if (!from_val.is_number() || !to_val.is_number()) {
            throw std::runtime_error("range requires numeric arguments");
        }
        
        int from = static_cast<int>(from_val.as_number());
        int to = static_cast<int>(to_val.as_number());
        
        if (from <= to) {
            for (int i = from; i < to; i++) {
                result.push_back(Value(static_cast<double>(i)));
            }
        } else {
            for (int i = from; i > to; i--) {
                result.push_back(Value(static_cast<double>(i)));
            }
        }
        return {Value(result)};
    }
    
    return {Value(result)};
}

std::vector<Value> Evaluator::builtin_flatten(const std::vector<std::vector<Value>>& args) {
    if (args.empty() || args[0].empty()) {
        return {Value()};
    }
    
    const auto& val = args[0][0];
    if (!val.is_array()) {
        throw std::runtime_error("flatten requires array");
    }
    
    // Get depth (optional second argument, default 1)
    int depth = 1;
    if (args.size() >= 2 && !args[1].empty() && args[1][0].is_number()) {
        depth = static_cast<int>(args[1][0].as_number());
    }
    
    std::function<std::vector<Value>(const Value&, int)> flatten_recursive;
    flatten_recursive = [&](const Value& v, int d) -> std::vector<Value> {
        if (d <= 0 || !v.is_array()) {
            return {v};
        }
        
        std::vector<Value> result;
        for (const auto& elem : v.as_array()) {
            if (elem.is_array() && d > 0) {
                auto flattened = flatten_recursive(elem, d - 1);
                result.insert(result.end(), flattened.begin(), flattened.end());
            } else {
                result.push_back(elem);
            }
        }
        return result;
    };
    
    auto flattened = flatten_recursive(val, depth);
    return {Value(flattened)};
}

std::vector<Value> Evaluator::builtin_transpose(const std::vector<std::vector<Value>>& args) {
    if (args.empty() || args[0].empty()) {
        return {Value()};
    }
    
    const auto& val = args[0][0];
    if (!val.is_array()) {
        throw std::runtime_error("transpose requires array");
    }
    
    const auto& arr = val.as_array();
    if (arr.empty()) {
        return {Value(std::vector<Value>())};
    }
    
    // Find max inner array size
    size_t max_size = 0;
    for (const auto& elem : arr) {
        if (elem.is_array()) {
            max_size = std::max(max_size, elem.as_array().size());
        }
    }
    
    // Create transposed array
    std::vector<Value> result;
    for (size_t i = 0; i < max_size; i++) {
        std::vector<Value> row;
        for (const auto& elem : arr) {
            if (elem.is_array()) {
                const auto& inner = elem.as_array();
                if (i < inner.size()) {
                    row.push_back(inner[i]);
                } else {
                    row.push_back(Value());
                }
            } else {
                row.push_back(Value());
            }
        }
        result.push_back(Value(row));
    }
    
    return {Value(result)};
}

// ============= UTILITY FUNCTIONS =============

std::vector<Value> Evaluator::builtin_error(const std::vector<std::vector<Value>>& args) {
    std::string msg = "error";
    
    if (!args.empty() && !args[0].empty()) {
        const auto& val = args[0][0];
        if (val.is_string()) {
            msg = val.as_string();
        } else {
            msg = val.to_toon();
        }
    }
    
    throw std::runtime_error(msg);
}

std::vector<Value> Evaluator::builtin_debug(const std::vector<std::vector<Value>>& args) {
    // debug prints to stderr and returns the input unchanged
    if (!args.empty() && !args[0].empty()) {
        const auto& val = args[0][0];
        // In a real implementation, this would print to stderr
        // For now, we just return the value
        return {val};
    }
    
    return {Value()};
}

std::vector<Value> Evaluator::builtin_not(const std::vector<std::vector<Value>>& args) {
    if (args.empty() || args[0].empty()) {
        return {Value(true)};
    }
    
    const auto& val = args[0][0];
    return {Value(!is_truthy(val))};
}

} // namespace tq
