#include "tq/tq.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>

void print_usage(const char* prog_name) {
    std::cerr << "Usage: " << prog_name << " [OPTIONS] <expression> [file]\n"
              << "\n"
              << "Query TOON data with jq-style expressions\n"
              << "\n"
              << "Arguments:\n"
              << "  <expression>    TQ query expression (e.g., '.users[].email')\n"
              << "  [file]          Input file (TOON format). Use '-' or omit for stdin\n"
              << "\n"
              << "Options:\n"
              << "  -b, --benchmark Benchmark mode: show execution time\n"
              << "  -h, --help      Show this help message\n"
              << "\n"
              << "Examples:\n"
              << "  tq '.name' data.toon\n"
              << "  tq '.users[].email' data.toon\n"
              << "  cat data.toon | tq '.items[].price'\n"
              << "  tq '.data' input.toon\n";
}

std::string read_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    
    std::ostringstream oss;
    oss << file.rdbuf();
    return oss.str();
}

std::string read_stdin() {
    std::ostringstream oss;
    oss << std::cin.rdbuf();
    return oss.str();
}

int main(int argc, char* argv[]) {
    try {
        if (argc < 2) {
            print_usage(argv[0]);
            return 1;
        }
        
        bool benchmark = false;
        std::string expression;
        std::string input_file;
        
        // Parse arguments
        int arg_idx = 1;
        while (arg_idx < argc) {
            std::string arg = argv[arg_idx];
            
            if (arg == "-h" || arg == "--help") {
                print_usage(argv[0]);
                return 0;
            } else if (arg == "-b" || arg == "--benchmark") {
                benchmark = true;
                ++arg_idx;
            } else if (expression.empty()) {
                expression = arg;
                ++arg_idx;
            } else if (input_file.empty()) {
                input_file = arg;
                ++arg_idx;
            } else {
                std::cerr << "Unexpected argument: " << arg << "\n";
                print_usage(argv[0]);
                return 1;
            }
        }
        
        if (expression.empty()) {
            std::cerr << "Error: No expression provided\n";
            print_usage(argv[0]);
            return 1;
        }
        
        // Read input data (TOON format)
        std::string data;
        if (input_file.empty() || input_file == "-") {
            data = read_stdin();
        } else {
            data = read_file(input_file);
        }
        
        if (data.empty()) {
            std::cerr << "Error: Empty input\n";
            return 1;
        }
        
        // Execute query
        auto start = std::chrono::high_resolution_clock::now();
        
        std::vector<std::string> results = tq::query(expression, data);
        
        auto end = std::chrono::high_resolution_clock::now();
        
        // Output results (TOON format)
        for (const auto& result : results) {
            std::cout << result << "\n";
        }
        
        // Benchmark output
        if (benchmark) {
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cerr << "\nExecution time: " << duration.count() / 1000.0 << " ms\n";
            std::cerr << "Results: " << results.size() << "\n";
        }
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
