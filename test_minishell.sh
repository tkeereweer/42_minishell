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
    rm -f "$TESTFILE" "$EXPECTED" "$ACTUAL" test1.txt test2.txt test_out.txt test_out2.txt heredoc_* test_*.txt
    rm -f a b c d e test_err.txt test_noperm.txt "file with spaces.txt" "test file with spaces.txt" "test-dashes.txt"
    rm -f test_wild_*.txt
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
    echo -e "$command\nexit" | $MINISHELL > "$raw_output" 2>&1

    # Filter: remove prompt lines, exact command line match, and exit line
    # Use -x with grep to match whole lines only for command matching
    grep -v "^[^:]*@.*:" "$raw_output" | grep -vxF "$command" | grep -vxF "exit" > "$ACTUAL"
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
    # run_test "echo with -n flag" "echo -n Hello"
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
    # run_test "pipe at start (should fail gracefully)" "| cat"
    # run_test "multiple pipes" "echo test | | cat"
    # run_test "redirect non-existent file" "cat < nonexistent.txt"
}

test_combined() {
    echo -e "\n${BLUE}[Combined Tests]${NC}"
    run_test "pipe + redirect" "echo test | cat > test_out.txt && cat test_out.txt"
    run_test "redirect + pipe" "cat < $TESTFILE | grep World"
    run_test "pipe + logic" "echo first | cat && echo second"
    run_test "multiple commands" "echo one && echo two && echo three"
}

test_crazy_quotes() {
    echo -e "\n${BLUE}[Crazy Quotes]${NC}"
    run_test "crazy quotes 1" "echo this 'is a'test'case'"
    run_test "crazy quotes 2" "echo \"hello \$USER\"how'are you'"
    run_test "crazy quotes 3" "echo 'hello \$USER'how\"are you\""
    run_test "crazy quotes 4" "echo \"hello '\$USER\'\"how'are you'"
    run_test "crazy quotes 5" "echo 'hello \"\$USER\"'how'are you'"
}

test_redirect_who() {
    echo -e "\n${BLUE}[Redirect Who?]${NC}"
    run_test "multiple redirs 1" "echo whatisupbabyyyy >test_4.txt>test_5.txt >test_4.txt >test_6.txt>test_7.txt"
    run_test "multiple redirs 2" "wc -c <test_4.txt<test_5.txt <test_4.txt >test_6.txt> test_7.txt"
}

test_pipe_madness() {
    echo -e "\n${BLUE}[Pipe Madness]${NC}"
    run_test "5 pipes" "echo hello| cat |cat|cat |  cat|cat"
    run_test "pipe + redirect middle" "echo start|cat>test_out.txt| cat"
    run_test "many pipes with grep" "echo 'one two three four'|cat |cat|grep three| cat"
    run_test "pipe chain with wc" "ls|cat |  cat  |wc -l"
    run_test "pipe with multiple outputs" "echo test |tee test_out.txt|cat"
}

test_redirect_chaos() {
    echo -e "\n${BLUE}[Redirect Chaos]${NC}"
    run_test "redirect before command" ">test_out.txt echo hello&&cat test_out.txt"
    run_test "multiple input redirects" "cat< $TESTFILE<$TESTFILE"
    run_test "redirect sandwich" "<$TESTFILE cat>test_out.txt&&cat test_out.txt"
    run_test "append chain" "echo one>test_out.txt&&echo two>>test_out.txt &&echo three  >>test_out.txt&&cat test_out.txt"
    run_test "overwrite after append" "echo first >test_out.txt &&echo second>> test_out.txt&&  echo third>test_out.txt && cat test_out.txt"
    # run_test "redirect stderr and stdout" "ls nonexistent 2>test_err.txt>test_out.txt||cat test_err.txt"
}

