# TQ Examples

Comprehensive examples demonstrating TQ query capabilities.

## Table of Contents

- [Basic Queries](#basic-queries)
- [Array Queries](#array-queries)
- [Nested Structures](#nested-structures)
- [Real-World Examples](#real-world-examples)
- [Python Integration](#python-integration)
- [C++ Integration](#c-integration)

## Basic Queries

### Simple Field Access

**Data:**
```json
{
  "name": "Alice",
  "age": 30,
  "active": true
}
```

**Queries:**
```bash
tq '.name' data.json      # "Alice"
tq '.age' data.json       # 30
tq '.active' data.json    # true
```

### Non-Existent Fields

```bash
tq '.missing' data.json   # (no output - empty result)
```

## Array Queries

### Simple Array Iteration

**Data:**
```json
{
  "items": [1, 2, 3, 4, 5]
}
```

**Query:**
```bash
tq '.items[]' data.json
# Output:
# 1
# 2
# 3
# 4
# 5
```

### Array of Objects

**Data:**
```json
{
  "users": [
    {"id": 1, "name": "Alice"},
    {"id": 2, "name": "Bob"},
    {"id": 3, "name": "Carol"}
  ]
}
```

**Queries:**
```bash
# Get all users
tq '.users[]' data.json

# Get all names
tq '.users[].name' data.json
# Output:
# "Alice"
# "Bob"
# "Carol"

# Get all IDs
tq '.users[].id' data.json
# Output:
# 1
# 2
# 3
```

## Nested Structures

### Nested Objects

**Data:**
```json
{
  "company": {
    "name": "Tech Corp",
    "address": {
      "city": "San Francisco",
      "state": "CA"
    }
  }
}
```

**Queries:**
```bash
tq '.company.name' data.json           # "Tech Corp"
tq '.company.address.city' data.json   # "San Francisco"
tq '.company.address.state' data.json  # "CA"
```

### Nested Arrays

**Data:**
```json
{
  "departments": [
    {
      "name": "Engineering",
      "employees": [
        {"name": "Alice", "role": "Developer"},
        {"name": "Bob", "role": "Manager"}
      ]
    },
    {
      "name": "Sales",
      "employees": [
        {"name": "Carol", "role": "Rep"}
      ]
    }
  ]
}
```

**Queries:**
```bash
# Get all department names
tq '.departments[].name' data.json
# Output:
# "Engineering"
# "Sales"

# Get all employee names (fan-out)
tq '.departments[].employees[].name' data.json
# Output:
# "Alice"
# "Bob"
# "Carol"

# Get all employee roles
tq '.departments[].employees[].role' data.json
# Output:
# "Developer"
# "Manager"
# "Rep"
```

## Real-World Examples

### User Database

**Data:**
```json
{
  "users": [
    {
      "id": 1,
      "username": "alice",
      "email": "alice@example.com",
      "profile": {
        "firstName": "Alice",
        "lastName": "Johnson",
        "age": 30
      },
      "tags": ["admin", "active"]
    },
    {
      "id": 2,
      "username": "bob",
      "email": "bob@example.com",
      "profile": {
        "firstName": "Bob",
        "lastName": "Smith",
        "age": 25
      },
      "tags": ["user"]
    }
  ]
}
```

**Queries:**
```bash
# Extract all emails
tq '.users[].email' users.json

# Get all usernames
tq '.users[].username' users.json

# Get all first names
tq '.users[].profile.firstName' users.json

# Get all ages
tq '.users[].profile.age' users.json

# Get all tags (nested arrays)
tq '.users[].tags[]' users.json
# Output:
# "admin"
# "active"
# "user"
```

### E-commerce Orders

**Data:**
```json
{
  "orders": [
    {
      "orderId": "ORD-001",
      "customer": "Alice",
      "items": [
        {"sku": "WIDGET-1", "quantity": 2, "price": 9.99},
        {"sku": "GADGET-2", "quantity": 1, "price": 14.50}
      ],
      "total": 34.48
    },
    {
      "orderId": "ORD-002",
      "customer": "Bob",
      "items": [
        {"sku": "WIDGET-1", "quantity": 1, "price": 9.99}
      ],
      "total": 9.99
    }
  ]
}
```

**Queries:**
```bash
# Get all order IDs
tq '.orders[].orderId' orders.json

# Get all customer names
tq '.orders[].customer' orders.json

# Get all SKUs (fan-out through nested arrays)
tq '.orders[].items[].sku' orders.json
# Output:
# "WIDGET-1"
# "GADGET-2"
# "WIDGET-1"

# Get all prices
tq '.orders[].items[].price' orders.json
# Output:
# 9.99
# 14.5
# 9.99

# Get all totals
tq '.orders[].total' orders.json
# Output:
# 34.48
# 9.99
```

### API Response

**Data:**
```json
{
  "status": "success",
  "data": {
    "users": [
      {"id": 1, "name": "Alice"},
      {"id": 2, "name": "Bob"}
    ]
  },
  "meta": {
    "page": 1,
    "pageSize": 10,
    "total": 2
  }
}
```

**Queries:**
```bash
# Get status
tq '.status' response.json

# Get total count
tq '.meta.total' response.json

# Extract user data
tq '.data.users[].name' response.json
# Output:
# "Alice"
# "Bob"
```

## Python Integration

### Basic Usage

```python
import pytq
import json

# Load data
with open('data.json', 'r') as f:
    data = json.load(f)

# Query
results = pytq.query('.users[].email', data)
print(results)
```

### Processing Results

```python
import pytq

data = {
    "products": [
        {"name": "Widget", "price": 9.99, "stock": 100},
        {"name": "Gadget", "price": 14.50, "stock": 50},
        {"name": "Doohickey", "price": 7.25, "stock": 200}
    ]
}

# Get all prices
prices = pytq.query('.products[].price', data)
total = sum(prices)
average = total / len(prices)

print(f"Total: ${total:.2f}")
print(f"Average: ${average:.2f}")

# Get all names
names = pytq.query('.products[].name', data)
for name in names:
    print(f"- {name}")
```

### Dynamic Query Building

```python
import pytq

def extract_fields(data, path):
    """Extract fields from nested data structure"""
    query = f'.{".".join(path)}'
    return pytq.query(query, data)

data = {
    "company": {
        "employees": {
            "engineering": [
                {"name": "Alice", "salary": 100000},
                {"name": "Bob", "salary": 120000}
            ]
        }
    }
}

# Build query dynamically
path = ["company", "employees", "engineering"]
names = extract_fields(data, path + ["[]", "name"])
print(names)  # ['Alice', 'Bob']
```

## C++ Integration

### Basic Usage

```cpp
#include "tq/tq.hpp"
#include <iostream>
#include <fstream>

int main() {
    // Load JSON file
    std::ifstream file("data.json");
    std::string json((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>());
    
    // Execute query
    auto results = tq::query(".users[].email", json);
    
    // Process results
    for (const auto& result : results) {
        std::cout << result << "\n";
    }
    
    return 0;
}
```

### Working with Value Objects

```cpp
#include "tq/tq.hpp"
#include <iostream>

int main() {
    // Build data programmatically
    std::vector<tq::Value> users;
    
    std::map<std::string, tq::Value> alice;
    alice["name"] = tq::Value("Alice");
    alice["email"] = tq::Value("alice@example.com");
    users.push_back(tq::Value(std::move(alice)));
    
    std::map<std::string, tq::Value> bob;
    bob["name"] = tq::Value("Bob");
    bob["email"] = tq::Value("bob@example.com");
    users.push_back(tq::Value(std::move(bob)));
    
    std::map<std::string, tq::Value> root;
    root["users"] = tq::Value(std::move(users));
    tq::Value data(std::move(root));
    
    // Query
    auto results = tq::query_values(".users[].email", data);
    
    for (const auto& result : results) {
        std::cout << result.as_string() << "\n";
    }
    
    return 0;
}
```

### Error Handling

```cpp
#include "tq/tq.hpp"
#include <iostream>

int main() {
    try {
        std::string json = R"({"invalid": json})";
        auto results = tq::query(".field", json);
    } catch (const std::runtime_error& e) {
        std::cerr << "Query failed: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
```

## Performance Examples

### Benchmarking

```bash
# Benchmark a query
tq -b '.users[].email' large_file.json

# Output:
# (results...)
# 
# Execution time: 2.34 ms
# Results: 1000
```

### Optimization Tips

```python
import pytq
import time

# BAD: Query in a loop
data = {"items": [{"id": i} for i in range(1000)]}
start = time.time()
for i in range(1000):
    result = pytq.query(f'.items[{i}].id', data)  # Don't do this!
print(f"Loop: {(time.time() - start) * 1000:.2f} ms")

# GOOD: Single fan-out query
start = time.time()
results = pytq.query('.items[].id', data)
print(f"Fan-out: {(time.time() - start) * 1000:.2f} ms")
```

## Edge Cases

### Empty Arrays

```json
{"items": []}
```

```bash
tq '.items[]' data.json
# (no output - empty result)
```

### Null Values

```json
{"name": null, "age": 30}
```

```bash
tq '.name' data.json
# null
```

### Missing Fields

```json
{"name": "Alice"}
```

```bash
tq '.age' data.json
# (no output - empty result)
```

### Mixed Types

```json
{"items": [1, "two", true, null, {"nested": "value"}]}
```

```bash
tq '.items[]' data.json
# 1
# "two"
# true
# null
# {"nested":"value"}
```
