# TQ Manual - Complete Reference

> **TQ is jq for TOON**  A complete query language for structured data with columnar/tabular awareness.

**Version:** 0.2.0 (Phase 2 in development)  
**Last Updated:** December 10, 2025

---

## Table of Contents

1. [Introduction](#introduction)
2. [Installation](#installation)
3. [Basic Usage](#basic-usage)
4. [Query Syntax](#query-syntax)
5. [Operators & Functions](#operators--functions)
6. [Advanced Features](#advanced-features)
7. [TOON-Specific Features](#toon-specific-features)
8. [API Reference](#api-reference)
9. [Performance](#performance)
10. [Roadmap & Status](#roadmap--status)

---

## Introduction

### What is TQ?

TQ (TOON Query) is a high-performance query engine that brings jq's elegant syntax to TOON data format, with additional columnar and tabular awareness. It's implemented in modern C++20 with bindings for Python and planned WASM support.

### Key Features

- **jq-compatible syntax** - If you know jq, you know TQ
- **TOON-native** - First-class support for columnar/tabular data
- **High performance** - C++20 core with zero-copy operations
- **Cross-platform** - Linux, macOS, Windows
- **Multiple interfaces** - CLI, C++ API, Python bindings, WASM (planned)

### TQ vs jq

| Feature | jq | TQ |
|---------|----|----|
| Input format | JSON only | TOON + JSON |
| Data model | Tree-only | Table + Tree hybrid |
| Column selection | Manual mapping | Native: `.users{name,email}` |
| Row streaming | Manual | Built-in |
| Analytics | Limited | SQL-like (planned) |
| Performance | Fast | Columnar-optimized |

---

## Installation

### From Source (C++)

```bash
# Clone and build
git clone https://github.com/yourusername/tq.git
cd tq

# Build with CMake
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64  # Windows
# or
cmake ..  # Linux/macOS
cmake --build . --config Release

# Run tests
ctest --output-on-failure
```

### Python Package

```bash
pip install tq
```

### Binary Releases

Download pre-built binaries from [releases page](https://github.com/yourusername/tq/releases).

---

## Basic Usage

### CLI Tool

```bash
# Query a file
tq '.users[].email' data.json

# From stdin
echo '{"name":"Alice"}' | tq '.name'

# Pretty print
tq -p '.' data.json

# Compact output
tq -c '.items[]' data.json
```

### Python API

```python
import pytq

# Query Python data
data = {"users": [{"name": "Alice", "age": 30}]}
result = pytq.query(".users[].name", data)
print(result)  # ["Alice"]

# Query JSON string
json_str = '{"x": 42}'
result = pytq.query_json(".x", json_str)
```

### C++ API

```cpp
#include "tq/tq.hpp"

std::string json = R"({"users":[{"name":"Alice"}]})";
auto results = tq::query(".users[].name", json);
for (const auto& r : results) {
    std::cout << r << "\n";
}
```

---

## Query Syntax

### Basic Filters

#### Identity: `.`

Returns input unchanged.

```bash
echo '42' | tq '.'
# 42
```

#### Field Access: `.foo`, `.foo.bar`

Access object fields.

```bash
echo '{"name":"Alice","age":30}' | tq '.name'
# "Alice"

echo '{"user":{"email":"alice@example.com"}}' | tq '.user.email'
# "alice@example.com"
```

#### Optional Field: `.foo?`

No error if field doesn't exist (planned Phase 2).

```bash
tq '.missing?' data.json
```

### Array Operations

#### Array Index: `.[n]`

 **Implemented** (Phase 2)

```bash
echo '[10,20,30]' | tq '.[0]'
# 10

echo '[10,20,30]' | tq '.[-1]'
# 30
```

#### Array Slice: `.[start:end]`

 **Implemented** (Phase 2)

```bash
echo '[1,2,3,4,5]' | tq '.[1:4]'
# [2,3,4]

echo '[1,2,3,4,5]' | tq '.[2:]'
# [3,4,5]

echo '[1,2,3,4,5]' | tq '.[:3]'
# [1,2,3]
```

#### Array Iterator: `.[]`

 **Implemented** (Phase 1)

```bash
echo '[1,2,3]' | tq '.[]'
# 1
# 2
# 3
```

### Pipe Operator: `|`

 **Implemented** (Phase 2)

Chains operations left-to-right.

```bash
tq '.users | .[0] | .name' data.json

tq '.items | .[] | .price' data.json
```

### Comma Operator: `,`

 **Planned** (Phase 2)

Produces multiple outputs.

```bash
tq '.foo, .bar' data.json
```

---

## Operators & Functions

### Comparison ( Planned Phase 2)

| Operator | Description | Example |
|----------|-------------|---------|
| `==` | Equal | `.age == 30` |
| `!=` | Not equal | `.status != "active"` |
| `<` | Less than | `.price < 100` |
| `<=` | Less or equal | `.age <= 18` |
| `>` | Greater than | `.score > 90` |
| `>=` | Greater or equal | `.count >= 10` |

### Arithmetic ( Planned Phase 2)

| Operator | Description | Example |
|----------|-------------|---------|
| `+` | Addition | `.x + .y` |
| `-` | Subtraction | `.total - .discount` |
| `*` | Multiplication | `.price * .quantity` |
| `/` | Division | `.sum / .count` |
| `%` | Modulo | `.num % 2` |

### Logical ( Planned Phase 2)

| Operator | Description | Example |
|----------|-------------|---------|
| `and` | Logical AND | `.active and .verified` |
| `or` | Logical OR | `.premium or .trial` |
| `not` | Logical NOT | `not .deleted` |
| `//` | Alternative | `.name // "unknown"` |

### Type Operations ( Planned Phase 2-3)

```bash
# Type checking
type                    # Get type as string
isnumber, isstring, isarray, isobject, isboolean, isnull

# Type conversion
tonumber, tostring, toboolean

# Type functions
length                  # Length of string/array/object
keys, keys_unsorted     # Object keys or array indices
values                  # Object values
```

### Array Functions ( Planned Phase 2-3)

```bash
# Selection & transformation
map(.x)                 # Transform each element
select(.x > 5)          # Filter elements
sort, sort_by(.field)   # Sort array
reverse                 # Reverse array
unique, unique_by(.x)   # Remove duplicates
flatten, flatten(depth) # Flatten nested arrays

# Aggregation
add                     # Sum/concatenate all
min, max                # Min/max value
min_by(.x), max_by(.x)  # Min/max by expression
group_by(.field)        # Group by key

# Search
contains(x)             # Check if contains
index(x), rindex(x)     # Find index
indices(x)              # All indices
inside(x)               # Inverse of contains
```

### String Functions ( Planned Phase 2-3)

```bash
# Case conversion
ascii_upcase, ascii_downcase

# Trimming
ltrimstr("prefix")      # Remove prefix
rtrimstr("suffix")      # Remove suffix
trimstr("str")          # Remove from both ends
trim, ltrim, rtrim      # Trim whitespace

# Splitting & joining
split(",")              # Split on delimiter
join(",")               # Join with delimiter
explode, implode        # String  codepoints

# Searching
startswith("str")       # Check prefix
endswith("str")         # Check suffix
test("regex")           # Test regex match
match("regex")          # Get match object
```

### Conditional Expressions ( Planned Phase 2)

```bash
# if-then-else
if .age >= 18 then "adult" else "minor" end

# elif chains
if .score >= 90 then "A"
elif .score >= 80 then "B"
elif .score >= 70 then "C"
else "F"
end

# Alternative operator
.name // "unknown"      # First non-null/false value

# Try-catch
try .field catch "default"
```

### Object Operations ( Planned Phase 3)

```bash
# Construction
{name: .firstName, age: .userAge}
{user, title}           # Shorthand
{(.key): .value}        # Computed key

# Manipulation
has("key")              # Check key existence
in({...})               # Check membership
del(.field)             # Delete field
with_entries(.key |= "new")  # Transform entries

# Conversion
to_entries              # [{key:k, value:v}, ...]
from_entries            # Inverse of to_entries
```

### Path Operations ( Planned Phase 3)

```bash
path(.a.b)              # Get path as array
getpath(["a","b"])      # Get value at path
setpath(["a","b"]; val) # Set value at path
delpaths([["a"],["b"]]) # Delete paths
paths                   # All paths in data
paths(filter)           # Filtered paths
```

---

## Advanced Features

### Variables ( Planned Phase 3-4)

```bash
# Bind variable
.realnames as $names | .posts[] | {title, author: $names[.author]}

# Multiple bindings
. as {realnames: $names, posts: [$first, $second]} | ...

# In object construction
"value" as $x | {foo: $x, bar: $x}
```

### Functions ( Planned Phase 3-4)

```bash
# Define function
def increment: . + 1;

# With arguments
def multiply(n): . * n;

# Multiple definitions
def addvalue($x): map(. + $x);
```

### Reduce & Foreach ( Planned Phase 4)

```bash
# Reduce
reduce .[] as $item (0; . + $item)

# Foreach
foreach .[] as $item (0; . + $item; .)

# Limit/first/last
limit(5; .items[])
first(.users[])
last(.events[])
```

### Regular Expressions ( Planned Phase 3)

```bash
# Test
test("pattern")
test("pattern"; "flags")

# Match
match("regex")
capture("(?<name>\\w+)")

# Replace
sub("old"; "new")
gsub("old"; "new"; "flags")

# Split
split("regex"; "flags")
scan("pattern")
```

---

## TOON-Specific Features

### Column Projection ( Planned Phase 3)

Select specific columns from tabular data:

```bash
# Select columns
.users{name,email,age}

# Exclude columns
.users{-password,-ssn}

# Computed columns
.products{name, taxed: .price * 1.2}

# Nested projections
.orders{id, customer{name,email}}
```

### Row Blocks ( Planned Phase 3)

Fetch limited rows with specific columns:

```bash
# First 100 rows with selected columns
.users[100]{name,email}

# Specific range with columns
.transactions[1000:2000]{id,amount,date}
```

### Streaming Aggregations ( Planned Phase 4-5)

Constant-memory aggregations:

```bash
# Stream processing
tq '.transactions[] | .amount | sum()' huge.toon --stream

# Parallel aggregation
tq '.events | group_by(.type) | count()' data.toon --parallel 16
```

### SQL-Like Operations ( Planned Phase 4)

```bash
# Complex analytics
.sales | group_by(.region) | {
  region: .[0].region,
  total: map(.amount) | add,
  avg: map(.amount) | add / length
}

# Filtering with where
.users | where(.age > 18 and .active == true)

# Sorting with multiple keys
.products | sort_by(.category, .price)

# Joins
.orders | join(.customers; .customer_id; .id)
```

---

## API Reference

### C++ Core API

```cpp
namespace tq {

// High-level query API
std::vector<std::string> query(
    const std::string& expression,
    const std::string& json_data
);

std::vector<Value> query_values(
    const std::string& expression,
    const Value& data
);

// Value types
class Value {
public:
    enum class Type { Null, Bool, Int, Float, String, Array, Object };
    
    Type type() const;
    bool is_null() const;
    bool is_bool() const;
    bool is_number() const;
    bool is_string() const;
    bool is_array() const;
    bool is_object() const;
    
    bool as_bool() const;
    int64_t as_int() const;
    double as_float() const;
    const std::string& as_string() const;
    const std::vector<Value>& as_array() const;
    const Value* get(const std::string& key) const;
    
    std::string to_json(bool pretty = false) const;
};

}
```

### Python API

```python
import pytq

# Query Python objects
result = pytq.query(expression: str, data: dict|list) -> list

# Query JSON strings
result = pytq.query_json(expression: str, json_str: str) -> list

# Configuration (future)
pytq.set_config(parallel=True, stream=True)
```

### CLI Options

```bash
# Output formatting
-c, --compact-output         Compact JSON output
-p, --pretty                 Pretty-print JSON
-r, --raw-output             Raw string output (no quotes)
-j, --join-output            No newlines after output

# Input options
-n, --null-input             Don't read input
-s, --slurp                  Read entire input into array
-R, --raw-input              Each line as string

# Processing
-b, --benchmark              Show timing information
--stream                     Streaming mode (Phase 5)
--parallel N                 Use N threads (Phase 5)

# Format conversion (Phase 5)
--input-format FORMAT        Input: json|toon|csv
--output-format FORMAT       Output: json|toon|csv|table

# Help & version
-h, --help                   Show help
-v, --version                Show version
```

---

## Performance

### Benchmarks

Current performance (Phase 1-2, sample data):

| Operation | Time | Notes |
|-----------|------|-------|
| Field access | ~0.01ms | `.field` |
| Array iteration | ~0.02ms | `.[]` |
| Fan-out query | ~0.05ms | `.users[].email` |
| Array indexing | ~0.01ms | `.[0]` |
| Pipe operations | ~0.03ms | `.a \| .b \| .c` |

### Optimization Tips

1. **Use streaming for large files** (Phase 5)
   ```bash
   tq '.large_array[]' --stream
   ```

2. **Parallelize when possible** (Phase 5)
   ```bash
   tq 'group_by(.key)' --parallel 8
   ```

3. **Select columns early** (Phase 3)
   ```bash
   # Good: Select columns first
   .users{name,email} | .[]
   
   # Bad: Iterate then select
   .users[] | {name,email}
   ```

4. **Use native TOON format** (Phase 5)
   - TOON's columnar structure is faster than JSON

---

## Roadmap & Status

###  Phase 1: Basic Queries (COMPLETED)

- [x] Field access: `.field`, `.nested.field`
- [x] Array iteration: `.[]`, `.field[]`
- [x] Fan-out queries: `.users[].email`
- [x] C++ core with RAII
- [x] CLI tool
- [x] Python bindings
- [x] Comprehensive tests

###  Phase 2: jq Core Features (IN PROGRESS)

#### Completed:
- [x] Pipe operator: `|`
- [x] Array indexing: `.[0]`, `.[-1]`
- [x] Array slicing: `.[1:5]`

#### In Progress:
- [ ] Comma operator: `,`
- [ ] Comparison: `==`, `!=`, `<`, `>`, `<=`, `>=`
- [ ] Arithmetic: `+`, `-`, `*`, `/`, `%`
- [ ] Logical: `and`, `or`, `not`, `//`
- [ ] Conditionals: `if-then-else`
- [ ] `select()` and `map()`
- [ ] Type functions

###  Phase 3: TOON Power Features (PLANNED)

- [ ] Column projections: `.users{name,email}`
- [ ] Row expressions with computed fields
- [ ] String operations: `split`, `join`, `test`, etc.
- [ ] Object construction: `{name: .x, age: .y}`
- [ ] Array functions: `sort`, `unique`, `group_by`
- [ ] Basic aggregations: `sum`, `avg`, `min`, `max`
- [ ] Regular expressions
- [ ] Variables and functions

###  Phase 4: Analytics (PLANNED)

- [ ] Advanced grouping and aggregation
- [ ] Multi-key sorting
- [ ] SQL-like `where()` filtering
- [ ] Joins and relationships
- [ ] Statistical functions
- [ ] `reduce` and `foreach`

###  Phase 5: Big Data (PLANNED)

- [ ] Streaming mode: `--stream`
- [ ] Parallel processing: `--parallel`
- [ ] Format conversion: CSV, Parquet, Arrow
- [ ] SIMD optimizations
- [ ] Memory-mapped I/O

###  Phase 6: Advanced (PLANNED)

- [ ] User-defined functions
- [ ] Module system
- [ ] Interactive REPL
- [ ] Query profiling
- [ ] VSCode extension
- [ ] Web playground (WASM)

---

## Examples

### Basic Queries

```bash
# Get all user names
tq '.users[].name' users.json

# First user's email
tq '.users[0].email' users.json

# Last 3 items
tq '.items[-3:]' data.json

# Pipe operations
tq '.data | .users | .[0] | .name' complex.json
```

### Real-World Examples

```bash
# Extract emails from user list
tq '.users[].email' contacts.json > emails.txt

# Get product names and prices
tq '.products[] | {name: .name, price: .price}' catalog.json

# Find users in specific region
tq '.users[] | select(.region == "US")' users.json

# Calculate total revenue
tq '.sales[] | .amount | add' sales.json

# Group by category and count
tq '.items | group_by(.category) | map({category: .[0].category, count: length})' inventory.json
```

---

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for development guidelines.

Key areas:
1. **jq feature parity** - Implement missing jq functions
2. **TOON-specific features** - Column operations, streaming
3. **Performance** - Optimize hot paths
4. **Documentation** - Examples and tutorials
5. **Testing** - Comprehensive test coverage

---

## Resources

- **GitHub**: https://github.com/yourusername/tq
- **Issues**: https://github.com/yourusername/tq/issues
- **jq Manual**: https://jqlang.org/manual/
- **TOON Spec**: https://github.com/toon-format/spec

---

## License

MIT License - see [LICENSE](../LICENSE) file

---

**TQ** - Making structured data querying elegant and powerful.