test_logic_insanity() {
    echo -e "\n${BLUE}[Logical Insanity]${NC}"
    run_test "AND chain success" "true&&true  &&true&&echo success"
    run_test "AND chain fail" "true &&false&& echo should_not_print"
    run_test "OR chain" "false||false ||true|| echo should_not_print"
    run_test "mixed logic 1" "true&&false||echo printed"
    run_test "mixed logic 2" "false  ||true&&echo printed"
    run_test "mixed logic 3" "true &&echo first||echo second"
    run_test "mixed logic 4" "false||echo first  &&  echo second"
    run_test "logic with pipes" "echo test|cat&&echo success  ||echo fail"
    run_test "logic with redirects" "echo test>test_out.txt&&cat test_out.txt||echo fail"
}

test_parentheses_priority() {
    echo -e "\n${BLUE}[Parentheses Priority]${NC}"
    run_test "simple parens" "( echo hello )"
    run_test "parens with AND" "(true&&echo yes)"
    run_test "parens with OR" "( false||echo yes)"
    run_test "parens priority 1" "false||(true&&echo yes)"
    run_test "parens priority 2" "(false||true)&&echo yes"
    run_test "nested parens" "(  (echo hello)  )"
    run_test "parens with pipes" "(echo test|cat)"
    run_test "multiple paren groups" "(echo one)&&(echo two)"
    run_test "complex parens 1" "(true&&false)||(true&&echo success)"
    run_test "complex parens 2" "false&&(echo no)||(  true&&echo yes)"
}

test_variable_expansion_madness() {
    echo -e "\n${BLUE}[Variable Expansion Madness]${NC}"
    run_test "expand in quotes" "echo \"User is \$USER\""
    run_test "no expand in single quotes" "echo 'User is \$USER'"
    run_test "mixed quote expansion" "echo 'USER:'\$USER"
    run_test "multiple variables" "echo \$USER \$HOME \$PATH|wc -w"

    run_test "undefined variable" "echo \$NONEXISTENT_VAR_12345"
    run_test "dollar without var" "echo \$"
    run_test "variable with special chars" "echo \$USER-\$HOME"
    run_test "variable in redirect" "echo test>\$HOME/.test_minishell_tmp&&cat \$HOME/.test_minishell_tmp&&rm \$HOME/.test_minishell_tmp"
}

test_wildcard_chaos() {
    echo -e "\n${BLUE}[Wildcard Chaos]${NC}"
    # Create test files for wildcards
    touch test_wild_1.txt test_wild_2.txt test_wild_a.txt test_wild_b.txt

    run_test "simple wildcard" "ls test_wild_*.txt|wc -l"
    # run_test "wildcard with cat" "cat test_wild_*.txt 2>/dev/null||echo no_match"
    run_test "wildcard in quotes" "echo 'test_*.txt'"
    # run_test "wildcard no match" "ls no_match_*.xyz 2>/dev/null||echo no_files"
    run_test "multiple wildcards" "ls test_*.txt *.sh|grep test|wc -l"
    # run_test "wildcard with redirect" "cat test_wild_*.txt>test_out.txt 2>/dev/null&&wc -l<test_out.txt"

    # Cleanup wildcard test files
    rm -f test_wild_*.txt
}

test_ultimate_chaos() {
    echo -e "\n${BLUE}[ULTIMATE CHAOS]${NC}"
    run_test "everything 1" "echo \$USER|cat>test_out.txt&&cat<test_out.txt|grep \$USER"
    run_test "everything 3" "<$TESTFILE cat|grep World>test_out.txt&&cat test_out.txt||echo fail"
    run_test "everything 4" "echo 'test \$USER'|cat&&(true||false)&&echo success"
    run_test "everything 5" "(false||true)&&echo \$USER|cat|cat  |cat"
    run_test "everything 6" "echo test>test_1.txt&&cat<test_1.txt|cat>test_2.txt&&cat test_2.txt"
    run_test "pipe redirect logic quote var" "echo \"\$USER is\" 'testing'|cat>test_out.txt&&cat test_out.txt||echo failed"
    run_test "massive pipe chain" "echo start|cat|cat|cat  |cat|cat  |cat|cat|wc -w"
    run_test "logic redirect pipe combo" "true&&echo ok|cat>test_out.txt||echo fail&&cat test_out.txt"
}

