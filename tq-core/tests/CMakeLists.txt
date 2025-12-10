# Test executables
add_executable(test_lexer test_lexer.cpp)
target_link_libraries(test_lexer tq_core_static)

add_executable(test_parser test_parser.cpp)
target_link_libraries(test_parser tq_core_static)

add_executable(test_evaluator test_evaluator.cpp)
target_link_libraries(test_evaluator tq_core_static)

add_executable(test_value test_value.cpp)
target_link_libraries(test_value tq_core_static)

add_executable(test_integration test_integration.cpp)
target_link_libraries(test_integration tq_core_static)

add_executable(test_evaluator_new test_evaluator_new.cpp)
target_link_libraries(test_evaluator_new tq_core_static)

# Benchmark executable
add_executable(benchmark benchmark.cpp)
target_link_libraries(benchmark tq_core_static)

# Register tests
add_test(NAME test_lexer COMMAND test_lexer)
add_test(NAME test_parser COMMAND test_parser)
add_test(NAME test_evaluator COMMAND test_evaluator)
add_test(NAME test_value COMMAND test_value)
add_test(NAME test_integration COMMAND test_integration)
add_test(NAME test_evaluator_new COMMAND test_evaluator_new)
