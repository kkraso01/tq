# TQ Query Engine - Implementation Summary

## Overview
TQ is a high-performance query engine for **TOON data only** (not JSON), inspired by `jq`. This document summarizes the complete C/C++ core implementation with phase completion status.

## Project Structure

```
tq/
 tq-core/              # C/C++ Core Library (Phase 1-3)
    include/          # Public headers
    src/              # Implementation
       lexer.cpp
       parser.cpp
       ast.cpp
       evaluator.cpp
       value.cpp
       toon_parser.cpp
       query.cpp
    tests/            # Comprehensive test suite
 tq-cli/               # CLI Tool (Phase 4)
    main.cpp
 pytq/                 # Python Bindings (Phase 5)
 docs/                 # Design documentation
 toon-reference/       # TOON specification
```

## Completed Phases

### Phase 1: C/C++ Core 
**Status**: COMPLETE - All core components implemented and tested

**Components**:
- **Lexer** (`lexer.cpp`): Tokenizes query strings into tokens (field names, brackets, dots, etc.)
- **Parser** (`parser.cpp`): Builds Abstract Syntax Tree (AST) from tokens using recursive descent
- **AST** (`ast.cpp`): Query representation as sequence of `Step` structs
  - Step types: FIELD_LOOKUP, ARRAY_ITERATION, ARRAY_INDEX
- **Value** (`value.cpp`): Native C++ representation of TOON data
  - Supports: scalars (string/int/float/bool), arrays, objects, null
  - Method: `Value::to_toon()` converts to TOON format string
- **TOON Parser** (`toon_parser.cpp`): Native parser for TOON data format
  - Full support for TOON specification
  - Proper handling of indent-based structure, arrays, and scalar types
- **Query Function** (`query.cpp`): Main entry point for query execution

**Key Features**:
- RAII memory management (std::unique_ptr, std::vector)
- Zero-copy evaluation where possible
- Support for nested field access and array iteration
- Proper error handling with detailed error messages

**Build Output**:
- `libtq_core.dll` (Windows) / `.so` (Linux) / `.dylib` (macOS): Shared library for bindings
- `libtq_core_static.lib` (Windows): Static library for CLI

### Phase 2: AST and Parser 
**Status**: COMPLETE - Full query grammar support implemented

**Supported Grammar**:
```
Query     ::= Step+
Step      ::= Prefix ( ArrayAccess | Identity )
Prefix    ::= '.' | '..'
ArrayAccess ::= '[' ( INT | '*' ) ']'
Identity  ::= IDENTIFIER | '.' IDENTIFIER
```

**Parsing Examples**:
```
.users            Field lookup
.users[]          Array iteration
.users[0]         Array index
.users[].name     Nested field + array iteration
.a.b.c            Chained field lookups
```

**AST Structure**:
```cpp
struct Step {
    StepType type;      // FIELD_LOOKUP, ARRAY_ITERATION, ARRAY_INDEX
    std::string field;  // For FIELD_LOOKUP
    int index;          // For ARRAY_INDEX
};

struct AST {
    std::vector<Step> steps;
};
```

**Recursion Prevention**: Parser has built-in depth limit (100) to prevent stack overflow on malformed input.

### Phase 3: Evaluator 
**Status**: COMPLETE - Full AST-based evaluation engine

**Algorithm**: 
1. Parse query into AST
2. Walk AST over TOON Value tree
3. Accumulate results in output vector
4. Return as native Value objects or TOON strings

**Evaluation Flow**:
```
Input: TOON String  ToonParser  Value Tree
         Query String  Parser  AST
              Value Tree + AST  Evaluator  Result Values
                           Result Values  Value::to_toon()  TOON Output
```

**Features**:
- Efficient recursive descent evaluation
- Array iteration with proper flattening
- Handles edge cases: empty arrays, missing fields, null values
- Result accumulation without intermediate copies

### Phase 4: CLI Tool 
**Status**: COMPLETE - Full TOON-only CLI implementation

**Usage**:
```bash
tq ".users[].name" input.toon
cat input.toon | tq ".items[].price"
```

**Features**:
- Reads TOON files (not JSON)
- Outputs TOON format (not JSON)
- Stdin support with streaming
- Proper error reporting
- Optimized for performance

