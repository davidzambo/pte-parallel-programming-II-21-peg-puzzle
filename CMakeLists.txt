cmake_minimum_required(VERSION 3.24)
project(pte_parallel_programming_II_21_peg_puzzle C)

set(CMAKE_C_STANDARD 11)

add_executable(pte_parallel_programming_II_21_peg_puzzle
        main.c)

enable_testing()
include(CTest)

set (USAGE_REGEX "^Usage:[.]+")

add_test(NAME "Show usage test"
        COMMAND "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}")
set_property(TEST "Show usage test"
        PROPERTY WILL_FAIL true)
set_property(TEST "Show usage test"
        PROPERTY PASS_REGULAR_EXPRESSION "${USAGE_REGEX}")
