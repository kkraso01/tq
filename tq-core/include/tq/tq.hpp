#pragma once

#include "value.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "evaluator.hpp"
#include "toon_parser.hpp"

#include <string>
#include <vector>

namespace tq {

// High-level API: query TOON data with TQ expression
// Returns results as TOON strings
std::vector<std::string> query(const std::string& expression, const std::string& data);

// Returns results as Value objects
std::vector<Value> query_values(const std::string& expression, const Value& data);

} // namespace tq
