# Minishell Builtin Tests

Comprehensive standalone test suite for all builtin functions in the minishell project.

## Overview

This test suite provides **independent C test programs** that test each builtin function (`ft_echo`, `ft_pwd`, `ft_cd`, `ft_env`, `ft_export`, `ft_unset`, `ft_exit`) directly without depending on the full minishell executable.

## Builtin Functions Tested

- **echo** - Display text with optional `-n` flag
- **pwd** - Print working directory
- **cd** - Change directory (with NULL, tilde expansion, relative/absolute paths)
- **env** - Display environment variables
- **export** - Set/update environment variables
- **unset** - Remove environment variables
- **exit** - Exit with specified code

## Prerequisites

Your project must have:
- `libft` compiled in `../libft/`
- Builtin function implementations in `../execution/builtins/`
- Helper functions in `../parsing/` (for `tab_realloc`)

## Quick Start

### Run All Tests

The simplest way to run all tests:

```bash
cd tests
bash run_tests.sh
```

This will:
1. Compile libft (if needed)
2. Compile all test programs
3. Run all tests
4. Display a summary

### Manual Compilation and Testing

If you prefer more control:

```bash
# Compile all tests
cd tests
make

# Run individual tests
./test_echo
./test_pwd
./test_cd
./test_env
./test_export
./test_unset
./test_exit

# Or run all at once
./run_all_tests.sh
```

### Clean Up

```bash
# Remove test executables only
make clean

# Remove test executables and clean libft
make fclean

# Rebuild everything
make re
```

## Test Output

Tests provide clear, color-coded output:
- **Green ✓**: Test passed
- **Red ✗**: Test failed

Each test suite shows:
- Individual test results
- Summary (total/passed/failed)
- Overall pass/fail status

### Example Output

```
========================================
  Testing FT_ECHO builtin
========================================

Test 1: echo hello
hello

Test 2: echo hello world
hello world

✓ echo with simple arguments (manual verification needed)
✓ echo with -n flag (manual verification needed)
✓ echo edge cases (manual verification needed)

========================================
  TEST SUMMARY
========================================
Total tests: 3
Passed: 3
Failed: 0

All tests passed!
```

## Test Coverage Details

### Echo Tests (`test_echo`)
- Simple echo with single/multiple arguments
- Echo with `-n` flag (no newline)
- Echo with no arguments
- Echo with empty strings
- Echo with special characters

### PWD Tests (`test_pwd`)
- Basic pwd functionality
- PWD after chdir operations
- PWD at root directory
- Return code validation

### CD Tests (`test_cd`)
- `cd(NULL)` - go to HOME
- Absolute paths (`/tmp`, `/`, `/usr`, `/etc`)
- Relative paths (`..`, `../../`)
- Tilde expansion (`~`, `~/.`)
- Non-existent directories (error handling)
- Trailing slashes
- Empty strings (error handling)

### ENV Tests (`test_env`)
- Display all environment variables
- Verify standard variables exist (PATH, HOME)
- Verify KEY=VALUE format

### Export Tests (`test_export`)
- Create new variables
- Update existing variables
- Export with empty values
- Export with special characters (colons, slashes)
- Invalid formats (starts with `=`, no `=` sign)
- Variable overwriting

### Unset Tests (`test_unset`)
- Remove single variable
- Remove multiple variables
- Unset non-existent variables (no crash)
- Preserve other variables
- Empty array handling

### Exit Tests (`test_exit`)
- Exit codes: 0, 1, 42, 255
- Exit code wrapping (>255)
- Uses fork() to test without terminating test process

## Architecture

Each test is a standalone C program that:

1. **Includes only what it needs**: `minishell.h`, specific builtin source
2. **Links with libft**: Uses your custom library functions
3. **Tests independently**: Doesn't require full minishell
4. **Provides detailed output**: Shows what's being tested and results

### Test Structure

```
tests/
├── README.md              # This file
├── Makefile              # Compiles all tests
├── run_tests.sh          # Main entry point (compile + run)
├── run_all_tests.sh      # Runs all compiled tests
├── test_echo.c           # Test for ft_echo
├── test_pwd.c            # Test for ft_pwd
├── test_cd.c             # Test for ft_cd
├── test_env.c            # Test for ft_env
├── test_export.c         # Test for ft_export (needs tab_realloc)
├── test_unset.c          # Test for ft_unset
└── test_exit.c           # Test for ft_exit
```

### Dependencies

Each test compiles with:
- Its specific builtin source file (`ft_*.c`)
- `libft.a` (for utility functions)
- Helper functions when needed (e.g., `tab_realloc` for export)

## Troubleshooting

### Compilation Errors

**Error: "minishell.h not found"**
- Make sure you're running tests from the `tests/` directory
- Check that `../minishell.h` exists

**Error: "libft.a not found"**
```bash
cd ../libft
make
cd ../tests
```

**Error: "undefined reference to ft_*"**
- Some functions might not be compiled
- Check that the builtin `.c` file exists in `../execution/builtins/`
- For export tests: ensure `parsing_utils2.c` exists

### Test Failures

**cd tests fail**
- Check that HOME environment variable is set
- Ensure you have permissions to cd to test directories

**export/unset tests fail**
- These tests create mock t_data structures
- Failures usually indicate issues with memory management

**exit tests fail**
- These use fork() to test exit codes
- Check that the exit code wrapping works correctly (modulo 256)

### Running Individual Tests with Debug

```bash
# Compile with debug symbols (already included)
make

# Run with lldb/gdb if needed
lldb ./test_cd
# or
gdb ./test_cd
```

## Adding New Tests

To add new test cases:

1. **Edit the test file** (e.g., `test_echo.c`)
2. **Add your test function**
   ```c
   void test_my_new_case(void)
   {
       // Your test code
       char *args[] = {"echo", "test", NULL};
       ft_echo(args);
       print_test_result("my test description", 1);
   }
   ```
3. **Call it from main()**
   ```c
   int main(void)
   {
       print_test_header("Testing FT_ECHO builtin");
       test_echo_simple();
       test_my_new_case();  // Add this
       // ... rest of main
   }
   ```
4. **Rebuild and test**
   ```bash
   make re
   ./test_echo
   ```

## Integration with Development Workflow

### Recommended Usage

```bash
# After implementing/modifying a builtin
cd tests
bash run_tests.sh

# Run specific test
make test_cd
./test_cd

# Clean and rebuild
make re
```

### CI/CD Integration

You can integrate these tests into your workflow:

```bash
#!/bin/bash
# In your project root
make -C tests
if [ $? -ne 0 ]; then
    echo "Tests failed!"
    exit 1
fi
```

## Notes

- Tests are designed to be **independent** of the full minishell
- Each test **directly calls** `ft_[builtin]()` functions
- Tests use **libft** functions (ft_printf, ft_strdup, etc.)
- Some tests require **manual verification** (especially echo output)
- Tests for `exit` use **fork()** to avoid terminating the test process

## Author Notes

These tests focus on:
1. **Correctness**: Do functions behave as expected?
2. **Error handling**: Do functions handle edge cases?
3. **Memory safety**: No leaks or crashes (run with valgrind)
4. **Independence**: Each builtin tested in isolation

Run with valgrind to check for memory leaks:
```bash
valgrind --leak-check=full ./test_export
```
