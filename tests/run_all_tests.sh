#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test counters
TOTAL_SUITES=0
PASSED_SUITES=0
FAILED_SUITES=0

# Function to print summary
print_summary() {
    echo ""
    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE}  OVERALL TEST SUMMARY${NC}"
    echo -e "${BLUE}========================================${NC}"
    echo -e "Total test suites: $TOTAL_SUITES"
    echo -e "${GREEN}Passed: $PASSED_SUITES${NC}"
    echo -e "${RED}Failed: $FAILED_SUITES${NC}"

    if [ $FAILED_SUITES -eq 0 ]; then
        echo -e "\n${GREEN}All test suites passed!${NC}"
    else
        echo -e "\n${RED}Some test suites failed.${NC}"
    fi
}

# Function to run a test
run_test() {
    local test_name=$1
    local test_binary=$2

    if [ ! -f "$test_binary" ]; then
        echo -e "${RED}Error: Test binary $test_binary not found${NC}"
        echo -e "${YELLOW}Run 'make' first to compile tests${NC}"
        return 1
    fi

    echo ""
    ./"$test_binary"
    local exit_code=$?

    TOTAL_SUITES=$((TOTAL_SUITES + 1))

    if [ $exit_code -eq 0 ]; then
        PASSED_SUITES=$((PASSED_SUITES + 1))
    else
        FAILED_SUITES=$((FAILED_SUITES + 1))
    fi

    return $exit_code
}

# Main execution
echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  Minishell Builtin Tests${NC}"
echo -e "${BLUE}========================================${NC}"

# Run each test
run_test "echo" "test_echo"
run_test "pwd" "test_pwd"
run_test "cd" "test_cd"
run_test "env" "test_env"
run_test "export" "test_export"
run_test "unset" "test_unset"
run_test "exit" "test_exit"
run_test "expand_vars" "test_expand_vars"

print_summary

# Exit with appropriate code
if [ $FAILED_SUITES -eq 0 ]; then
    exit 0
else
    exit 1
fi
