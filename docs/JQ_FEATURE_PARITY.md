# jq Feature Parity Tracking

This document tracks TQ's implementation status for all jq features, ensuring full compatibility.

Reference: [jq Manual v1.7](https://jqlang.github.io/jq/manual/v1.7/)

---

## Legend

- ✅ **Implemented** - Feature complete and tested
- 🚧 **In Progress** - Currently being implemented
- 📋 **Planned** - Scheduled for implementation
- ⏳ **Deferred** - Lower priority, future version
- ❌ **Not Applicable** - Doesn't apply to TOON context

---

## Basic Filters

| Feature | jq Syntax | Status | TQ Equivalent | Notes |
|---------|-----------|--------|---------------|-------|
| Identity | `.` | ✅ | `.` | Returns input unchanged |
| Field access | `.foo` | ✅ | `.foo` | Access object field |
| Nested fields | `.foo.bar` | ✅ | `.foo.bar` | Deep field access |
| Optional field | `.foo?` | 📋 | `.foo?` | No error if field missing |
| Array index | `.[0]` | 📋 | `.[0]` | Access array element |
| Array slice | `.[2:4]` | 📋 | `.[2:4]` | Slice array |
| Iterator | `.[]` | ✅ | `.[]` | Iterate array/object values |
| Recursive descent | `..` | 📋 | `..` | All values recursively |

---

## Pipe & Composition

| Feature | jq Syntax | Status | TQ Equivalent | Notes |
|---------|-----------|--------|---------------|-------|
| Pipe | `\| ` | 📋 | `\| ` | Chain operations |
| Comma | `,` | 📋 | `,` | Multiple outputs |
| Parentheses | `()` | 📋 | `()` | Grouping |

**Examples**:
```bash
# jq
jq '.users[] | .email' data.json

# tq (planned)
tq '.users[] | .email' data.toon
```

---

## Types & Values

| Feature | jq Syntax | Status | TQ Equivalent | Notes |
|---------|-----------|--------|---------------|-------|
| `type` | `type` | 📋 | `type` | Get value type |
| `length` | `length` | 📋 | `length` | Get length |
| `keys` | `keys` | 📋 | `keys` | Object keys |
| `keys_unsorted` | `keys_unsorted` | 📋 | `keys_unsorted` | Unsorted keys |
| `values` | `values` | 📋 | `values` | Object values |
| `empty` | `empty` | 📋 | `empty` | Empty sequence |
| `null` | `null` | ✅ | `null` | Null value |

---

## Array Operations

| Feature | jq Syntax | Status | TQ Equivalent | Notes |
|---------|-----------|--------|---------------|-------|
| `add` | `add` | 📋 | `add` | Sum array |
| `reverse` | `reverse` | 📋 | `reverse` | Reverse array |
| `sort` | `sort` | 📋 | `sort` | Sort array |
| `sort_by` | `sort_by(.x)` | 📋 | `sort_by(.x)` | Sort by expression |
| `group_by` | `group_by(.x)` | 📋 | `group_by(.x)` | Group by expression |
| `unique` | `unique` | 📋 | `unique` | Unique values |
| `unique_by` | `unique_by(.x)` | 📋 | `unique_by(.x)` | Unique by expression |
| `min` | `min` | 📋 | `min` | Minimum value |
| `max` | `max` | 📋 | `max` | Maximum value |
| `min_by` | `min_by(.x)` | 📋 | `min_by(.x)` | Min by expression |
| `max_by` | `max_by(.x)` | 📋 | `max_by(.x)` | Max by expression |
| `indices` | `indices(x)` | 📋 | `indices(x)` | Find indices |
| `index` | `index(x)` | 📋 | `index(x)` | Find first index |
| `rindex` | `rindex(x)` | 📋 | `rindex(x)` | Find last index |
| `inside` | `inside(x)` | 📋 | `inside(x)` | Check containment |
| `contains` | `contains(x)` | 📋 | `contains(x)` | Check contains |
| `flatten` | `flatten` | 📋 | `flatten` | Flatten nested arrays |
| `range` | `range(n)` | 📋 | `range(n)` | Generate range |

---

## Object Operations

| Feature | jq Syntax | Status | TQ Equivalent | Notes |
|---------|-----------|--------|---------------|-------|
| `to_entries` | `to_entries` | 📋 | `to_entries` | Convert to key-value pairs |
| `from_entries` | `from_entries` | 📋 | `from_entries` | Convert from key-value pairs |
| `with_entries` | `with_entries(f)` | 📋 | `with_entries(f)` | Transform entries |
| `has` | `has("key")` | 📋 | `has("key")` | Check key existence |
| `in` | `in({...})` | 📋 | `in({...})` | Check membership |
| `del` | `del(.x)` | 📋 | `del(.x)` | Delete field |
| `getpath` | `getpath(["a","b"])` | 📋 | `getpath(["a","b"])` | Get by path |
| `setpath` | `setpath(["a","b"]; x)` | 📋 | `setpath(["a","b"]; x)` | Set by path |
| `delpaths` | `delpaths([["a","b"]])` | 📋 | `delpaths([["a","b"]])` | Delete paths |

---

## Selection & Filtering

| Feature | jq Syntax | Status | TQ Equivalent | Notes |
|---------|-----------|--------|---------------|-------|
| `select` | `select(.x > 5)` | 📋 | `select(.x > 5)` | Filter values |
| `map` | `map(.x)` | 📋 | `map(.x)` | Transform array |
| `map_values` | `map_values(.x)` | 📋 | `map_values(.x)` | Transform object values |
| `pick` | `pick(.a, .b)` | 📋 | `pick(.a, .b)` | Select specific values |

---

## Conditionals

| Feature | jq Syntax | Status | TQ Equivalent | Notes |
|---------|-----------|--------|---------------|-------|
| `if-then-else` | `if A then B else C end` | 📋 | `if A then B else C end` | Conditional |
| `if-then` | `if A then B end` | 📋 | `if A then B end` | No else clause |
| Alternative | `A // B` | 📋 | `A // B` | First non-null/false |
| Try-catch | `try A catch B` | 📋 | `try A catch B` | Error handling |

---

## Comparisons & Logic

| Feature | jq Syntax | Status | TQ Equivalent | Notes |
|---------|-----------|--------|---------------|-------|
| Equal | `==` | 📋 | `==` | Equality |
| Not equal | `!=` | 📋 | `!=` | Inequality |
| Less than | `<` | 📋 | `<` | Comparison |
| Less/equal | `<=` | 📋 | `<=` | Comparison |
| Greater than | `>` | 📋 | `>` | Comparison |
| Greater/equal | `>=` | 📋 | `>=` | Comparison |
| `and` | `and` | 📋 | `and` | Logical AND |
| `or` | `or` | 📋 | `or` | Logical OR |
| `not` | `not` | 📋 | `not` | Logical NOT |

---

## Arithmetic

| Feature | jq Syntax | Status | TQ Equivalent | Notes |
|---------|-----------|--------|---------------|-------|
| Addition | `+` | 📋 | `+` | Add numbers/strings/arrays |
| Subtraction | `-` | 📋 | `-` | Subtract numbers/arrays |
| Multiplication | `*` | 📋 | `*` | Multiply numbers/objects |
| Division | `/` | 📋 | `/` | Divide numbers |
| Modulo | `%` | 📋 | `%` | Remainder |
| `floor` | `floor` | 📋 | `floor` | Floor division |
| `sqrt` | `sqrt` | 📋 | `sqrt` | Square root |
| `pow` | `pow(x; y)` | 📋 | `pow(x; y)` | Exponentiation |
| `log` | `log` | 📋 | `log` | Natural log |
| `log10` | `log10` | 📋 | `log10` | Base-10 log |
| `exp` | `exp` | 📋 | `exp` | Exponential |
| `abs` | `abs` | 📋 | `abs` | Absolute value |

---

## String Operations

| Feature | jq Syntax | Status | TQ Equivalent | Notes |
|---------|-----------|--------|---------------|-------|
| String interpolation | `"\\(x)"` | 📋 | `"\\(x)"` | Embed expressions |
| `tostring` | `tostring` | 📋 | `tostring` | Convert to string |
| `tonumber` | `tonumber` | 📋 | `tonumber` | Parse number |
| `ascii_downcase` | `ascii_downcase` | 📋 | `lowercase` | Lowercase |
| `ascii_upcase` | `ascii_upcase` | 📋 | `uppercase` | Uppercase |
| `split` | `split(";")` | 📋 | `split(";")` | Split string |
| `join` | `join(",")` | 📋 | `join(",")` | Join array |
| `ltrimstr` | `ltrimstr("x")` | 📋 | `ltrimstr("x")` | Trim left |
| `rtrimstr` | `rtrimstr("x")` | 📋 | `rtrimstr("x")` | Trim right |
| `startswith` | `startswith("x")` | 📋 | `startswith("x")` | Check prefix |
| `endswith` | `endswith("x")` | 📋 | `endswith("x")` | Check suffix |
| `test` | `test("regex")` | 📋 | `test("regex")` | Regex match |
| `match` | `match("regex")` | 📋 | `match("regex")` | Regex match object |
| `capture` | `capture("regex")` | 📋 | `capture("regex")` | Named captures |
| `sub` | `sub("a"; "b")` | 📋 | `sub("a"; "b")` | Replace first |
| `gsub` | `gsub("a"; "b")` | 📋 | `gsub("a"; "b")` | Replace all |

---

## Type Checking

| Feature | jq Syntax | Status | TQ Equivalent | Notes |
|---------|-----------|--------|---------------|-------|
| `isnumber` | `isnumber` | 📋 | `isnumber` | Check number |
| `isstring` | `isstring` | 📋 | `isstring` | Check string |
| `isboolean` | `isboolean` | 📋 | `isboolean` | Check boolean |
| `isarray` | `isarray` | 📋 | `isarray` | Check array |
| `isobject` | `isobject` | 📋 | `isobject` | Check object |
| `isnull` | `isnull` | 📋 | `isnull` | Check null |
| `isnan` | `isnan` | 📋 | `isnan` | Check NaN |
| `isinfinite` | `isinfinite` | 📋 | `isinfinite` | Check infinity |
| `isfinite` | `isfinite` | 📋 | `isfinite` | Check finite |
| `isnormal` | `isnormal` | 📋 | `isnormal` | Check normal number |

---

## Date Operations

| Feature | jq Syntax | Status | TQ Equivalent | Notes |
|---------|-----------|--------|---------------|-------|
| `now` | `now` | 📋 | `now` | Current timestamp |
| `fromdateiso8601` | `fromdateiso8601` | 📋 | `fromdateiso8601` | Parse ISO date |
| `todateiso8601` | `todateiso8601` | 📋 | `todateiso8601` | Format ISO date |
| `fromdate` | `fromdate` | 📋 | `fromdate` | Parse Unix timestamp |
| `todate` | `todate` | 📋 | `todate` | Format Unix timestamp |
| `strftime` | `strftime("%Y-%m-%d")` | 📋 | `strftime("%Y-%m-%d")` | Format date |
| `strptime` | `strptime("%Y-%m-%d")` | 📋 | `strptime("%Y-%m-%d")` | Parse date |
| `gmtime` | `gmtime` | 📋 | `gmtime` | GMT time |
| `mktime` | `mktime` | 📋 | `mktime` | Make timestamp |

---

## I/O & Formatting

| Feature | jq Syntax | Status | TQ Equivalent | Notes |
|---------|-----------|--------|---------------|-------|
| `@base64` | `@base64` | 📋 | `@base64` | Base64 encode |
| `@base64d` | `@base64d` | 📋 | `@base64d` | Base64 decode |
| `@uri` | `@uri` | 📋 | `@uri` | URI encode |
| `@csv` | `@csv` | 📋 | `@csv` | CSV format |
| `@tsv` | `@tsv` | 📋 | `@tsv` | TSV format |
| `@json` | `@json` | 📋 | `@json` | JSON format |
| `@html` | `@html` | 📋 | `@html` | HTML encode |
| `@text` | `@text` | 📋 | `@text` | Plain text |
| `format` | `format("csv")` | 📋 | `format("csv")` | Format as type |

---

## SQL & Path

| Feature | jq Syntax | Status | TQ Equivalent | Notes |
|---------|-----------|--------|---------------|-------|
| `@sh` | `@sh` | 📋 | `@sh` | Shell escape |
| `paths` | `paths` | 📋 | `paths` | All paths |
| `paths(filter)` | `paths(isnumber)` | 📋 | `paths(isnumber)` | Filtered paths |
| `leaf_paths` | `leaf_paths` | 📋 | `leaf_paths` | Leaf paths only |

---

## Advanced Streaming

| Feature | jq Syntax | Status | TQ Equivalent | Notes |
|---------|-----------|--------|---------------|-------|
| `--stream` | CLI flag | 📋 | `--stream` | Streaming mode |
| `truncate_stream` | `truncate_stream(f)` | ⏳ | `truncate_stream(f)` | Truncate stream |
| `fromstream` | `fromstream(f)` | ⏳ | `fromstream(f)` | Build from stream |
| `tostream` | `tostream` | ⏳ | `tostream` | Convert to stream |

---

## Assignment & Update

| Feature | jq Syntax | Status | TQ Equivalent | Notes |
|---------|-----------|--------|---------------|-------|
| Update | `.x = 5` | 📋 | `.x = 5` | Update field |
| Update-assign | `.x \|= f` | 📋 | `.x \|= f` | Update with pipe |
| Arithmetic update | `.x += 1` | 📋 | `.x += 1` | Add and assign |
| Alternative update | `.x //= 5` | 📋 | `.x //= 5` | Set if null |

---

## Variables & Functions

| Feature | jq Syntax | Status | TQ Equivalent | Notes |
|---------|-----------|--------|---------------|-------|
| Variable binding | `x as $var \| ...` | 📋 | `x as $var \| ...` | Bind variable |
| Destructuring | `[.a, .b] as [$x, $y] \| ...` | 📋 | `[.a, .b] as [$x, $y] \| ...` | Destructure |
| Function definition | `def f: ...;` | 📋 | `def f: ...;` | Define function |
| Function with args | `def f(x; y): ...;` | 📋 | `def f(x; y): ...;` | Function with params |
| Recursive functions | `def fact: if . <= 1 then 1 else . * ((.-1)\|fact) end;` | 📋 | Same | Recursion |

---

## Reduce & Foreach

| Feature | jq Syntax | Status | TQ Equivalent | Notes |
|---------|-----------|--------|---------------|-------|
| `reduce` | `reduce .[] as $x (0; . + $x)` | 📋 | `reduce .[] as $x (0; . + $x)` | Custom reduction |
| `foreach` | `foreach .[] as $x (init; update; extract)` | 📋 | `foreach .[] as $x (init; update; extract)` | Stateful iteration |
| `limit` | `limit(n; exp)` | 📋 | `limit(n; exp)` | Limit output |
| `first` | `first(exp)` | 📋 | `first(exp)` | First value |
| `last` | `last(exp)` | 📋 | `last(exp)` | Last value |
| `until` | `until(cond; update)` | 📋 | `until(cond; update)` | Loop until |
| `while` | `while(cond; update)` | 📋 | `while(cond; update)` | Loop while |
| `repeat` | `repeat(exp)` | 📋 | `repeat(exp)` | Infinite repeat |

---

## Modules & Imports

| Feature | jq Syntax | Status | TQ Equivalent | Notes |
|---------|-----------|--------|---------------|-------|
| `import` | `import "module" as mod;` | ⏳ | `import "module" as mod;` | Import module |
| `include` | `include "module";` | ⏳ | `include "module";` | Include module |
| Module metadata | `modulemeta("mod")` | ⏳ | `modulemeta("mod")` | Module info |

---

## TOON-Specific Extensions

These features are **unique to TQ** and leverage TOON's columnar/tabular nature:

| Feature | Syntax | Status | Notes |
|---------|--------|--------|-------|
| Column projection | `.users{name,email}` | 📋 | Select specific columns |
| Column exclusion | `.users{-password}` | 📋 | Exclude columns |
| Row blocks | `.users[100]{name,age}` | 📋 | Fetch N rows with columns |
| Streaming aggregation | `.users[] \| .age \| avg()` | 📋 | Constant memory aggregation |
| TOON format output | `--output-format toon` | 📋 | Native TOON output |
| Multi-format input | `--input-format csv` | 📋 | Read CSV/JSON/TOON |

---

## Implementation Priority

### High Priority (Phase 2)
1. Pipe operator (`|`)
2. Array indexing (`.[0]`, `.[1:5]`)
3. `select()` and `map()`
4. Arithmetic operators
5. Comparison operators
6. `if-then-else`

### Medium Priority (Phase 3)
1. String operations
2. Type checking
3. Object construction
4. Array operations (`sort`, `group_by`, `unique`)
5. Aggregations (`sum`, `avg`, `min`, `max`)

### Lower Priority (Phase 4+)
1. Date operations
2. Advanced streaming
3. Modules
4. Format conversions
5. Reduce/foreach

---

## Testing Strategy

For each jq feature:
1. ✅ Add to grammar/lexer
2. ✅ Implement in AST
3. ✅ Implement in evaluator
4. ✅ Add C++ unit tests
5. ✅ Add integration tests
6. ✅ Update Python bindings
7. ✅ Update CLI
8. ✅ Add documentation examples

---

## Reference Implementations

- **jq**: https://github.com/jqlang/jq
- **gojq**: https://github.com/itchyny/gojq (Go implementation)
- **jaq**: https://github.com/01mf02/jaq (Rust implementation)
- **jqjq**: https://github.com/wader/jqjq (jq in jq!)

---

**Let's achieve 100% jq feature parity for TOON! 🚀**
