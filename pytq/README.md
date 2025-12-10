# pytq - Python Bindings for TQ

Python bindings for the TQ query engine, enabling jq-style queries on Python data structures.

## Installation

```bash
pip install pytq
```

Or build from source:

```bash
cd pytq
pip install -e .
```

## Usage

```python
import pytq

# Query Python data structures
data = {
    "users": [
        {"name": "Alice", "email": "alice@example.com"},
        {"name": "Bob", "email": "bob@example.com"}
    ]
}

results = pytq.query(".users[].email", data)
print(results)  # ['alice@example.com', 'bob@example.com']

# Query JSON strings
json_data = '{"name": "Alice", "age": 30}'
results = pytq.query_json(".name", json_data)
print(results)  # ['"Alice"']
```

## API

### `query(expression, data)`

Query Python data structures.

- **expression** (str): TQ query expression (e.g., `.users[].email`)
- **data**: Python dict, list, or primitive value
- **Returns**: list of query results as Python objects

### `query_json(expression, json_data)`

Query JSON strings.

- **expression** (str): TQ query expression
- **json_data** (str): JSON string
- **Returns**: list of query results as JSON strings

## Requirements

- Python >= 3.7
- pybind11 >= 2.10.0
- CMake >= 3.20
- C++20 compiler

## License

MIT