test_edge_cases_extreme() {
    echo -e "\n${BLUE}[Extreme Edge Cases]${NC}"
    run_test "many spaces" "echo          hello          world"
    # run_test "tabs and spaces" "echo 	hello	world"
    run_test "empty quotes" "echo '' \"\" ''"
    run_test "only quotes" "echo ''"
    run_test "nested empty quotes" "echo '\"\"'"
    run_test "redirect to same file twice" "echo test > test_out.txt && cat test_out.txt > test_out.txt && cat test_out.txt"
    run_test "cat to itself" "cat test_out.txt > test_out.txt || echo cannot_do"
    run_test "many redirects same file" "echo a > test_out.txt > test_out.txt > test_out.txt && cat test_out.txt"
}

test_quote_escape_hell() {
    echo -e "\n${BLUE}[Quote Escape Hell]${NC}"
    run_test "backslash madness" "echo \\\\\\\\"
    run_test "dollar in single quotes" "echo '\$USER \$HOME \$PATH'"
    run_test "dollar in double quotes" "echo \"\$USER is \$USER\""
    run_test "empty var expansion" "echo prefix\$NONEXISTENTsuffix"
    run_test "quote concatenation" "echo hello\"world\"test'foo'bar"
    run_test "spaces in quotes" "echo \"   spaced   out   \""
}

test_redirect_edge_cases() {
    echo -e "\n${BLUE}[Redirect Edge Cases]${NC}"
    run_test "redirect with no command" ">test_out.txt"
    # run_test "input redirect nonexistent" "<nonexistent_file.txt cat 2>/dev/null||echo error"
    run_test "redirect in weird positions" "echo<test1.txt hello>test_out.txt&&cat test_out.txt"
    # run_test "multiple appends" "echo a>>test_out.txt&&echo b  >>test_out.txt&&echo c>>  test_out.txt&&cat test_out.txt"
    run_test "overwrite after read" "cat test1.txt>test1.txt||echo cannot_read_and_write"
}

test_builtin_edge_cases() {
    echo -e "\n${BLUE}[Builtin Edge Cases]${NC}"
    run_test "echo -n with newline" "echo -n hello&&echo world"
    run_test "echo -n multiple" "echo -n one&&echo -n two  &&echo three"
    # run_test "echo with -n and -e" "echo -n hello"
    run_test "pwd in pipe" "pwd|cat"
    run_test "env in pipe" "env|grep PATH|head -1"
    run_test "export in subshell" "(export TEST=value&&echo \$TEST)"
}

test_special_characters() {
    echo -e "\n${BLUE}[Special Characters]${NC}"
    run_test "ampersand in quotes" "echo 'one && two'"
    run_test "pipe in quotes" "echo 'one | two'"
    run_test "redirect in quotes" "echo 'output > file'"
    run_test "dollar question mark" "true && echo \$?"
}

test_exit_status_madness() {
    echo -e "\n${BLUE}[Exit Status Tests]${NC}"
    run_test "exit status after true" "true && echo \$?"
    run_test "exit status after false" "false || echo \$?"
    run_test "exit status in pipe" "false | echo \$?"
    run_test "multiple exit status" "echo \$? && echo \$? && echo \$?"
    run_test "exit status after command" "echo test && echo \$?"
    run_test "exit status with redirect" "echo test > test_out.txt && echo \$?"
    # run_test "exit status after fail" "cat nonexistent.txt 2>/dev/null || echo \$?"
    run_test "exit status in subshell" "(false) && echo \$? || echo \$?"
}

test_tokenization_hell() {
    echo -e "\n${BLUE}[Tokenization Hell]${NC}"
    run_test "no spaces around pipes" "echo test|cat|cat|cat"
    run_test "no spaces around redirects" "echo test>test_out.txt&&cat<test_out.txt"
    run_test "no spaces around operators" "echo one&&echo two||echo three"
    run_test "mixed spacing chaos" "echo test  |  cat|cat  |  cat"
    # run_test "tabs everywhere" "echo	test	|	cat"
    run_test "quotes touching operators" "echo 'test'|cat"
    run_test "quotes touching redirects" "echo 'test'>test_out.txt&&cat test_out.txt"
    run_test "variable touching quotes" "echo \$USER'test'"
    run_test "variable touching operator" "echo \$USER|cat"
    run_test "empty between operators" "echo test||echo other"
}

