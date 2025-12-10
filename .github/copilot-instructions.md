# Copilot Instructions for TQ Query Engine

## Project Overview
- **TQ** is a high-performance query engine for **TOON data only**, inspired by `jq`, implemented in C/C++ first, with planned bindings for Python, WASM, and a CLI tool.
- **IMPORTANT**: TQ exclusively uses TOON format for both input and output. JSON is NOT supported.
- The project is organized in phases, each with a dedicated design doc in `docs/`.

## TOON Format Only
- **TQ accepts ONLY TOON format as input** - no JSON support
- **TQ outputs ONLY TOON format** - no JSON conversion
- TOON parser: `ToonParser::parse()` in `tq-core/src/toon_parser.cpp`
- TOON encoder: `Value::to_toon()` in `tq-core/src/value.cpp`
- Reference implementation in `toon-reference/` directory

## Architecture & Key Components
- **C/C++ Core** (`tq-core/`): Implements lexer, parser, AST, evaluator, and TOON value traversal. Produces a shared library (`libtq_core.{so|dll|dylib}`).
- **AST & Parser**: Query grammar is simple, supporting field lookups and iteration. AST is a vector of `Step` structs.
- **Evaluator**: Walks the AST over TOON data, returning results as C++ objects.
- **CLI Tool**: Wraps the core, reads TOON files, outputs TOON format.
- **Python Bindings** (`pytq/`): Uses pybind11 to expose the core query function to Python.
- **WASM Build**: Uses Emscripten to compile the core for browser/Node.js use.

## Developer Workflows
- **C++ Build**: Use CMake in `tq-core/` to build the core library and CLI.
- **Testing**: C++ tests are in `tq-core/tests/`. Use CTest or run test binaries directly.
- **Python Bindings**: Build with `python -m build` or `pip install -e .` in `pytq/`.
- **WASM**: Build with Emscripten as shown in `PHASE_6_WASM.md`.

## Conventions & Patterns
- **Memory Management**: Use RAII, `std::unique_ptr`, and `std::vector`. Avoid raw `malloc/free`.
- **TOON Data**: Native TOON parser implemented in `toon_parser.cpp` based on reference implementation.
- **Query Grammar**: See `PHASE_2_AST_AND_PARSER.md` for the supported syntax and AST structure.
- **Result Handling**: Always return results in TOON format for CLI and as native Value objects for bindings.

## Integration Points
- **Python**: `pytq/` exposes `query()` via pybind11.
- **WASM**: Exports `_tq_query` for browser/Node.js use.

## Key References
- See `docs/PHASE_1_CPP_CORE.md` for core structure and memory rules.
- See `docs/PHASE_2_AST_AND_PARSER.md` for grammar and AST.
- See `docs/PHASE_3_EVALUATOR.md` for evaluation logic.
- See `docs/PHASE_4_CPP_CLI.md` for CLI usage.
- See `docs/PHASE_5_PYTHON_BINDINGS.md` for Python integration.
- See `docs/PHASE_6_WASM.md` for WASM build instructions.

## Example: Python Binding
```python
import pytq
# Input and output are TOON format only
toon_data = """users[2]:
  - email: alice@example.com
  - email: bob@example.com"""
pytq.query('.users[].email', toon_data)
# Returns: ['alice@example.com', 'bob@example.com'] (as TOON strings)
```

## Example: CLI
```sh
# Input and output are TOON format only
tq ".users[].name" file.toon
cat file.toon | tq ".items[].price"
# Output is TOON format, not JSON
```

---
For more details, always consult the relevant `docs/PHASE_*.md` file for the current phase.
