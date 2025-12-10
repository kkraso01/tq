# TQ API Documentation

## C++ API

### High-Level API (`tq.hpp`)

#### `std::vector<std::string> query(const std::string& expression, const std::string& data)`

Execute a TQ query on JSON/TOON data.

**Parameters:**
- `expression`: TQ query expression (e.g., `".users[].email"`)
- `data`: JSON or TOON string to query

**Returns:** Vector of JSON strings representing query results

**Example:**
```cpp
#include "tq/tq.hpp"

std::string json = R"({"name":"Alice","age":30})";
auto results = tq::query(".name", json);
// results = ["\"Alice\""]
```

#### `std::vector<Value> query_values(const std::string& expression, const Value& data)`

Execute a TQ query on a Value object.

**Parameters:**
- `expression`: TQ query expression
- `data`: Value object to query

**Returns:** Vector of Value objects

**Example:**
```cpp
tq::Value data = tq::Value::from_json(json_string);
auto results = tq::query_values(".users[].email", data);
```

### Value API (`value.hpp`)

#### Constructors

```cpp
Value();                                    // null
Value(bool b);                             // boolean
Value(double d);                           // number
Value(int i);                              // number
Value(const std::string& s);               // string
Value(std::vector<Value>&& arr);           // array
Value(std::map<std::string, Value>&& obj); // object
```

#### Type Checking

```cpp
bool is_null() const;
bool is_boolean() const;
bool is_number() const;
bool is_string() const;
bool is_array() const;
bool is_object() const;
Type type() const;  // enum Type { Null, Boolean, Number, String, Array, Object }
```

#### Getters

```cpp
bool as_boolean() const;                          // throws if not boolean
double as_number() const;                         // throws if not number
const std::string& as_string() const;             // throws if not string
const std::vector<Value>& as_array() const;       // throws if not array
const std::map<std::string, Value>& as_object() const; // throws if not object
```

#### Safe Access

```cpp
const Value* get(const std::string& key) const;  // nullptr if key not found
const Value* get(size_t index) const;            // nullptr if out of bounds
```

#### Serialization

```cpp
std::string to_json(bool pretty = false, int indent = 0) const;
static Value from_json(const std::string& json);
```

**Example:**
```cpp
// Build a Value object
std::map<std::string, tq::Value> obj;
obj["name"] = tq::Value("Alice");
obj["age"] = tq::Value(30);
tq::Value data(std::move(obj));

// Serialize
std::string json = data.to_json(true);

// Parse
tq::Value parsed = tq::Value::from_json(json);
```

### Lexer API (`lexer.hpp`)

```cpp
class Lexer {
public:
    explicit Lexer(std::string query);
    std::vector<Token> tokenize();
};

enum class TokenType { Dot, Identifier, LeftBracket, RightBracket, End };

struct Token {
    TokenType type;
    std::string value;  // only for Identifier
    size_t position;
};
```

### Parser API (`parser.hpp`)

```cpp
class Parser {
public:
    explicit Parser(std::vector<Token> tokens);
    std::vector<Step> parse();
};

struct Step {
    enum class Type { Field, Iterate };
    Type type;
    std::string field_name;  // only for Field type
};
```

### Evaluator API (`evaluator.hpp`)

```cpp
class Evaluator {
public:
    std::vector<Value> evaluate(const std::vector<Step>& steps, const Value& data);
};
```

## Python API

### `pytq.query(expression, data)`

Query Python data structures.

**Parameters:**
- `expression` (str): TQ query expression
- `data`: Python dict, list, or primitive

**Returns:** list of Python objects

**Example:**
```python
import pytq

data = {"users": [{"email": "a@b.com"}, {"email": "c@d.com"}]}
results = pytq.query(".users[].email", data)
# results = ['a@b.com', 'c@d.com']
```

### `pytq.query_json(expression, json_data)`

Query JSON strings.

**Parameters:**
- `expression` (str): TQ query expression
- `json_data` (str): JSON string

**Returns:** list of JSON strings

**Example:**
```python
import pytq

json_str = '{"name": "Alice", "age": 30}'
results = pytq.query_json(".name", json_str)
# results = ['"Alice"']
```

## CLI Usage

```
tq [OPTIONS] <expression> [file]

Options:
  -c, --compact      Compact JSON output (default)
  -p, --pretty       Pretty-print JSON output
  -b, --benchmark    Show execution time
  -h, --help         Show help message

Examples:
  tq '.name' data.json
  tq '.users[].email' data.toon
  cat data.json | tq '.items[].price'
  tq -p '.data' input.json
```

## Query Expression Syntax

| Expression | Description |
|------------|-------------|
| `.field` | Access object field named "field" |
| `.a.b.c` | Access nested field: data.a.b.c |
| `.arr[]` | Iterate over array elements |
| `.arr[].field` | Iterate and access field on each element |

### Evaluation Semantics

1. **Field access** (`.field`): 
   - If current value is an object, return the value of `field`
   - Otherwise, return empty result

2. **Iteration** (`[]`):
   - If current value is an array, return all elements
   - Otherwise, return empty result

3. **Fan-out**: When a step produces multiple results, subsequent steps apply to each result independently

**Example:**
```
.users[].email

Steps:
1. .users  get "users" field (array)
2. []  iterate array (3 objects)
3. .email  get "email" from each object (fan-out)

Result: ["alice@example.com", "bob@example.com", "carol@example.com"]
```

## Error Handling

### C++

All errors throw exceptions:

- `std::runtime_error` for runtime errors
- `tq::LexerError` for lexing errors
- `tq::ParseError` for parsing errors

**Example:**
```cpp
try {
    auto results = tq::query(".field", json);
} catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
}
```

### Python

Errors raise Python exceptions:

```python
try:
    results = pytq.query(".field", data)
except Exception as e:
    print(f"Error: {e}")
```

## Memory Management

All C++ components use RAII:

- `std::string` for strings
- `std::vector<T>` for arrays
- `std::map<K, V>` for objects
- `std::variant` for discriminated unions
- `std::unique_ptr` and `std::shared_ptr` where needed

**No manual memory management required.** All resources are automatically cleaned up.

## Performance Tips

1. **Use move semantics**: Construct Value objects with `std::move()` to avoid copies
2. **Reuse evaluators**: Create one Evaluator and reuse for multiple queries
3. **Batch queries**: Process multiple items with fan-out rather than individual queries
4. **Profile**: Use the `-b` flag in CLI or benchmark tools to identify bottlenecks

## Thread Safety

- **Value objects**: Not thread-safe. Don't share across threads without synchronization.
- **Evaluator**: Not thread-safe. Create one per thread or use synchronization.
- **Immutable operations**: `query()` and `query_values()` can be called from multiple threads on different data.

## Building Custom Tools

### Embedding TQ

Link against `libtq_core`:

```cmake
find_library(TQ_CORE tq_core)
target_link_libraries(my_app ${TQ_CORE})
```

### Extending the Parser

To add new operators:

1. Add token types to `lexer.hpp`
2. Update `Lexer::tokenize()` to recognize new tokens
3. Add step types to `parser.hpp`
4. Update `Parser::parse()` to handle new syntax
5. Implement evaluation in `Evaluator::apply_step()`