test_quote_tokenization() {
    echo -e "\n${BLUE}[Quote Tokenization Edge Cases]${NC}"
    run_test "empty quotes in command" "echo '' test '' more"
    run_test "only empty quotes" "echo '' ''"
    run_test "quotes with no spaces" "echo 'hello'\"world\"'test'"
    run_test "quotes around operators" "echo 'test | test'"
    run_test "quotes around redirects" "echo 'test > file'"
    run_test "single char in quotes" "echo 'a' 'b' 'c'"
    run_test "quote at start and end" "'echo' 'hello' 'world'"
    run_test "alternating quotes" "echo 'a'\"b\"'c'\"d\""
    run_test "quote after redirect" "echo test > 'test_out.txt' && cat test_out.txt"
}

test_variable_edge_cases() {
    echo -e "\n${BLUE}[Variable Tokenization Edge Cases]${NC}"
    run_test "dollar at end" "echo test\$"
    run_test "dollar with number" "echo \$1"
    run_test "variable with underscore" "echo \$USER_TEST"
    run_test "variable no space before text" "echo \$USER-suffix"
    run_test "variable no space after text" "echo prefix-\$USER"
    run_test "variable between quotes" "echo '\$USER'\"\$USER\"'\$USER'"
    run_test "empty variable expansion" "echo prefix\$NONEXIST_VAR_123suffix"
    run_test "multiple variables no space" "echo \$USER\$HOME\$PATH"
    run_test "dollar in different quotes" "echo '\$' \"\$\""
}

test_operator_edge_cases() {
    echo -e "\n${BLUE}[Operator Edge Cases]${NC}"
    run_test "multiple ands" "true && true && true && echo yes"
    run_test "multiple ors" "false || false || false || echo yes"
    run_test "and or mix no spaces" "true&&false||true&&echo yes"
    # run_test "operators with empty" "true && && echo test"
    run_test "pipe after redirect" "echo test > test_out.txt | cat"
    run_test "redirect after pipe" "echo test | cat > test_out.txt && cat test_out.txt"
    run_test "multiple redirects no space" "echo test>test_out.txt>test_out2.txt"
    run_test "redirect then logic" "echo test > test_out.txt && echo success || echo fail"
    run_test "logic then pipe" "true && echo test | cat"
}

test_redirection_edge_cases_extreme() {
    echo -e "\n${BLUE}[Extreme Redirection Edge Cases]${NC}"
    run_test "redirect with spaces in between" "echo test >   test_out.txt && cat test_out.txt"
    run_test "redirect before and after" ">test_out.txt cat test1.txt >test_out.txt"
    run_test "many output redirects" "echo test > a > b > c > d > e && cat e"
    run_test "input from nowhere" "< /dev/null cat"
    run_test "redirect to /dev/null" "echo test > /dev/null && echo success"
    # run_test "redirect stderr only" "cat nonexist 2> test_err.txt || cat test_err.txt"
    run_test "append then overwrite" "echo a >> test_out.txt && echo b > test_out.txt && cat test_out.txt"
    run_test "overwrite then append" "echo a > test_out.txt && echo b >> test_out.txt && cat test_out.txt"
}

test_parsing_errors() {
    echo -e "\n${BLUE}[Parsing Error Handling]${NC}"
    # run_test "unclosed single quote" "echo 'test"
    # run_test "unclosed double quote" "echo \"test"
    # run_test "pipe at start" "| echo test"
    # run_test "pipe at end" "echo test |"
    run_test "double pipe" "echo test || echo other"
    # run_test "redirect no file" "echo test >"
    run_test "redirect no command" "> test_out.txt"
    run_test "double redirect" "echo test >> test_out.txt"
    # run_test "only operators" "&&"
    # run_test "only pipe" "|"
    # run_test "empty parens" "()"
    # run_test "unclosed paren" "(echo test"
}

