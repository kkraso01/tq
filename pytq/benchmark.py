#!/usr/bin/env python3
"""
Benchmark script for pytq Python bindings
"""

import time
import json
import sys

try:
    import pytq
except ImportError:
    print("Error: pytq module not found. Build and install it first.")
    sys.exit(1)

def benchmark_query(name, expression, data, iterations=1000):
    """Run a benchmark for a specific query"""
    start = time.perf_counter()
    
    result_count = 0
    for _ in range(iterations):
        results = pytq.query(expression, data)
        result_count = len(results)
    
    end = time.perf_counter()
    avg_time_ms = (end - start) * 1000 / iterations
    
    return {
        'name': name,
        'time_ms': avg_time_ms,
        'result_count': result_count
    }

def main():
    print("pytq Python Bindings Benchmarks")
    print("================================\n")
    
    # Load test data
    with open('../tq-core/tests/data/sample.json', 'r') as f:
        data = json.load(f)
    
    # Run benchmarks
    benchmarks = [
        ("Simple field access", ".metadata", data),
        ("Nested field", ".metadata.count", data),
        ("Array iteration", ".users[]", data),
        ("Array field fanout", ".users[].email", data),
        ("Nested array access", ".users[].roles[]", data),
    ]
    
    results = []
    for name, expr, test_data in benchmarks:
        result = benchmark_query(name, expr, test_data)
        results.append(result)
    
    # Print results
    print(f"{'Query':<30} {'Time (ms)':>10} {'Results':>10}")
    print("-" * 52)
    
    for result in results:
        print(f"{result['name']:<30} {result['time_ms']:>10.4f} {result['result_count']:>10}")
    
    print("\n Benchmarks completed successfully")

if __name__ == '__main__':
    main()