**Binary Location**: `build/tq-cli/Release/tq.exe` (Windows)

### Phase 5: Python Bindings 
**Status**: READY TO IMPLEMENT
- Bindings file: `pytq/pytq.cpp`
- Uses pybind11 for C++/Python bridge
- Exposes `query(query_str, toon_input)` function
- Returns: List of TOON strings
- Build: `python -m build` or `pip install -e .`

### Phase 6: WASM Build 
**Status**: READY TO IMPLEMENT
- Build system: Emscripten
- Exports: `_tq_query` function for browser/Node.js
- See: `docs/PHASE_6_WASM.md`

## Test Suite

**Components Tested**:
-  Lexer (test_lexer.exe)
-  Parser (test_parser.exe)
-  Value/TOON (test_value.exe)
-  Integration tests (test_integration.exe)
-  Evaluator new AST (test_evaluator_new.exe)

**Test Execution**:
```powershell
cd build/tq-core/tests/Release
.\test_integration.exe      # All integration tests pass
.\test_lexer.exe           # Lexer tests pass
.\test_parser.exe          # Parser tests pass
.\test_value.exe           # Value/TOON tests pass
```

**Coverage**: 
- Lexer: All token types
- Parser: All query patterns, error cases, edge cases
- TOON: All data types, nested structures
- Integration: End-to-end query execution

## Critical Implementation Details

### TOON Format
- **Input**: Only TOON format accepted (no JSON)
- **Output**: Only TOON format produced (no JSON conversion)
- **Parser**: Indent-based, type-inferred (strings, numbers, booleans)
- **Special**: Bracket notation for arrays, colon for key-value pairs

### Memory Management
- All heap allocations use `std::unique_ptr` or `std::vector`
- No manual `delete` calls (RAII enforced)
- No memory leaks or dangling pointers
- Efficient move semantics for string data

### Error Handling
- Parser errors: Detailed position/context information
- Runtime errors: Graceful handling with descriptive messages
- Edge cases: Empty arrays, missing fields, type mismatches

## Build System

**CMake Configuration**:
- Separate targets for core library and CLI
- Release/Debug configurations
- Automatic dependency management
- Cross-platform support (Windows/Linux/macOS)

**Build Commands**:
```powershell
cd build
cmake --build . --config Release
```

**Output Binaries**:
- `tq-core/Release/tq_core.dll` - Core library
- `tq-core/Release/tq_core_static.lib` - Static library
- `tq-cli/Release/tq.exe` - CLI executable

## Example Usage

**CLI Example**:
```bash
# Input TOON data
users[2]:
  - name: alice
    email: alice@example.com
  - name: bob
    email: bob@example.com

# Query and result
$ tq ".users[].name" < input.toon
alice
bob
```

**Direct API Example** (Phase 1 core):
```cpp
#include "tq-core/include/query.h"

std::string toon_data = "users[2]:...";
std::string query = ".users[].name";
auto results = tq::query(query, toon_data);
// results contains: ["alice", "bob"] as TOON strings
```

## Performance Characteristics

- **Time Complexity**: O(n) where n = size of input data
- **Space Complexity**: O(r) where r = size of results
- **No Intermediate Copies**: Direct tree traversal
- **Efficient String Handling**: Move semantics where possible

## Next Steps

1. **Python Bindings** (Phase 5): Build with `python -m build` in `pytq/`
2. **WASM Build** (Phase 6): Compile with Emscripten as documented
3. **CLI Extensions**: Add filter operations, output formatting options
4. **Performance Optimization**: Profile and optimize hot paths

## Reference Documentation

- `docs/PHASE_1_CPP_CORE.md` - Core architecture and design
- `docs/PHASE_2_AST_AND_PARSER.md` - Grammar and parsing
- `docs/PHASE_3_EVALUATOR.md` - Evaluation algorithm
- `docs/PHASE_4_CPP_CLI.md` - CLI implementation
- `toon-reference/SPEC.md` - TOON format specification

## Verification

All components have been built and tested:
-  Release build successful
-  All unit tests pass
-  Integration tests pass
-  CLI end-to-end functional
-  TOON input/output working correctly

The C/C++ core implementation is complete, tested, and production-ready for phases 5 (Python) and 6 (WASM) to proceed.
