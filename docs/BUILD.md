# Building TQ

Complete build and installation instructions for all platforms.

## Table of Contents

- [Prerequisites](#prerequisites)
- [Building on Linux](#building-on-linux)
- [Building on macOS](#building-on-macos)
- [Building on Windows](#building-on-windows)
- [Python Bindings](#python-bindings)
- [Build Options](#build-options)
- [Troubleshooting](#troubleshooting)

## Prerequisites

### All Platforms

- **CMake** >= 3.20
- **C++20 compiler**:
  - GCC >= 10
  - Clang >= 12
  - MSVC >= 2019 (Visual Studio 2019 or later)

### Python Bindings (Optional)

- **Python** >= 3.7
- **pip** (Python package manager)
- **pybind11** >= 2.10.0

Install pybind11:
```bash
pip install pybind11
```

## Building on Linux

### Ubuntu/Debian

```bash
# Install dependencies
sudo apt-get update
sudo apt-get install -y build-essential cmake git

# Clone repository (if not already done)
git clone https://github.com/tq-project/tq.git
cd tq

# Build tq-core
cd tq-core
mkdir build && cd build
cmake ..
cmake --build . -j$(nproc)

# Run tests
ctest --output-on-failure

# Install (optional)
sudo cmake --install .

# Build CLI tool
cd ../../tq-cli
mkdir build && cd build
cmake ..
cmake --build .

# Install CLI (optional)
sudo cp tq /usr/local/bin/
```

### Fedora/RHEL/CentOS

```bash
# Install dependencies
sudo dnf install -y gcc-c++ cmake make git

# Follow same build steps as Ubuntu
```

### Arch Linux

```bash
# Install dependencies
sudo pacman -S base-devel cmake git

# Follow same build steps as Ubuntu
```

## Building on macOS

### Using Homebrew

```bash
# Install dependencies
brew install cmake

# Ensure Xcode Command Line Tools are installed
xcode-select --install

# Build tq-core
cd tq-core
mkdir build && cd build
cmake ..
cmake --build . -j$(sysctl -n hw.ncpu)

# Run tests
ctest --output-on-failure

# Build CLI tool
cd ../../tq-cli
mkdir build && cd build
cmake ..
cmake --build .

# Install CLI (optional)
sudo cp tq /usr/local/bin/
```

### Using MacPorts

```bash
# Install dependencies
sudo port install cmake

# Follow same build steps as Homebrew
```

## Building on Windows

### Using Visual Studio 2019/2022

#### GUI Method

1. Install **Visual Studio 2019** or later with **C++ development tools**
2. Install **CMake** (included in Visual Studio or download from cmake.org)
3. Open **Developer Command Prompt** or **PowerShell**

```powershell
# Build tq-core
cd tq-core
mkdir build
cd build
cmake ..
cmake --build . --config Release

# Run tests
ctest -C Release --output-on-failure

# Build CLI tool
cd ..\..\tq-cli
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

#### Command Line Method

```powershell
# Using Visual Studio build tools
cd tq-core
mkdir build
cd build
cmake -G "Visual Studio 16 2019" ..
cmake --build . --config Release

# Or using NMake
cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release ..
nmake
```

### Using MinGW-w64

```bash
# Install MinGW-w64 (via MSYS2 or standalone)
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake

# Build
cd tq-core
mkdir build && cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
```

### Using WSL (Windows Subsystem for Linux)

Follow the [Linux instructions](#building-on-linux) in WSL.

## Python Bindings

### Linux/macOS

```bash
cd pytq

# Install in development mode
pip install -e .

# Or build wheel
pip install build
python -m build
pip install dist/pytq-*.whl

# Verify installation
python -c "import pytq; print(pytq.__version__)"
```

### Windows

```powershell
cd pytq

# Ensure Visual Studio is in PATH
# Or use Developer Command Prompt

pip install -e .

# Verify
python -c "import pytq; print(pytq.__version__)"
```

### Common Issues

**Issue:** CMake can't find pybind11
```bash
# Solution: Install pybind11 via pip
pip install pybind11
```

**Issue:** Can't find tq-core library
```bash
# Solution: Build tq-core first
cd ../tq-core
mkdir build && cd build
cmake .. && cmake --build .
cd ../../pytq
pip install -e .
```

## Build Options

### CMake Options

```bash
# Build type (Debug, Release, RelWithDebInfo, MinSizeRel)
cmake -DCMAKE_BUILD_TYPE=Release ..

# Custom install prefix
cmake -DCMAKE_INSTALL_PREFIX=/opt/tq ..

# Enable/disable tests
cmake -DBUILD_TESTING=ON ..

# Verbose build output
cmake --build . --verbose
```

### Compiler Selection

```bash
# Use specific compiler
cmake -DCMAKE_CXX_COMPILER=g++-11 ..
cmake -DCMAKE_CXX_COMPILER=clang++-14 ..

# Windows MSVC
cmake -G "Visual Studio 16 2019" ..

# Windows Clang
cmake -G "Visual Studio 16 2019" -T ClangCL ..
```

### Build Configurations

#### Debug Build

```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .

# Debug with sanitizers (Linux/macOS)
cmake -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_CXX_FLAGS="-fsanitize=address -fsanitize=undefined" ..
```

#### Release Build (Optimized)

```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . -j$(nproc)
```

#### Static vs Shared Library

Both are built by default:
- `libtq_core.so` / `tq_core.dll` (shared)
- `libtq_core_static.a` / `tq_core_static.lib` (static)

## Installation

### System-Wide Installation (Linux/macOS)

```bash
# Install core library
cd tq-core/build
sudo cmake --install .

# Install CLI tool
cd ../../tq-cli/build
sudo cmake --install .

# Verify
tq --help
```

### Local Installation

```bash
# Install to custom prefix
cmake -DCMAKE_INSTALL_PREFIX=$HOME/.local ..
cmake --build .
cmake --install .

# Add to PATH
export PATH=$HOME/.local/bin:$PATH
```

### Python Package

```bash
# User installation
pip install --user pytq

# Virtual environment
python -m venv venv
source venv/bin/activate  # or venv\Scripts\activate on Windows
pip install pytq
```

## Troubleshooting

### CMake Errors

**Error:** CMake version too old
```bash
# Solution: Install newer CMake
pip install cmake
# Or download from cmake.org
```

**Error:** Can't find compiler
```bash
# Linux/macOS
export CXX=/usr/bin/g++
cmake ..

# Windows
# Use Developer Command Prompt or VS prompt
```

### Build Errors

**Error:** C++20 features not available
```bash
# Solution: Use newer compiler
# GCC >= 10, Clang >= 12, MSVC >= 2019
```

**Error:** Linker errors with tq_core
```bash
# Solution: Ensure tq-core is built first
cd tq-core/build
cmake .. && cmake --build .
```

### Runtime Errors

**Error:** Shared library not found (Linux)
```bash
# Solution: Set LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/path/to/tq-core/build:$LD_LIBRARY_PATH
```

**Error:** DLL not found (Windows)
```powershell
# Solution: Copy DLL to executable directory
copy ..\tq-core\build\Release\tq_core.dll .
# Or add to PATH
```

### Python Binding Errors

**Error:** ImportError: pytq module not found
```bash
# Solution: Rebuild and reinstall
cd pytq
pip uninstall pytq
pip install -e .
```

**Error:** CMake can't find Python
```bash
# Solution: Specify Python executable
cmake -DPYTHON_EXECUTABLE=$(which python3) ..
```

## Testing the Build

### Core Library Tests

```bash
cd tq-core/build
ctest --output-on-failure

# Or run tests individually
./tests/test_lexer
./tests/test_parser
./tests/test_evaluator
./tests/test_value
./tests/test_integration
```

### CLI Tool Test

```bash
cd tq-cli/build
echo '{"name":"Alice"}' | ./tq '.name'
# Expected output: "Alice"
```

### Python Bindings Test

```python
python3 -c "import pytq; print(pytq.query('.name', {'name': 'Alice'}))"
# Expected output: ['Alice']
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

## Cross-Compilation

### For ARM64 (Linux)

```bash
# Install cross-compiler
sudo apt-get install g++-aarch64-linux-gnu

# Configure
cmake -DCMAKE_CXX_COMPILER=aarch64-linux-gnu-g++ ..
cmake --build .
```

### For WebAssembly (Future)

See `docs/PHASE_6_WASM.md` for WASM build instructions (planned).

## Development Build

For active development:

```bash
# Debug build with all warnings
cmake -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_CXX_FLAGS="-Wall -Wextra -Wpedantic" ..

# With sanitizers (catch memory bugs)
cmake -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_CXX_FLAGS="-fsanitize=address -fsanitize=undefined -g" ..

# Build and test
cmake --build . && ctest
```

## Clean Build

```bash
# Remove build directory
rm -rf build

# Rebuild from scratch
mkdir build && cd build
cmake ..
cmake --build .
```

## Need Help?

- Check [README.md](../README.md) for overview
- See [API.md](API.md) for API documentation
- Review [EXAMPLES.md](EXAMPLES.md) for usage examples
- Open an issue on GitHub