test_command_execution_edge_cases() {
    echo -e "\n${BLUE}[Command Execution Edge Cases]${NC}"
    # run_test "command not found" "nonexistentcommand123"
    run_test "empty command" ""
    run_test "only whitespace" "     "
    # run_test "only tabs" "		"
    run_test "command with path" "/bin/echo test"
    # run_test "relative path command" "./test_minishell.sh 2>/dev/null || echo not_executable"
    run_test "multiple slashes in path" "///bin///echo test"
}

test_builtin_stress() {
    echo -e "\n${BLUE}[Builtin Command Stress]${NC}"
    run_test "echo with many args" "echo a b c d e f g h i j k l m n o p q r s t u v w x y z"
    # run_test "echo with many flags" "echo -n -n -n test"
    run_test "echo empty string" "echo ''"
    run_test "echo only spaces" "echo '     '"
    run_test "pwd multiple times" "pwd && pwd && pwd | cat"
    run_test "env grep multiple" "env | grep PATH | grep home"
}

test_pipe_stress() {
    echo -e "\n${BLUE}[Pipe Stress Test]${NC}"
    run_test "10 pipes" "echo test|cat|cat|cat|cat|cat|cat|cat|cat|cat|cat"
    # run_test "pipe with empty middle" "echo test | | cat"
    run_test "pipes with various cmds" "echo hello | cat | grep hello | cat | wc -c"
    run_test "pipe into redirect into pipe" "echo test | cat > test_out.txt && cat test_out.txt | cat | cat"
    run_test "failing pipe chain" "false | echo test | cat"
}

test_memory_stress() {
    echo -e "\n${BLUE}[Memory/Long Input Stress]${NC}"
    run_test "very long echo" "echo aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    run_test "many variables" "echo \$USER \$HOME \$PATH \$USER \$HOME \$PATH \$USER \$HOME \$PATH"
    run_test "long variable expansion" "echo \$PATH\$PATH\$PATH"
    run_test "many empty quotes" "echo '' '' '' '' '' '' '' '' '' ''"
    # run_test "deeply nested parens" "((((echo test))))"
}

test_whitespace_chaos() {
    echo -e "\n${BLUE}[Whitespace Chaos]${NC}"
    run_test "leading spaces" "     echo test"
    run_test "trailing spaces" "echo test     "
    run_test "spaces everywhere" "  echo   test   |   cat   "
    # run_test "tab delimiters" "echo	test	|	cat"
    run_test "mixed tabs and spaces" " 	  echo 	test  	"
#     run_test "newline in quotes" "echo 'test
# test'"
}

test_quote_variable_interaction() {
    echo -e "\n${BLUE}[Quote-Variable Interaction]${NC}"
    run_test "var in single quotes" "echo '\$USER is \$USER'"
    run_test "var in double quotes" "echo \"\$USER is \$USER\""
    run_test "var between quotes" "echo 'pre'\$USER'post'"
    run_test "empty var in quotes" "echo \"\$NONEXIST\""
    run_test "dollar in single quotes" "echo 'test \$ sign'"
    run_test "dollar in double quotes" "echo \"test \$ sign\""
    run_test "quote inside var expansion" "echo \"\$USER'test'\""
    run_test "multiple vars in quotes" "echo \"\$USER and \$HOME\""
}

test_redirect_operator_interaction() {
    echo -e "\n${BLUE}[Redirect-Operator Interaction]${NC}"
    run_test "redirect before and" ">test_out.txt echo test && cat test_out.txt"
    run_test "redirect after or" "false || echo test > test_out.txt && cat test_out.txt"
    run_test "redirect between logic" "echo one > test_out.txt && echo two >> test_out.txt || echo three && cat test_out.txt"
    run_test "multiple redirect with logic" "echo a > test_out.txt && echo b > test_out.txt && echo c > test_out.txt && cat test_out.txt"
    run_test "redirect in parens with logic" "(echo test > test_out.txt) && cat test_out.txt"
}

