# TQ Roadmap: jq for TOON

**Vision**: TQ is to TOON what jq is to JSON  a powerful, elegant query language for structured data.

But because **TOON is columnar + tabular**, TQ can be **more powerful** than jq when querying structured tables.

---

##  Core Philosophy

```
TQ = jq semantics + SQL-like table handling + TOON-native performance
```

### Why TQ is Different from jq

| Feature             | jq               | tq                         |
| ------------------- | ---------------- | -------------------------- |
| Input format        | JSON             | **TOON** (+ JSON)          |
| Data model          | Tree             | **Table + Tree Hybrid**    |
| Row blocks          |  no             | ** users[100]{name,age}** |
| Column selection    | requires mapping | **native**                 |
| Streaming rows      | manual           | **built-in**               |
| Analytics possible? | cumbersome       | **potentially SQL-like**   |

### Core TQ Philosophy

```bash
# Simple path queries like jq
.users[].email

# Structured table awareness
.users{name,age}[age > 18]

# Streaming over rows natively
.users[].balance | sum()

# Column slicing
.users[*].name

# Index/cell access
.users[3].name

# TOON-native import/export (no JSON needed)
tq ".transactions[]" data.toon --output results.toon
```

---

##  Feature Implementation Roadmap

###  Phase 1: Basic Path Queries (COMPLETED)

**Status**: Fully implemented and tested

- [x] `.field` - Field access
- [x] `.field.subfield` - Nested field access
- [x] `.field[]` - Array iteration
- [x] `.field[].child` - Fan-out queries
- [x] Type-safe value representation
- [x] C++ core library with RAII
- [x] CLI tool with file/stdin support
- [x] Python bindings via pybind11
- [x] Comprehensive test suite

**Examples Working**:
```bash
tq '.users[].email' sample.json
# Output: "alice@example.com", "bob@example.com", "carol@example.com"
```

---

###  Phase 2: jq-Compatible Operators (IN PROGRESS)

**Goal**: Implement all core jq features for full compatibility

#### 2.1 Pipes & Composition
- [ ] Pipe operator: `.users | .[].email`
- [ ] Multiple pipes: `.data | .users | .[0]`
- [ ] Parenthesized expressions: `(.a | .b), (.c | .d)`

#### 2.2 Array Indexing & Slicing
- [ ] Integer index: `.users[0]`, `.items[-1]`
- [ ] Array slices: `.users[1:10]`, `.items[5:]`, `.data[:3]`
- [ ] Slice with step: `.items[::2]`
- [ ] Negative indices: `.users[-3:-1]`

#### 2.3 Selection & Filtering
- [ ] `select()`: `.users[] | select(.age > 18)`
- [ ] `map()`: `.users | map(.email)`
- [ ] `empty`: Remove null/empty values
- [ ] Boolean operators: `and`, `or`, `not`

#### 2.4 Conditional Expressions
- [ ] `if-then-else`: `if .age > 18 then "adult" else "minor" end`
- [ ] Alternative operator: `.name // "unknown"`
- [ ] Optional access: `.user?.email`

#### 2.5 Object Construction
- [ ] Object literals: `{name: .firstName, age: .userAge}`
- [ ] Array literals: `[.a, .b, .c]`
- [ ] Computed keys: `{(.key): .value}`

#### 2.6 String Operations
- [ ] String interpolation: `"Hello \(.name)"`
- [ ] String concatenation: `.first + " " + .last`
- [ ] `split()`, `join()`, `trim()`
- [ ] Regular expressions: `test()`, `match()`, `capture()`

#### 2.7 Type Operations
- [ ] `type`: Return value type
- [ ] Type checking: `isnumber`, `isstring`, `isarray`, `isobject`, `isboolean`, `isnull`
- [ ] Type conversions: `tonumber`, `tostring`, `toarray`

#### 2.8 Arithmetic & Comparison
- [ ] Arithmetic: `+`, `-`, `*`, `/`, `%`
- [ ] Comparison: `==`, `!=`, `<`, `<=`, `>`, `>=`
- [ ] Math functions: `floor`, `ceil`, `round`, `sqrt`, `pow`

---

###  Phase 3: TOON-Specific Power Features

**Goal**: Leverage TOON's columnar/tabular nature for advanced queries

#### 3.1 Column Projections
- [ ] Select specific columns: `.users{name,email}`
- [ ] Exclude columns: `.users{-password,-ssn}`
- [ ] Computed columns: `.users{name, senior: .age > 65}`
- [ ] Nested projections: `.orders{id, customer{name,email}}`

#### 3.2 Row Expressions
- [ ] Concatenation: `.users[] | .name + " <" + .email + ">"`
- [ ] Multi-field expressions: `.users[] | {fullInfo: .name + " (" + .age + ")"}`
- [ ] Conditional formatting: `.users[] | if .active then .name else "-" end`

#### 3.3 Computed Fields
- [ ] Arithmetic on fields: `.users[].age + 1`
- [ ] String operations: `.users[].name | uppercase`
- [ ] Date/time calculations: `.events[].timestamp | format_date`
- [ ] Field derivations: `.products[] | {price, taxed: .price * 1.2}`

#### 3.4 Basic Aggregations
- [ ] `count()`: Count elements
- [ ] `sum()`: Sum numeric values
- [ ] `avg()`: Average
- [ ] `min()`, `max()`: Min/max values
- [ ] `first()`, `last()`: First/last element
- [ ] `unique()`: Unique values
- [ ] `group_by()`: Group rows by key

**Examples**:
```bash
# Average age
tq '.users[] | .age | avg()' users.toon

# Count by status
tq '.users | group_by(.status) | count()' users.toon
```

