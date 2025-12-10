#pragma once

#include "ast.hpp"
#include "value.hpp"
#include <vector>
#include <map>
#include <string>
#include <functional>
#include <queue>

namespace tq {

// Built-in function type for value-based functions
using BuiltinFunc = std::function<std::vector<Value>(const std::vector<std::vector<Value>>&)>;

// Built-in function type for expression-based functions (e.g., map, select)
// Takes: evaluator context, expression, data
using ExprBuiltinFunc = std::function<std::vector<Value>(class Evaluator*, const ExprPtr&, const Value&)>;

class Evaluator {
public:
    Evaluator();
    
    // Set input values for input/inputs functions
    void set_input_values(const std::vector<Value>& values);
    
    // Evaluate expression against data, returning multiple results (jq stream semantics)
    std::vector<Value> eval(const ExprPtr& expr, const Value& data);
    
private:
    // Built-in functions registry
    std::map<std::string, BuiltinFunc> builtins_;
    
    // Expression-based built-in functions registry
    std::map<std::string, ExprBuiltinFunc> expr_builtins_;
    
    // Variable environment (for as-patterns, reduce, etc.)
    std::map<std::string, Value> vars_;
    
    // Input stream for input/inputs functions
    std::queue<Value> input_stream_;
    
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
    std::vector<Value> builtin_abs(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_pow(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_log(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_log10(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_log2(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_exp(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_exp10(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_exp2(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_sin(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_cos(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_tan(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_asin(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_acos(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_atan(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_ltrimstr(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_rtrimstr(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_ascii_downcase(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_ascii_upcase(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_contains(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_index(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_rindex(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_inside(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_indices(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_first(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_last(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_nth(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_range(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_flatten(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_transpose(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_error(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_debug(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_not(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_paths(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_leaf_paths(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_keys_unsorted(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_min_by_value(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_max_by_value(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_to_array(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_to_object(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_combinations(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_numbers(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_strings(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_arrays(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_objects(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_nulls(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_booleans(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_scalars(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_iterables(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_ascii(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_implode(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_explode(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_tojsonstream(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_fromjsonstream(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_walk(const ExprPtr& expr, const Value& data);
    std::vector<Value> builtin_sqrt(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_empty(const std::vector<std::vector<Value>>& args);
    
    // Date/time functions
    std::vector<Value> builtin_now(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_gmtime(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_mktime(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_strftime(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_strptime(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_todate(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_fromdate(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_todateiso8601(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_fromdateiso8601(const std::vector<std::vector<Value>>& args);
    
    // Format functions (@base64, @uri, @csv, @tsv, @html, @json, @text, @base64d)
    std::vector<Value> builtin_format_base64(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_format_base64d(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_format_uri(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_format_csv(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_format_tsv(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_format_html(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_format_json(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_format_text(const std::vector<std::vector<Value>>& args);
    
    // Expression-based built-ins
    std::vector<Value> builtin_map(const ExprPtr& expr, const Value& data);
    std::vector<Value> builtin_select(const ExprPtr& expr, const Value& data);
    std::vector<Value> builtin_sort_by(const ExprPtr& expr, const Value& data);
    std::vector<Value> builtin_unique_by(const ExprPtr& expr, const Value& data);
    std::vector<Value> builtin_group_by(const ExprPtr& expr, const Value& data);
    std::vector<Value> builtin_min_by(const ExprPtr& expr, const Value& data);
    std::vector<Value> builtin_max_by(const ExprPtr& expr, const Value& data);
    std::vector<Value> builtin_any(const ExprPtr& expr, const Value& data);
    std::vector<Value> builtin_all(const ExprPtr& expr, const Value& data);
    
    // I/O and SQL-style functions
    std::vector<Value> builtin_limit(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_input(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_inputs(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_INDEX(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_IN(const std::vector<std::vector<Value>>& args);
    std::vector<Value> builtin_GROUP_BY_advanced(const ExprPtr& expr, const Value& data);
    
    // ... many more
};

} // namespace tq
