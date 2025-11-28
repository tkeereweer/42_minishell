#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Counters
TOTAL=0
PASSED=0
FAILED=0

# Minishell path
MINISHELL="./minishell"

# Test files
TESTFILE="test_input.txt"
EXPECTED="expected_output.txt"
ACTUAL="actual_output.txt"

# Initialize test environment
setup() {
    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE}    MINISHELL TEST SUITE${NC}"
    echo -e "${BLUE}========================================${NC}\n"

    # Create test files
    echo "Hello World" > "$TESTFILE"
    echo "Line 1" > test1.txt
    echo "Line 2" >> test1.txt
    echo "Line 3" >> test1.txt
}

# Cleanup test environment
cleanup() {
    rm -f "$TESTFILE" "$EXPECTED" "$ACTUAL" test1.txt test2.txt test_out.txt heredoc_* test_*.txt
    echo -e "\n${BLUE}========================================${NC}"
    echo -e "${BLUE}    TEST SUMMARY${NC}"
    echo -e "${BLUE}========================================${NC}"
    echo -e "Total tests: $TOTAL"
    echo -e "${GREEN}Passed: $PASSED${NC}"
    echo -e "${RED}Failed: $FAILED${NC}"
    if [ $FAILED -eq 0 ]; then
        echo -e "\n${GREEN}✓ ALL TESTS PASSED!${NC}"
    else
        echo -e "\n${RED}✗ SOME TESTS FAILED${NC}"
    fi
}

# Run a test command
# Usage: run_test "test_name" "command"
run_test() {
    local test_name="$1"
    local command="$2"
    TOTAL=$((TOTAL + 1))

    # Run command in bash
    bash -c "$command" > "$EXPECTED" 2>&1
    local bash_exit=$?

    # Run command in minishell and filter output
    # We need to remove:
    # 1. The prompt lines (lines with username@path:)
    # 2. Lines that exactly match the command (echo of input)
    # 3. The exit command
    local raw_output=$(mktemp)
    echo -e "$command\nexit" | $MINISHELL 2>&1 > "$raw_output"

    # Filter: remove prompt lines, command echo, and exit line
    grep -v "^[^:]*@.*:" "$raw_output" | grep -vF "$command" | grep -v "^exit$" > "$ACTUAL"
    local mini_exit=$?
    rm -f "$raw_output"

    # Always show command and outputs
    echo -e "\n${BLUE}Command:${NC} $command"
    echo -e "${YELLOW}Expected output:${NC}"
    cat "$EXPECTED" | sed 's/^/  /'
    echo -e "${YELLOW}Minishell output:${NC}"
    cat "$ACTUAL" | sed 's/^/  /'

    # Compare outputs
    if diff -q "$EXPECTED" "$ACTUAL" > /dev/null 2>&1; then
        echo -e "${GREEN}✓ PASS${NC} - $test_name"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗ FAIL${NC} - $test_name"
        echo -e "${RED}Outputs do not match!${NC}"
        FAILED=$((FAILED + 1))
    fi
}

# Run a test that only checks exit status
run_test_exit() {
    local test_name="$1"
    local command="$2"
    local expected_exit="$3"
    TOTAL=$((TOTAL + 1))

    # Run command in minishell
    echo "$command" | $MINISHELL > /dev/null 2>&1
    local mini_exit=$?

    if [ $mini_exit -eq $expected_exit ]; then
        echo -e "${GREEN}✓${NC} $test_name"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗${NC} $test_name"
        echo -e "  ${YELLOW}Command:${NC} $command"
        echo -e "  ${YELLOW}Expected exit:${NC} $expected_exit"
        echo -e "  ${YELLOW}Got exit:${NC} $mini_exit"
        FAILED=$((FAILED + 1))
    fi
}

# Test categories

