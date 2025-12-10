#include "tq/new_evaluator.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace tq {

NewEvaluator::NewEvaluator() {
    register_builtins();
}

void NewEvaluator::register_builtins() {
    // Core built-ins that will be implemented
    builtins_["select"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_select(args); };
    builtins_["map"] = [this](const std::vector<std::vector<Value>>& args) { return builtin_map(args); };
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
}

// Main evaluation dispatcher
std::vector<Value> NewEvaluator::eval(const ExprPtr& expr, const Value& data) {
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

std::vector<Value> NewEvaluator::eval_identity(const Value& data) {
    return {data};
}

std::vector<Value> NewEvaluator::eval_field(const ExprPtr& expr, const Value& data) {
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

std::vector<Value> NewEvaluator::eval_index(const ExprPtr& expr, const Value& data) {
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

std::vector<Value> NewEvaluator::eval_slice(const ExprPtr& expr, const Value& data) {
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

std::vector<Value> NewEvaluator::eval_iterator(const Value& data) {
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

std::vector<Value> NewEvaluator::eval_recursive_descent(const Value& data) {
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

std::vector<Value> NewEvaluator::eval_pipe(const ExprPtr& expr, const Value& data) {
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

std::vector<Value> NewEvaluator::eval_comma(const ExprPtr& expr, const Value& data) {
    // Comma produces multiple outputs
    std::vector<Value> results;
    
    std::vector<Value> left_results = eval(expr->left, data);
    results.insert(results.end(), left_results.begin(), left_results.end());
    
    std::vector<Value> right_results = eval(expr->right, data);
    results.insert(results.end(), right_results.begin(), right_results.end());
    
    return results;
}

std::vector<Value> NewEvaluator::eval_binary_op(const ExprPtr& expr, const Value& data) {
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

std::vector<Value> NewEvaluator::eval_unary_op(const ExprPtr& expr, const Value& data) {
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

std::vector<Value> NewEvaluator::eval_if(const ExprPtr& expr, const Value& data) {
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

std::vector<Value> NewEvaluator::eval_try(const ExprPtr& expr, const Value& data) {
    try {
        return eval(expr->left, data);
    } catch (...) {
        if (expr->right) {
            return eval(expr->right, data);
        }
        return {};
    }
}

std::vector<Value> NewEvaluator::eval_function_call(const ExprPtr& expr, const Value& data) {
    auto it = builtins_.find(expr->func_name);
    if (it == builtins_.end()) {
        throw std::runtime_error("Unknown function: " + expr->func_name);
    }
    
    // Evaluate arguments
    std::vector<std::vector<Value>> arg_results;
    for (const auto& arg : expr->args) {
        arg_results.push_back(eval(arg, data));
    }
    
    // Add data as implicit first argument for most functions
    if (expr->func_name != "empty") {
        arg_results.insert(arg_results.begin(), {data});
    }
    
    return it->second(arg_results);
}

std::vector<Value> NewEvaluator::eval_array_literal(const ExprPtr& expr, const Value& data) {
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

std::vector<Value> NewEvaluator::eval_object_literal(const ExprPtr& expr, const Value& data) {
    std::map<std::string, Value> result_obj;
    
    for (const auto& [key, val_expr] : expr->object_fields) {
        std::vector<Value> val_results = eval(val_expr, data);
        if (!val_results.empty()) {
            result_obj[key] = val_results[0];
        }
    }
    
    return {Value(std::move(result_obj))};
}

std::vector<Value> NewEvaluator::eval_reduce(const ExprPtr& expr, const Value& data) {
    // Simplified reduce - full implementation would handle variables properly
    throw std::runtime_error("reduce not yet implemented");
}

std::vector<Value> NewEvaluator::eval_foreach(const ExprPtr& expr, const Value& data) {
    throw std::runtime_error("foreach not yet implemented");
}

// Helper functions

bool NewEvaluator::is_truthy(const Value& val) {
    if (val.is_null()) return false;
    if (val.is_boolean()) return val.as_boolean();
    return true; // Everything else is truthy in jq
}

Value NewEvaluator::apply_arithmetic(TokenType op, const Value& left, const Value& right) {
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

bool NewEvaluator::apply_comparison(TokenType op, const Value& left, const Value& right) {
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

int NewEvaluator::compare_values(const Value& a, const Value& b) {
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

std::vector<Value> NewEvaluator::builtin_select(const std::vector<std::vector<Value>>& args) {
    if (args.size() < 2) throw std::runtime_error("select requires condition");
    
    const Value& data = args[0][0];
    const auto& cond_results = args[1];
    
    if (!cond_results.empty() && is_truthy(cond_results[0])) {
        return {data};
    }
    return {};
}

std::vector<Value> NewEvaluator::builtin_map(const std::vector<std::vector<Value>>& args) {
    // Simplified - full version would evaluate expression for each element
    throw std::runtime_error("map requires expression evaluation - not yet implemented");
}

std::vector<Value> NewEvaluator::builtin_length(const std::vector<std::vector<Value>>& args) {
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

std::vector<Value> NewEvaluator::builtin_keys(const std::vector<std::vector<Value>>& args) {
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

std::vector<Value> NewEvaluator::builtin_values(const std::vector<std::vector<Value>>& args) {
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

std::vector<Value> NewEvaluator::builtin_type(const std::vector<std::vector<Value>>& args) {
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

std::vector<Value> NewEvaluator::builtin_add(const std::vector<std::vector<Value>>& args) {
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

std::vector<Value> NewEvaluator::builtin_sort(const std::vector<std::vector<Value>>& args) {
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

std::vector<Value> NewEvaluator::builtin_reverse(const std::vector<std::vector<Value>>& args) {
    if (args.empty()) throw std::runtime_error("reverse requires input");
    
    const Value& val = args[0][0];
    
    if (!val.is_array()) {
        return {val};
    }
    
    std::vector<Value> reversed = val.as_array();
    std::reverse(reversed.begin(), reversed.end());
    
    return {Value(std::move(reversed))};
}

std::vector<Value> NewEvaluator::builtin_unique(const std::vector<std::vector<Value>>& args) {
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

std::vector<Value> NewEvaluator::builtin_min(const std::vector<std::vector<Value>>& args) {
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

std::vector<Value> NewEvaluator::builtin_max(const std::vector<std::vector<Value>>& args) {
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

std::vector<Value> NewEvaluator::builtin_empty(const std::vector<std::vector<Value>>& args) {
    return {}; // empty returns no results
}

std::vector<Value> NewEvaluator::builtin_tostring(const std::vector<std::vector<Value>>& args) {
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

std::vector<Value> NewEvaluator::builtin_tonumber(const std::vector<std::vector<Value>>& args) {
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

std::vector<Value> NewEvaluator::builtin_floor(const std::vector<std::vector<Value>>& args) {
    if (args.empty()) throw std::runtime_error("floor requires input");
    
    const Value& val = args[0][0];
    
    if (!val.is_number()) throw std::runtime_error("floor requires number");
    
    return {Value(std::floor(val.as_number()))};
}

std::vector<Value> NewEvaluator::builtin_sqrt(const std::vector<std::vector<Value>>& args) {
    if (args.empty()) throw std::runtime_error("sqrt requires input");
    
    const Value& val = args[0][0];
    
    if (!val.is_number()) throw std::runtime_error("sqrt requires number");
    
    return {Value(std::sqrt(val.as_number()))};
}

// Placeholder implementations for remaining functions
std::vector<Value> NewEvaluator::builtin_group_by(const std::vector<std::vector<Value>>& args) {
    throw std::runtime_error("group_by not yet implemented");
}

std::vector<Value> NewEvaluator::builtin_has(const std::vector<std::vector<Value>>& args) {
    throw std::runtime_error("has not yet implemented");
}

std::vector<Value> NewEvaluator::builtin_to_entries(const std::vector<std::vector<Value>>& args) {
    throw std::runtime_error("to_entries not yet implemented");
}

std::vector<Value> NewEvaluator::builtin_from_entries(const std::vector<std::vector<Value>>& args) {
    throw std::runtime_error("from_entries not yet implemented");
}

std::vector<Value> NewEvaluator::builtin_split(const std::vector<std::vector<Value>>& args) {
    throw std::runtime_error("split not yet implemented");
}

std::vector<Value> NewEvaluator::builtin_join(const std::vector<std::vector<Value>>& args) {
    throw std::runtime_error("join not yet implemented");
}

std::vector<Value> NewEvaluator::builtin_startswith(const std::vector<std::vector<Value>>& args) {
    throw std::runtime_error("startswith not yet implemented");
}

std::vector<Value> NewEvaluator::builtin_endswith(const std::vector<std::vector<Value>>& args) {
    throw std::runtime_error("endswith not yet implemented");
}

std::vector<Value> NewEvaluator::builtin_ceil(const std::vector<std::vector<Value>>& args) {
    throw std::runtime_error("ceil not yet implemented");
}

std::vector<Value> NewEvaluator::builtin_round(const std::vector<std::vector<Value>>& args) {
    throw std::runtime_error("round not yet implemented");
}

} // namespace tq
