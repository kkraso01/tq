#include "tq/tq.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>

struct BenchmarkResult {
    std::string name;
    double time_ms;
    size_t result_count;
};

std::string read_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    return content;
}

BenchmarkResult benchmark_query(const std::string& name, 
                                const std::string& expr,
                                const std::string& data,
                                int iterations = 1000) {
    auto start = std::chrono::high_resolution_clock::now();
    
    size_t total_results = 0;
    for (int i = 0; i < iterations; ++i) {
        auto results = tq::query(expr, data);
        total_results = results.size();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    return BenchmarkResult{
        name,
        duration.count() / 1000.0 / iterations,
        total_results
    };
}

int main() {
    std::cout << "TQ Query Engine Benchmarks\n";
    std::cout << "===========================\n\n";
    
    try {
        // Load test data
        std::string data = read_file("tests/data/sample.json");
        
        std::vector<BenchmarkResult> results;
        
        // Benchmark various queries
        results.push_back(benchmark_query("Simple field access", ".metadata", data));
        results.push_back(benchmark_query("Nested field", ".metadata.count", data));
        results.push_back(benchmark_query("Array iteration", ".users[]", data));
        results.push_back(benchmark_query("Array field fanout", ".users[].email", data));
        results.push_back(benchmark_query("Nested array access", ".users[].roles[]", data));
        
        // Print results
        std::cout << "Query                          Time (ms)    Results\n";
        std::cout << "----------------------------------------------------\n";
        
        for (const auto& result : results) {
            std::cout << std::left << std::setw(30) << result.name
                      << std::right << std::setw(10) << std::fixed 
                      << std::setprecision(4) << result.time_ms
                      << std::setw(10) << result.result_count << "\n";
        }
        
        std::cout << "\n Benchmarks completed successfully\n";
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Benchmark failed: " << e.what() << "\n";
        return 1;
    }
}