test_heredoc_edge_cases() {
    echo -e "\n${BLUE}[Heredoc Edge Cases]${NC}"
#     run_test "simple heredoc" "cat << EOF
# test
# EOF"
    run_test "heredoc with quotes" "cat << 'EOF'
\$USER
EOF"
    run_test "heredoc with expansion" "cat << EOF
\$USER
EOF"
    run_test "heredoc empty" "cat << EOF
EOF"
#     run_test "multiple heredocs" "cat << EOF
# first
# EOF
# cat << EOF
# second
# EOF"
}

test_filename_edge_cases() {
    echo -e "\n${BLUE}[Filename Edge Cases]${NC}"
    # Create test files with special names
    touch "test file with spaces.txt"
    echo "content" > "test file with spaces.txt"
    touch "test-dashes.txt"
    echo "dashes" > "test-dashes.txt"

    run_test "file with spaces quoted" "cat 'test file with spaces.txt'"
    run_test "file with dashes" "cat test-dashes.txt"
    run_test "redirect to file with spaces" "echo test > 'file with spaces.txt' && cat 'file with spaces.txt' && rm 'file with spaces.txt'"
    run_test "multiple files with spaces" "cat 'test file with spaces.txt' 'test file with spaces.txt'"

    # Cleanup
    rm -f "test file with spaces.txt" "test-dashes.txt"
}

test_permission_edge_cases() {
    echo -e "\n${BLUE}[Permission Edge Cases]${NC}"
    # Create a non-readable file
    touch test_noperm.txt
    chmod 000 test_noperm.txt

    # run_test "no permission read" "cat test_noperm.txt 2>&1 | grep -i permission"
    # run_test "no permission write" "echo test > test_noperm.txt 2>&1 | grep -i permission"

    chmod 644 test_noperm.txt
    rm -f test_noperm.txt
}

test_signal_handling() {
    echo -e "\n${BLUE}[Signal/Control Flow]${NC}"
    run_test "exit in logic" "true && true && echo before_exit"
    run_test "multiple exits" "echo test && echo test2"
    run_test "command after false" "false && echo should_not_print"
    run_test "command after true" "true || echo should_not_print"
}

test_stress_test() {
    echo -e "\n${BLUE}[STRESS TEST - THE FINAL BOSS]${NC}"
    run_test "mega combo 2" "echo 'test \"nested\" quotes'|cat|cat  |cat|cat>test_out.txt&&<test_out.txt cat"
    run_test "mega combo 4" "echo start>test_1.txt&&cat test_1.txt|cat>test_2.txt&&cat test_2.txt|cat>test_3.txt&&cat test_3.txt"
    run_test "mega combo 5" "<$TESTFILE cat|grep World|cat|cat>test_out.txt&&<test_out.txt cat|cat  |cat"
}

# Main execution
main() {
    setup

    # Basic tests
    test_simple_commands
    test_pipes
    test_redirections
    test_builtins
    test_logical_operators
    test_quotes
    test_variables
    test_edge_cases
    test_combined

    # Crazy tests
    test_crazy_quotes
    test_redirect_who
    test_pipe_madness
    test_redirect_chaos
    test_logic_insanity
    test_parentheses_priority
    test_variable_expansion_madness
    test_wildcard_chaos
    test_edge_cases_extreme
    test_quote_escape_hell
    test_redirect_edge_cases
    test_builtin_edge_cases
    test_special_characters
    test_ultimate_chaos

    # NEW: Extreme robustness tests
    test_exit_status_madness
    test_tokenization_hell
    test_quote_tokenization
    test_variable_edge_cases
    test_operator_edge_cases
    test_redirection_edge_cases_extreme
    test_parsing_errors
    test_command_execution_edge_cases
    test_builtin_stress
    test_pipe_stress
    test_memory_stress
    test_whitespace_chaos
    test_quote_variable_interaction
    test_redirect_operator_interaction
    test_heredoc_edge_cases
    test_filename_edge_cases
    test_permission_edge_cases
    test_signal_handling
    test_stress_test

    cleanup
}

# Run tests
main