test_simple_commands() {
    echo -e "\n${BLUE}[Simple Commands]${NC}"
    run_test "echo with args" "echo Hello World"
    run_test "echo with -n flag" "echo -n Hello"
    run_test "pwd" "pwd"
    run_test "ls current directory" "ls"
    run_test "cat existing file" "cat $TESTFILE"
}

test_pipes() {
    echo -e "\n${BLUE}[Pipe Tests]${NC}"
    run_test "simple pipe: echo | cat" "echo hello | cat"
    run_test "simple pipe: ls | grep test" "ls | grep test"
    run_test "two pipes: echo | cat | cat" "echo test | cat | cat"
    run_test "three pipes" "echo hello | cat | cat | cat"
    run_test "pipe with grep" "cat $TESTFILE | grep World"
    run_test "pipe with wc" "echo -e 'line1\nline2\nline3' | wc -l"
    run_test "ls | sort" "ls | sort"
    run_test "echo | cat | wc -w" "echo one two three | cat | wc -w"
}

test_redirections() {
    echo -e "\n${BLUE}[Redirection Tests]${NC}"

    # Output redirection
    run_test "output redirect >" "echo test > test_out.txt && cat test_out.txt"
    run_test "append redirect >>" "echo first > test_out.txt && echo second >> test_out.txt && cat test_out.txt"

    # Input redirection
    run_test "input redirect <" "cat < $TESTFILE"
    run_test "input and output" "cat < $TESTFILE > test_out.txt && cat test_out.txt"

    # Multiple redirections
    run_test "multiple output redirects" "echo test > test_out.txt > test2.txt && cat test2.txt"
}

test_builtins() {
    echo -e "\n${BLUE}[Builtin Tests]${NC}"
    run_test "echo simple" "echo hello"
    run_test "echo multiple args" "echo one two three"
    run_test "echo with quotes" "echo 'hello world'"
    run_test "pwd" "pwd"
    run_test "env (check if exists)" "env | grep PATH | head -1"
}

test_logical_operators() {
    echo -e "\n${BLUE}[Logical Operators]${NC}"
    run_test "AND true true" "echo first && echo second"
    run_test "AND true false" "true && false && echo should_not_print"
    run_test "OR false true" "false || echo should_print"
    run_test "OR true true" "echo first || echo second"
    run_test "complex logic" "true && echo yes || echo no"
}

test_quotes() {
    echo -e "\n${BLUE}[Quote Tests]${NC}"
    run_test "single quotes" "echo 'hello world'"
    run_test "double quotes" "echo \"hello world\""
    run_test "mixed quotes" "echo 'single' \"double\""
    run_test "quotes with spaces" "echo \"test   spaces\""
}

test_variables() {
    echo -e "\n${BLUE}[Variable Expansion Tests]${NC}"
    run_test "expand HOME" "echo \$HOME"
    run_test "expand USER" "echo \$USER"
    run_test "expand PATH (first part)" "echo \$PATH | cut -d: -f1"
    run_test "expand with text" "echo test\$USER"
}

test_edge_cases() {
    echo -e "\n${BLUE}[Edge Cases]${NC}"
    run_test "empty command" ""
    run_test "only spaces" "   "
    run_test "pipe at start (should fail gracefully)" "| cat"
    run_test "pipe at end (should fail gracefully)" "echo test |"
    run_test "multiple pipes" "echo test | | cat"
    run_test "redirect non-existent file" "cat < nonexistent.txt"
}

test_combined() {
    echo -e "\n${BLUE}[Combined Tests]${NC}"
    run_test "pipe + redirect" "echo test | cat > test_out.txt && cat test_out.txt"
    run_test "redirect + pipe" "cat < $TESTFILE | grep World"
    run_test "pipe + logic" "echo first | cat && echo second"
    run_test "multiple commands" "echo one && echo two && echo three"
}

# Main execution
main() {
    setup

    test_simple_commands
    test_pipes
    test_redirections
    test_builtins
    test_logical_operators
    test_quotes
    test_variables
    test_edge_cases
    test_combined

    cleanup
}

# Run tests
main
