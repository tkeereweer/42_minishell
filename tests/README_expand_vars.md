# Variable Expansion Tests

Comprehensive tests for the `expand_vars` function which handles quote removal, environment variable expansion, and wildcard expansion.

## What expand_vars Does

The `expand_vars` function processes command arguments and performs:

1. **Quote Removal** - Removes single (`'`) and double (`"`) quotes
2. **Variable Expansion** - Expands `$VAR` to environment variable values
3. **Wildcard Expansion** - Expands `*` patterns to matching filenames

### Quote Behavior

- **Single quotes `'$VAR'`**: No expansion (treated literally)
- **Double quotes `"$VAR"`**: Variable expansion but no wildcard expansion
- **No quotes `$VAR`**: Both variable and wildcard expansion

## Running the Tests

```bash
cd tests
make test_expand_vars
./test_expand_vars
```

Or run with all other tests:
```bash
bash run_tests.sh
```

## Test Coverage

### ✅ Tests That Pass (12/14)

1. **Simple variable expansion** - `$HOME` → `/home/user`
2. **Multiple variables** - `$USER` and `$HOME` in separate args
3. **Single quotes (no expansion)** - `'$HOME'` → `$HOME`
4. **Double quotes (variable expansion)** - `"$USER"` → `testuser`
5. **Non-existent variable** - `$NONEXISTENT` → empty string
6. **Empty variable** - `$EMPTY` → empty string
7. **Wildcard expansion** - `test_*.txt` → `test_file1.txt test_file2.txt`
8. **Wildcard in double quotes** - `"*.txt"` → `*.txt` (no expansion)
9. **Mixed quotes and variables** - Correct handling of different quote types
10. **No expansion needed** - Regular text passes through unchanged
11. **Variable with spaces around** - `a $USER b` → `a testuser b`
12. **Variable with underscore** - `$USER_suffix` → empty (correct bash behavior)

### ⚠️ Known Limitations vs Bash (2/14)

These tests fail because your implementation doesn't match bash behavior:

1. **Variable within text without spaces**
   - Input: `hello$USERworld`
   - Expected (bash): `hellotestuserworld`
   - Actual: `hello$USERworld` (not expanded)
   - **Reason**: `has_envvar()` only finds variables at the start or after a space
   - **Bash**: Expands variables anywhere in text

2. **Multiple variables in one argument**
   - Input: `$USER:$HOME`
   - Expected (bash): `testuser:/home/user`
   - Actual: Only partial expansion
   - **Reason**: After first expansion, second variable may not be detected properly
   - **Bash**: Expands all variables in the string

**Note on variable names:** Your implementation correctly treats `$USER_suffix` as one variable name (matches bash). To expand `$USER` and keep the suffix in bash, you'd use `${USER}_suffix`, but brace expansion is optional for a minishell project.

## Bug Fixed During Testing

**Critical bug found and fixed in `expand_vars` (wildcards.c:143):**

The `mode` variable was never initialized or reset between loop iterations. This caused strings without quotes to have `remove_quotes()` called on them incorrectly, corrupting the output.

**Fix:** Added `mode = 0;` at the start of each loop iteration.

## Implementation Details

### Variable Detection (`has_envvar`)

The function detects variables in two cases:
- At the beginning: `$VAR...`
- After a space: `text $VAR...`

It does NOT detect:
- Variables in the middle: `prefix$VARsuffix`
- Variables after other characters: `prefix:$VAR`

### Variable Length (`envvar_len`)

Stops reading the variable name at:
- Whitespace
- Another `$`
- End of string

Does NOT stop at other delimiters like `:`, `_`, `/`, etc.

## Suggested Improvements

If you want to improve variable expansion to match bash more closely:

1. **Detect variables anywhere in the string:**
   ```c
   // Current: only finds variables at start or after space
   // Better: find variables anywhere with ft_strchr(str, '$')
   ```

2. **Stop variable names at proper delimiters:**
   ```c
   // Current: stops at space, $, or end
   // Better: stop at any non-alphanumeric character except underscore
   // Variable names can be: [A-Za-z_][A-Za-z0-9_]*
   ```

3. **Handle multiple variables in one argument:**
   ```c
   // Recursively expand all variables in the string
   // Already done with expand_envvars(), but needs better detection
   ```

## Testing with Real Files

The wildcard test creates temporary files:
- `test_file1.txt`
- `test_file2.txt`

These are automatically cleaned up after the test.

## Memory Leak Testing

Run with valgrind to check for memory leaks:
```bash
valgrind --leak-check=full ./test_expand_vars
```

## Integration with Minishell

These tests are independent of the full minishell executable. They test the `expand_vars` function in isolation with mock data.

In your actual minishell, ensure:
1. Environment is properly initialized
2. Quote handling works with your lexer/parser
3. Wildcard expansion works in the current directory
4. Error handling doesn't crash the shell

## Test Environment

Mock environment used in tests:
- `HOME=/home/user`
- `USER=testuser`
- `PATH=/usr/bin:/bin`
- `EMPTY=` (empty value)
- `NUM=42`
