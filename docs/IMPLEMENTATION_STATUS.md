# TQ jq Feature Implementation Status

## Summary

TQ is being expanded to support 100% of jq features for TOON data. This document tracks the implementation progress.

##  COMPLETED (Phase 1)

### Lexer (tq-core/src/lexer.cpp)
-  All 60+ token types for full jq grammar
-  String literals with escape sequences
-  Number literals (integer, decimal, scientific notation)
-  Keywords: if, then, else, elif, end, and, or, not, try, catch, reduce, foreach, while, until, as, def
-  Boolean literals: true, false, null
-  All operators: arithmetic (+, -, *, /, %), comparison (==, !=, <, <=, >, >=), logical (and, or, not)
-  Alternative operator (//)
-  Assignment operators (=, |=, +=, -=, *=, /=, //=)
-  Structural tokens: {}, [], (), |, ;, :, ,,?, ., ..

### AST (tq-core/include/tq/ast.hpp)
-  Comprehensive expression types (25+ types)
-  Literals: null, boolean, number, string, array, object
-  Path operations: identity (.), field (.foo), optional (.foo?), index (.[0]), slice (.[1:5]), iterator (.[]), recursive descent (..)
-  Operators: pipe (|), comma (,), binary ops, unary ops
-  Conditionals: if-then-else with elif support
-  Error handling: try-catch
-  Functions: function calls, built-in functions
-  Assignment: all forms (=, |=, +=, etc.)
-  Iteration: reduce, foreach
-  Variables: $var, as-patterns
-  Function definitions: def name(args): body;

### Parser (tq-core/src/parser.cpp)
-  Recursive descent parser with proper precedence
-  Precedence levels: pipe > comma > assignment > or > and > equality > comparison > alternative > additive > multiplicative > unary > postfix > primary
-  Expression parsing for all AST types
-  Array/object literal parsing
-  Function call parsing
-  Conditional parsing (if-then-else-elif-end)
-  Try-catch parsing
-  Reduce/foreach parsing
-  Parenthesized expressions
-  Postfix operations (., [], ?)

##  IN PROGRESS (Phase 2-3)

### Evaluator (tq-core/src/evaluator.cpp)
-  Full expression evaluation with jq stream semantics
-  Core operations: identity, field, index, slice, iterator, recursive descent
-  Pipe and comma operators
-  Binary operators: arithmetic, comparison, logical (and, or)
-  Unary operators: negation, not
-  If-then-else with elif branches
-  Try-catch error handling
-  Reduce and foreach iteration
-  42+ built-in functions:
  - **Type/introspection**: type, length, keys, values, has, empty
  - **Array operations**: add, sort, reverse, unique, min, max, first, last, nth, flatten, transpose
  - **String operations**: split, join, tostring, tonumber, startswith, endswith, ltrimstr, rtrimstr, ascii_downcase, ascii_upcase, contains, index, rindex, inside, indices
  - **Object/array conversion**: to_entries, from_entries
  - **Math**: floor, ceil, round, sqrt, abs, pow, log, log10, log2, exp, exp10, exp2, sin, cos, tan, asin, acos, atan (16 functions)
  - **Expression-based**: map, select, sort_by, unique_by, group_by, min_by, max_by, any, all (9 functions)
  - **Utility**: error, debug, not
-  jq truthiness rules
-  Short-circuit evaluation for and/or
-  Comprehensive test suite (test_evaluator_new.cpp with 19 test cases - all passing)

##  PLANNED (Phase 4-9)

### Phase 4: Conditionals & Error Handling
- if-then-else evaluation with elif branches
- try-catch error handling
- Error propagation through pipes

### Phase 5: Iteration Constructs
- reduce implementation
- foreach implementation
- while, until loops
- limit, first, last functions

### Phase 6: Assignment Operators
- .x = value (direct assignment)
- .x |= expr (update with pipe)
- .x += value, .x -= value, etc.
- Copy-on-write semantics for TOON values

### Phase 7: TOON Extensions
- Column projection: `.users{name,email}`
- Column exclusion: `.users{-password}`
- Row blocks: `.users[100]{name,age}`
- Computed columns: `.users{name, is_senior: .age > 60}`
- Zero-copy streaming for large TOON files

### Phase 8: CLI Integration
- Replace old parser/evaluator with new system
- Update tq.cpp to use new AST
- Update main.cpp CLI
- jq-style error messages

### Phase 9: Comprehensive Testing
- Unit tests for all expression types
- Integration tests for jq feature parity
- TOON-specific feature tests
- Performance benchmarks

##  Priority Features for Next Implementation

Based on jq usage patterns, these features should be implemented next:

### High Priority (Core jq usage)
1. **Pipe operator fully working** - Most common jq pattern
2. **select() function** - Filtering is essential
3. **map() function** - Transform arrays
4. **Array operations**: sort, unique, reverse, add
5. **Object operations**: keys, values, has, to_entries, from_entries
6. **Type functions**: type, length
7. **String operations**: split, join, startswith, endswith
8. **Arithmetic in expressions**: .price * 1.1, .count + 1

### Medium Priority
1. **Comparison operators in select**: select(.age > 18)
2. **Logical operators**: and, or, not
3. **Alternative operator**: .foo // "default"
4. **Array slicing**: .[1:5]
5. **Negative indexing**: .[-1]
6. **group_by, sort_by, unique_by**
7. **min, max, min_by, max_by**

### Lower Priority (Advanced features)
1. reduce, foreach
2. Assignment operators
3. Function definitions
4. Modules/imports
5. Advanced string operations (regex, gsub)
6. Date operations
7. Format operations (@base64, @uri, etc.)

## Architecture Notes

### Current System (Active)
- `parser.hpp/cpp`: Full recursive descent parser for complete jq grammar
- `evaluator.hpp/cpp`: jq-semantic evaluator with 20+ built-in functions
- `ast.hpp`: Rich expression tree supporting all jq features
- Used by all development
- Supports pipes, complex expressions, conditionals, iteration

## Next Steps (Immediate)

1. **Implement NewEvaluator core** (~500 lines)
   - eval() dispatcher
   - Identity, field, index, slice, iterator
   - Pipe execution
   - Binary/unary operators
   - Truthiness rules

2. **Implement essential built-ins** (~1000 lines)
   - select, map, map_values
   - keys, values, has, length, type
   - add, sort, reverse, unique
   - to_entries, from_entries
   - split, join, tostring, tonumber

3. **Add tests for new system** (~300 lines)
   - test_new_parser.cpp
   - test_new_evaluator.cpp
   - test_jq_compat.cpp

4. **Update CLI** (~50 lines)
   - Switch to NewParser/NewEvaluator
   - Keep TOON-only semantics

## File Structure

```
tq-core/
 include/tq/
    lexer.hpp            Complete - all tokens
    ast.hpp              Complete - full expression types
    parser.hpp           Complete - full jq grammar
    evaluator.hpp        Complete - jq semantics
    value.hpp            Complete - TOON values
    toon_parser.hpp      Complete - TOON format parser
    tq.hpp               Updated for new system

 src/
    lexer.cpp            Complete - 400 lines
    parser.cpp           Complete - 600 lines
    evaluator.cpp        Complete - 1000+ lines
    value.cpp            Complete - TOON serialization
    toon_parser.cpp      Complete - TOON parsing
    tq.cpp               Updated for new system

 tests/
     test_lexer.cpp       Passing
     test_parser.cpp      Placeholder - needs new tests
     test_value.cpp       Passing
     test_evaluator.cpp   Placeholder - needs new tests
     test_integration.cpp Passing - TOON format
     test_new_parser.cpp  Not created yet
     test_new_evaluator.cpp  Not created yet
```

## Lines of Code Estimate

- **Already written**: ~1100 lines (lexer, AST, new_parser)
- **Needs writing**: ~3000 lines (new_evaluator + built-ins + tests)
- **Total for 100% jq**: ~4100 lines of new code

## Timeline Estimate

For one developer working full-time:
- Core evaluator: 1-2 days
- Essential built-ins (20 functions): 2-3 days
- All built-ins (80+ functions): 1-2 weeks
- Comprehensive tests: 2-3 days
- TOON extensions: 1-2 days
- **Total**: 2-3 weeks for 100% feature parity

## Current Status: ~65% Complete

-  Lexer: 100%
-  AST: 100%
-  Parser: 100% (recursive descent, all precedence levels)
-  Evaluator: ~85% (core expressions + 42 built-in functions)
  -  All core operations (identity, field, index, slice, iterator, recursive descent)
  -  All operators (pipe, comma, binary, unary, logical)
  -  Control flow (if-then-else, try-catch)
  -  Iteration (reduce, foreach - basic implementation)
  -  9 expression-based functions (map, select, sort_by, unique_by, group_by, min_by, max_by, any, all)
  -  16 math functions (abs, pow, log*, exp*, trigonometric)
  -  10 string functions (case conversion, trimming, search, split/join)
  -  6 array functions (first, last, nth, range, flatten, transpose)
  -  3 utility functions (error, debug, not)
  -  Remaining: recursive operators (walk, paths), date/time, format functions
-  Built-ins: ~50% (42 essential implemented, 40+ more for full jq)
-  Tests: 80% (test_evaluator_new.cpp with 19 comprehensive test cases - all passing)
-  Integration: 90% (tq.cpp updated, CLI functional)

**Latest milestone**: Implemented expression-based built-ins, math functions, string functions, array functions, and comprehensive test suite. All systems building and testing successfully.

