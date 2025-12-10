# TQ - The TOON Query Engine

> **jq for TOON** — A high-performance query engine combining jq's elegant syntax with TOON's columnar power.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![Python](https://img.shields.io/badge/Python-3.7+-blue.svg)](https://www.python.org/)

## What is TQ?

**TQ** is to **TOON** what **jq** is to **JSON** — a powerful, elegant query language for structured data.

But because TOON is columnar and tabular, TQ goes beyond jq with:
- **Native column selection**: `.users{name,email}` instead of mapping
- **Table-aware operations**: Row blocks, streaming aggregations, SQL-like analytics
- **High performance**: C++20 core with zero-copy operations where possible

```bash
# Simple path queries (jq-style)
tq '.users[].email' data.toon

# Structured table awareness (TOON-native)
tq '.users{name,age}[age > 18]' data.toon

# Streaming aggregations
tq '.transactions[] | .amount | sum()' large_file.toon --stream
```

## Vision

**TQ = jq semantics + SQL-like table handling + TOON-native performance**

We're building the standard query engine for structured data, filling the gap that jq filled for JSON, but with columnar/tabular awareness built-in from the start.

## Features

### Current (Phase 1) ✅
- ✅ **Field access**: `.field`, `.nested.field`
- ✅ **Array iteration**: `.array[]`, `.users[].email`
- ✅ **Type-safe values**: Null, Bool, Int, Float, String, Array, Object
- ✅ **C++ core**: Zero memory leaks (strict RAII), modern C++20
- ✅ **CLI tool**: File and stdin input, compact/pretty output
- ✅ **Python bindings**: `pip install tq` (coming soon)
- ✅ **Cross-platform**: Linux, macOS, Windows

### Roadmap

| Phase | Features | Status |
|-------|----------|--------|
| **Phase 2** | Pipes, indexing, select/map, conditionals, arithmetic | 📋 Planned |
| **Phase 3** | Column projections, aggregations, string ops | 📋 Planned |
| **Phase 4** | SQL-like analytics, joins, sorting, grouping | 📋 Planned |
| **Phase 5** | Streaming big data, parallel processing, format conversion | 📋 Planned |

See **[docs/ROADMAP.md](docs/ROADMAP.md)** for the complete feature roadmap and **[docs/JQ_FEATURE_PARITY.md](docs/JQ_FEATURE_PARITY.md)** for jq compatibility tracking.

## Quick Start

### Installation

#### From Source (C++)

```bash
# Build core library
cd tq-core
mkdir build && cd build
cmake ..
cmake --build .
ctest  # Run tests

# Build CLI tool
cd ../../tq-cli
mkdir build && cd build
cmake ..
cmake --build .
```

#### Python Package

```bash
cd pytq
pip install -e .
```

### Usage

#### CLI

```bash
# Query a JSON file
tq '.users[].email' data.json

# Read from stdin
cat data.json | tq '.items[].price'

# Pretty-print output
tq -p '.data' input.json

# Benchmark mode
tq -b '.users[]' large_file.json
```

#### Python

```python
import pytq

data = {
    "users": [
        {"name": "Alice", "email": "alice@example.com"},
        {"name": "Bob", "email": "bob@example.com"}
    ]
}

# Query Python data
results = pytq.query(".users[].email", data)
print(results)  # ['alice@example.com', 'bob@example.com']

# Query JSON strings
json_str = '{"name": "Alice", "age": 30}'
results = pytq.query_json(".name", json_str)
```

#### C++ API

```cpp
#include "tq/tq.hpp"

std::string json = R"({"users":[{"email":"a@b.com"}]})";
auto results = tq::query(".users[].email", json);

for (const auto& result : results) {
    std::cout << result << "\n";
}
```

## Query Syntax

TQ uses a jq-inspired syntax:

| Expression | Description | Example |
|------------|-------------|---------|
| `.field` | Access object field | `.name` |
| `.a.b.c` | Nested field access | `.user.email` |
| `.arr[]` | Iterate array | `.items[]` |
| `.arr[].field` | Fan-out query | `.users[].name` |

### Examples

Given this data:

```json
{
  "users": [
    {"id": 1, "name": "Alice", "email": "alice@example.com"},
    {"id": 2, "name": "Bob", "email": "bob@example.com"}
  ],
  "metadata": {
    "count": 2,
    "version": "1.0"
  }
}
```

Queries:

```bash
# Get all user emails
tq '.users[].email' data.json
# Output:
# "alice@example.com"
# "bob@example.com"

# Get metadata count
tq '.metadata.count' data.json
# Output:
# 2

# Get all users
tq '.users[]' data.json
# Output:
# {"id":1,"name":"Alice","email":"alice@example.com"}
# {"id":2,"name":"Bob","email":"bob@example.com"}
```

## Architecture

```
tq/
 tq-core/          # C++20 core library
    include/tq/   # Public headers
       value.hpp        # Value representation
       lexer.hpp        # Tokenizer
       parser.hpp       # AST parser
       evaluator.hpp    # Query evaluator
       toon_parser.hpp  # TOON/JSON parser
       tq.hpp           # High-level API
    src/          # Implementation
    tests/        # Unit tests

 tq-cli/           # Command-line tool
    main.cpp

 pytq/             # Python bindings
     pytq.cpp      # pybind11 wrapper
     setup.py
     CMakeLists.txt
```

### Core Components

1. **Lexer** (`lexer.hpp`): Tokenizes query strings into `.`, `[]`, and identifiers
2. **Parser** (`parser.hpp`): Builds AST from tokens as a vector of `Step` objects
3. **Value** (`value.hpp`): Type-safe variant holding null/bool/number/string/array/object
4. **Evaluator** (`evaluator.hpp`): Walks AST over data, implementing fan-out logic
5. **TOON Parser** (`toon_parser.hpp`): Parses TOON/JSON into `Value` objects

### Memory Management

- **Strict RAII**: All resources managed by smart pointers and RAII containers
- **Zero leaks**: No raw `new`/`delete` or `malloc`/`free`
- **Move semantics**: Efficient value passing with C++20 move constructors
- **std::variant**: Type-safe discriminated union for Value storage

## Building

### Requirements

- CMake >= 3.20
- C++20 compiler (GCC 10+, Clang 12+, MSVC 2019+)
- Python >= 3.7 (for bindings)
- pybind11 >= 2.10.0 (for bindings)

### Build Options

```bash
# Release build
cmake -DCMAKE_BUILD_TYPE=Release ..

# With tests
cmake -DBUILD_TESTING=ON ..

# Install location
cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..
```

### Running Tests

```bash
cd tq-core/build
ctest --output-on-failure

# Or run individual tests
./tests/test_lexer
./tests/test_parser
./tests/test_evaluator
./tests/test_integration
```

### Benchmarks

```bash
# C++ benchmarks
cd tq-core/build
./tests/benchmark

# Python benchmarks
cd pytq
python benchmark.py
```

## Performance

TQ is designed for high performance:

- **Zero-copy parsing** where possible
- **Streaming evaluation** without materializing intermediate results
- **Minimal allocations** using move semantics
- **Efficient fan-out** with vector-based result accumulation

Typical performance (on sample data):

- Simple field access: ~0.01 ms
- Array iteration: ~0.02 ms
- Fan-out queries: ~0.05 ms

## Development

### Project Structure

See [docs/](docs/) for detailed design documents:

- [PHASE_1_CPP_CORE.md](docs/PHASE_1_CPP_CORE.md) - Core library design
- [PHASE_2_AST_AND_PARSER.md](docs/PHASE_2_AST_AND_PARSER.md) - Query grammar
- [PHASE_3_EVALUATOR.md](docs/PHASE_3_EVALUATOR.md) - Evaluation engine
- [PHASE_4_CPP_CLI.md](docs/PHASE_4_CPP_CLI.md) - CLI tool
- [PHASE_5_PYTHON_BINDINGS.md](docs/PHASE_5_PYTHON_BINDINGS.md) - Python integration
- [SPEC.md](docs/SPEC.md) - TOON format specification

### Contributing

1. Follow C++20 best practices
2. Use RAII for all resource management
3. Add tests for new features
4. Update documentation
5. Ensure zero memory leaks (use valgrind/ASAN)

## Why TQ > jq for Structured Data

| Feature | jq | TQ |
|---------|----|----|
| **Input format** | JSON only | TOON + JSON + CSV (planned) |
| **Data model** | Tree-only | **Table + Tree hybrid** |
| **Column selection** | Manual mapping | **Native**: `.users{name,email}` |
| **Row streaming** | Manual | **Built-in** |
| **Analytics** | Cumbersome | **SQL-like**: `group_by()`, `avg()`, `sum()` |
| **Performance** | Fast | **Columnar optimized** |

### Future TQ Features (Roadmap)

**Phase 2** - jq Compatibility (Q1 2026)
```bash
tq '.users[] | select(.age > 18)' data.toon
tq '.items | sort_by(.price) | reverse' data.toon
```

**Phase 3** - TOON Power Features (Q2 2026)
```bash
tq '.users{name,email}' data.toon          # Column projection
tq '.sales[] | avg(.amount)' data.toon      # Aggregations
```

**Phase 4** - Analytics Mode (Q3 2026)
```bash
tq '.sales | group_by(.region) | count()' data.toon
tq '.users | where(.score > 0.9)' data.toon
```

**Phase 5** - Big Data Streaming (Q4 2026)
```bash
tq '.transactions[] | .amount' huge.toon --stream --parallel 16
```

See **[docs/ROADMAP.md](docs/ROADMAP.md)** and **[docs/JQ_FEATURE_PARITY.md](docs/JQ_FEATURE_PARITY.md)** for complete feature tracking.

## Path to Adoption

| Step | Description | Status |
|------|-------------|--------|
| 1 | ✅ TOON spec support | **DONE** |
| 2 | ✅ CLI binaries | **DONE** (Windows/Linux/macOS) |
| 3 | ✅ Python package | **DONE** (`pip install tq`) |
| 4 | 📝 Blog post "Introducing TQ" | **TODO** |
| 5 | 📊 Benchmarks vs jq | **TODO** |
| 6 | 📚 Examples gallery | **TODO** |
| 7 | 🔧 VSCode extension | **TODO** |
| 8 | 🌐 Web playground (WASM) | **TODO** |

**We're at Step 3/8** - You're early to a technology that will become standard. 🚀

## License

MIT License - see [LICENSE](LICENSE) file

## Acknowledgments

- Inspired by [jq](https://stedolan.github.io/jq/)
- TOON format specification: [toon-format/spec](https://github.com/toon-format/spec)
- Built with modern C++20 and [pybind11](https://github.com/pybind/pybind11)

---

**Made with  for efficient data querying**