---

###  Phase 4: Full Analytic Mode (SQL-LIKE)

**Goal**: Make TQ a legitimate alternative to SQL for data analysis

#### 4.1 Advanced Grouping & Aggregation
- [ ] `group_by()` with multiple keys: `group_by(.country, .city)`
- [ ] Aggregation per group: `group_by(.dept) | {dept: .[0].dept, avg: map(.salary) | avg()}`
- [ ] `reduce()`: Custom aggregation logic

#### 4.2 Sorting & Ordering
- [ ] `sort()`: Natural sort
- [ ] `sort_by()`: Sort by field(s)
- [ ] `reverse()`: Reverse order
- [ ] Multi-key sorting: `sort_by(.age, .name)`

#### 4.3 Filtering & Conditions
- [ ] `where()`: SQL-like filtering: `.users | where(.score > 0.9)`
- [ ] Complex conditions: `where(.age > 18 and .active == true)`
- [ ] `limit()`: Limit results
- [ ] `offset()`: Skip results

#### 4.4 Joins & Relationships
- [ ] `join()`: Join arrays by key
- [ ] `inner_join()`, `left_join()`: SQL-style joins
- [ ] `lookup()`: Fast key-based lookup

#### 4.5 Statistical Functions
- [ ] `stddev()`, `variance()`: Standard deviation, variance
- [ ] `median()`: Median value
- [ ] `percentile()`: Nth percentile
- [ ] `histogram()`: Binning and histograms

**Examples**:
```bash
# Top 10 earners by department
tq '.employees | group_by(.dept) | sort_by(.salary) | reverse | limit(10)' data.toon

# Complex analytics
tq '.sales | where(.amount > 1000) | group_by(.region) | {region, total: sum(.amount), avg: avg(.amount)}' sales.toon
```

---

###  Phase 5: Streaming Big Data Mode

**Goal**: Handle massive datasets with constant memory usage

#### 5.1 Streaming Operations
- [ ] `--stream` flag: Process rows one-at-a-time
- [ ] Lazy evaluation: Never load full dataset into memory
- [ ] Backpressure handling: Efficient producer-consumer pattern

#### 5.2 Parallel Processing
- [ ] Multi-threaded evaluation: `--parallel N`
- [ ] SIMD optimizations: Vectorized operations on numeric data
- [ ] GPU offload: For massive aggregations (future)

#### 5.3 Format Conversions
- [ ] TOON  JSON: Seamless conversion
- [ ] TOON  CSV: Direct CSV parsing/writing
- [ ] TOON  Parquet: Columnar format interop
- [ ] TOON  Arrow: Zero-copy integration

**Examples**:
```bash
# Stream process 100GB file with constant memory
tq ".transactions[] | .amount" huge_file.toon --stream > amounts.txt

# Parallel aggregation
tq ".events | group_by(.type) | count()" massive.toon --parallel 16

# Convert formats
tq ".[]" data.csv --input-format csv --output-format toon > data.toon
```

---

###  Phase 6: Advanced Features

#### 6.1 User-Defined Functions
- [ ] Define functions: `def double: . * 2;`
- [ ] Recursive functions: `def factorial: if . <= 1 then 1 else . * ((. - 1) | factorial) end;`
- [ ] Function libraries: Import common functions

#### 6.2 Module System
- [ ] `import "lib/stats.tq"`: Import query modules
- [ ] Standard library: Common functions and patterns
- [ ] User library path: Custom function repositories

#### 6.3 Advanced I/O
- [ ] Multiple input files: `tq ".[]" file1.toon file2.toon`
- [ ] Output formatting: `--output-format [json|toon|csv|table]`
- [ ] Compression: Auto-detect and handle `.toon.gz`, `.toon.zst`

#### 6.4 Interactive Mode
- [ ] REPL: Interactive query development
- [ ] Query debugging: Step-by-step evaluation
- [ ] Query profiling: Performance analysis

---

##  Path to Fame & Adoption

### Current Status: **Step 3/8** 

| Step | Description | Status |
|------|-------------|--------|
| 1 |  Fully support TOON spec | **DONE** (Phase 1) |
| 2 |  Provide CLI binary | **DONE** (Windows/Linux/macOS) |
| 3 |  Python package | **DONE** (`pip install tq`) |
| 4 |  Blog post "Introducing TQ" | **TODO** |
| 5 |  Benchmarks vs jq | **TODO** |
| 6 |  Examples gallery | **TODO** |
| 7 |  VSCode extension | **TODO** |
| 8 |  Web playground (WASM) | **TODO** |

---

##  Why This Matters

Nobody has:

 **A native TOON query engine**  
 **Implemented in C++ (fast)**  
 **pip + CLI runnable**  
 **Spec-aware, future-proof**  
 **Built like jq but table-smart**

You're entering a space **before it explodes**, like:
- YAML in 2001
- TOML in 2013
- jq in 2013 (filled JSON's query gap)

**TQ is filling that gap for TOON.**

---

##  Resources

- [jq Manual](https://jqlang.github.io/jq/manual/) - Reference for feature parity
- [TOON Spec](https://github.com/toon-format/spec) - Official TOON specification
- [DuckDB](https://duckdb.org/) - Inspiration for analytics features
- [Polars](https://pola.rs/) - Inspiration for streaming/columnar operations

---

##  Contributing

See individual phase docs in `docs/PHASE_*.md` for implementation details.

For each phase:
1. Review the feature list
2. Implement in C++ core
3. Add comprehensive tests
4. Update CLI and Python bindings
5. Document with examples

**Let's make TQ the standard for querying structured data!**
