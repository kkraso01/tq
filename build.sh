#!/bin/bash
# Build and test TQ project
# Run from project root: ./build.sh

set -e

GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

echo -e "${CYAN}====================================${NC}"
echo -e "${CYAN}  TQ Project Build Script${NC}"
echo -e "${CYAN}====================================${NC}"
echo ""

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_SUCCESS=true

# Build tq-core
echo -e "${YELLOW}[1/3] Building tq-core...${NC}"
cd "$ROOT/tq-core"

mkdir -p build
cd build

if cmake .. && cmake --build . -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 2); then
    echo -e "${GREEN} tq-core built successfully${NC}"
    
    # Run tests
    echo -e "\n${YELLOW}[Tests] Running tq-core tests...${NC}"
    if ctest --output-on-failure; then
        echo -e "${GREEN} All tests passed${NC}"
    else
        echo -e "${RED} Some tests failed${NC}"
        BUILD_SUCCESS=false
    fi
else
    echo -e "${RED} tq-core build failed${NC}"
    BUILD_SUCCESS=false
fi

# Build tq-cli
echo -e "\n${YELLOW}[2/3] Building tq-cli...${NC}"
cd "$ROOT/tq-cli"

mkdir -p build
cd build

if cmake .. && cmake --build .; then
    echo -e "${GREEN} tq-cli built successfully${NC}"
    
    # Test CLI
    echo -e "\n${YELLOW}[Tests] Testing CLI...${NC}"
    result=$(echo '{"name":"Alice"}' | ./tq '.name' 2>/dev/null || echo "FAILED")
    if [ "$result" = '"Alice"' ]; then
        echo -e "${GREEN} CLI test passed${NC}"
    else
        echo -e "${RED} CLI test failed${NC}"
        BUILD_SUCCESS=false
    fi
else
    echo -e "${RED} tq-cli build failed${NC}"
    BUILD_SUCCESS=false
fi

# Build Python bindings (optional)
echo -e "\n${YELLOW}[3/3] Building Python bindings...${NC}"
cd "$ROOT/pytq"

if command -v python3 &> /dev/null; then
    if python3 -m pip install -e . &>/dev/null; then
        echo -e "${GREEN} Python bindings built successfully${NC}"
        
        # Test Python module
        result=$(python3 -c "import pytq; print(pytq.query('.name', {'name': 'Alice'}))" 2>/dev/null || echo "FAILED")
        if echo "$result" | grep -q "Alice"; then
            echo -e "${GREEN} Python bindings test passed${NC}"
        else
            echo -e "${RED} Python bindings test failed${NC}"
            BUILD_SUCCESS=false
        fi
    else
        echo -e "${YELLOW} Python bindings build failed (optional)${NC}"
    fi
else
    echo -e "${YELLOW} Python not found, skipping bindings (optional)${NC}"
fi

# Summary
echo -e "\n${CYAN}====================================${NC}"
if [ "$BUILD_SUCCESS" = true ]; then
    echo -e "${GREEN}   BUILD SUCCESSFUL${NC}"
    echo -e "${CYAN}====================================${NC}"
    echo ""
    echo -e "${CYAN}Next steps:${NC}"
    echo "  - Run CLI: ./tq-cli/build/tq --help"
    echo "  - Run benchmarks: ./tq-core/build/tests/benchmark"
    echo "  - See docs/ for API documentation"
else
    echo -e "${RED}   BUILD FAILED${NC}"
    echo -e "${CYAN}====================================${NC}"
    echo ""
    echo "Check error messages above for details"
    exit 1
fi
