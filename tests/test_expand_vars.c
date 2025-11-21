/* ************************************************************************** */
/*                                                                            */
/*   test_expand_vars.c                                                      */
/*   Standalone test for expand_vars function                                */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>

int g_test_count = 0;
int g_pass_count = 0;
int g_fail_count = 0;

void print_test_header(const char *title)
{
    printf("\n========================================\n");
    printf("  %s\n", title);
    printf("========================================\n");
}

void print_test_result(const char *test_name, int passed)
{
    g_test_count++;
    if (passed)
    {
        printf("\033[0;32m✓\033[0m %s\n", test_name);
        g_pass_count++;
    }
    else
    {
        printf("\033[0;31m✗\033[0m %s\n", test_name);
        g_fail_count++;
    }
}

t_data *create_test_data(void)
{
    t_data *data;

    data = (t_data *)malloc(sizeof(t_data));
    if (!data)
        return (NULL);

    data->env = (char **)malloc(sizeof(char *) * 6);
    if (!data->env)
    {
        free(data);
        return (NULL);
    }

    data->env[0] = ft_strdup("HOME=/home/user");
    data->env[1] = ft_strdup("USER=testuser");
    data->env[2] = ft_strdup("PATH=/usr/bin:/bin");
    data->env[3] = ft_strdup("EMPTY=");
    data->env[4] = ft_strdup("NUM=42");
    data->env[5] = NULL;
    data->tree = NULL;

    return (data);
}

void free_test_data(t_data *data)
{
    int i;

    if (!data)
        return;

    i = 0;
    while (data->env && data->env[i])
    {
        free(data->env[i]);
        i++;
    }
    if (data->env)
        free(data->env);
    free(data);
}

void free_tab(char **tab)
{
    int i;

    if (!tab)
        return;

    i = 0;
    while (tab[i])
    {
        free(tab[i]);
        i++;
    }
    free(tab);
}

char **create_tab(int count, ...)
{
    va_list args;
    char **tab;
    int i;

    tab = (char **)malloc(sizeof(char *) * (count + 1));
    if (!tab)
        return (NULL);

    va_start(args, count);
    i = 0;
    while (i < count)
    {
        tab[i] = ft_strdup(va_arg(args, char *));
        i++;
    }
    tab[i] = NULL;
    va_end(args);

    return (tab);
}

void print_tab(char **tab)
{
    int i = 0;
    while (tab[i])
    {
        printf("    [%d]: \"%s\"\n", i, tab[i]);
        i++;
    }
}

void test_simple_variable_expansion(void)
{
    t_data *data;
    char **tab;
    int ret;

    printf("\n--- Test 1: Simple variable expansion ---\n");
    data = create_test_data();
    tab = create_tab(2, "echo", "$HOME");

    printf("Before:\n");
    print_tab(tab);

    ret = expand_vars(&tab, data);

    printf("After:\n");
    print_tab(tab);

    if (ret == 0 && tab[1] && ft_strncmp(tab[1], "/home/user", 11) == 0)
        print_test_result("expand $HOME", 1);
    else
        print_test_result("expand $HOME", 0);

    free_tab(tab);
    free_test_data(data);
}

void test_multiple_variables(void)
{
    t_data *data;
    char **tab;
    int ret;

    printf("\n--- Test 2: Multiple variables ---\n");
    data = create_test_data();
    tab = create_tab(3, "echo", "$USER", "$HOME");

    printf("Before:\n");
    print_tab(tab);

    ret = expand_vars(&tab, data);

    printf("After:\n");
    print_tab(tab);

    if (ret == 0 && tab[1] && ft_strncmp(tab[1], "testuser", 9) == 0 &&
        tab[2] && ft_strncmp(tab[2], "/home/user", 11) == 0)
        print_test_result("expand multiple variables", 1);
    else
        print_test_result("expand multiple variables", 0);

    free_tab(tab);
    free_test_data(data);
}

void test_variable_in_text(void)
{
    t_data *data;
    char **tab;
    int ret;

    printf("\n--- Test 3: Variable within text ---\n");
    data = create_test_data();
    tab = create_tab(2, "echo", "hello$USERworld");

    printf("Before:\n");
    print_tab(tab);

    ret = expand_vars(&tab, data);

    printf("After:\n");
    print_tab(tab);

    if (ret == 0 && tab[1] && ft_strncmp(tab[1], "hellotestuserworld", 19) == 0)
        print_test_result("expand variable in text", 1);
    else
        print_test_result("expand variable in text", 0);

    free_tab(tab);
    free_test_data(data);
}

void test_single_quotes_no_expansion(void)
{
    t_data *data;
    char **tab;
    int ret;

    printf("\n--- Test 4: Single quotes (no expansion) ---\n");
    data = create_test_data();
    tab = create_tab(2, "echo", "'$HOME'");

    printf("Before:\n");
    print_tab(tab);

    ret = expand_vars(&tab, data);

    printf("After:\n");
    print_tab(tab);

    // Single quotes should be removed but variable not expanded
    if (ret == 0 && tab[1] && ft_strncmp(tab[1], "$HOME", 6) == 0)
        print_test_result("single quotes prevent expansion", 1);
    else
        print_test_result("single quotes prevent expansion", 0);

    free_tab(tab);
    free_test_data(data);
}

void test_double_quotes_var_expansion(void)
{
    t_data *data;
    char **tab;
    int ret;

    printf("\n--- Test 5: Double quotes (variable expansion) ---\n");
    data = create_test_data();
    tab = create_tab(2, "echo", "\"$USER\"");

    printf("Before:\n");
    print_tab(tab);

    ret = expand_vars(&tab, data);

    printf("After:\n");
    print_tab(tab);

    // Double quotes should be removed and variable expanded
    if (ret == 0 && tab[1] && ft_strncmp(tab[1], "testuser", 9) == 0)
        print_test_result("double quotes allow variable expansion", 1);
    else
        print_test_result("double quotes allow variable expansion", 0);

    free_tab(tab);
    free_test_data(data);
}

void test_nonexistent_variable(void)
{
    t_data *data;
    char **tab;
    int ret;

    printf("\n--- Test 6: Non-existent variable ---\n");
    data = create_test_data();
    tab = create_tab(2, "echo", "$NONEXISTENT");

    printf("Before:\n");
    print_tab(tab);

    ret = expand_vars(&tab, data);

    printf("After:\n");
    print_tab(tab);

    // Non-existent variable should expand to empty string
    if (ret == 0 && tab[1] && ft_strlen(tab[1]) == 0)
        print_test_result("non-existent variable expands to empty", 1);
    else
        print_test_result("non-existent variable expands to empty", 0);

    free_tab(tab);
    free_test_data(data);
}

void test_empty_variable(void)
{
    t_data *data;
    char **tab;
    int ret;

    printf("\n--- Test 7: Empty variable ---\n");
    data = create_test_data();
    tab = create_tab(2, "echo", "$EMPTY");

    printf("Before:\n");
    print_tab(tab);

    ret = expand_vars(&tab, data);

    printf("After:\n");
    print_tab(tab);

    // Empty variable should expand to empty string
    if (ret == 0 && tab[1] && ft_strlen(tab[1]) == 0)
        print_test_result("empty variable expands to empty string", 1);
    else
        print_test_result("empty variable expands to empty string", 0);

    free_tab(tab);
    free_test_data(data);
}

void test_multiple_vars_in_one_arg(void)
{
    t_data *data;
    char **tab;
    int ret;

    printf("\n--- Test 8: Multiple variables in one argument ---\n");
    data = create_test_data();
    tab = create_tab(2, "echo", "$USER:$HOME");

    printf("Before:\n");
    print_tab(tab);

    ret = expand_vars(&tab, data);

    printf("After:\n");
    print_tab(tab);

    if (ret == 0 && tab[1] && ft_strncmp(tab[1], "testuser:/home/user", 20) == 0)
        print_test_result("multiple variables in one arg", 1);
    else
        print_test_result("multiple variables in one arg", 0);

    free_tab(tab);
    free_test_data(data);
}

void test_wildcard_expansion(void)
{
    t_data *data;
    char **tab;
    int ret;
    int fd;

    printf("\n--- Test 9: Wildcard expansion ---\n");

    // Create test files
    fd = open("test_file1.txt", O_CREAT | O_WRONLY, 0644);
    close(fd);
    fd = open("test_file2.txt", O_CREAT | O_WRONLY, 0644);
    close(fd);

    data = create_test_data();
    tab = create_tab(2, "echo", "test_*.txt");

    printf("Before:\n");
    print_tab(tab);

    ret = expand_vars(&tab, data);

    printf("After:\n");
    print_tab(tab);

    // Check that wildcard expanded to multiple files
    int count = 0;
    while (tab[count])
        count++;

    if (ret == 0 && count >= 3)  // echo + at least 2 files
        print_test_result("wildcard expansion", 1);
    else
        print_test_result("wildcard expansion (may fail if no matches)", 0);

    // Cleanup
    unlink("test_file1.txt");
    unlink("test_file2.txt");
    free_tab(tab);
    free_test_data(data);
}

void test_wildcard_in_double_quotes(void)
{
    t_data *data;
    char **tab;
    int ret;

    printf("\n--- Test 10: Wildcard in double quotes (no expansion) ---\n");
    data = create_test_data();
    tab = create_tab(2, "echo", "\"*.txt\"");

    printf("Before:\n");
    print_tab(tab);

    ret = expand_vars(&tab, data);

    printf("After:\n");
    print_tab(tab);

    // Double quotes should prevent wildcard expansion
    if (ret == 0 && tab[1] && ft_strncmp(tab[1], "*.txt", 6) == 0)
        print_test_result("double quotes prevent wildcard expansion", 1);
    else
        print_test_result("double quotes prevent wildcard expansion", 0);

    free_tab(tab);
    free_test_data(data);
}

void test_mixed_quotes_and_vars(void)
{
    t_data *data;
    char **tab;
    int ret;

    printf("\n--- Test 11: Mixed quotes and variables ---\n");
    data = create_test_data();
    tab = create_tab(4, "echo", "\"$USER\"", "'$HOME'", "$PATH");

    printf("Before:\n");
    print_tab(tab);

    ret = expand_vars(&tab, data);

    printf("After:\n");
    print_tab(tab);

    if (ret == 0 && tab[1] && ft_strncmp(tab[1], "testuser", 9) == 0 &&
        tab[2] && ft_strncmp(tab[2], "$HOME", 6) == 0 &&
        tab[3] && ft_strncmp(tab[3], "/usr/bin:/bin", 14) == 0)
        print_test_result("mixed quotes and variables", 1);
    else
        print_test_result("mixed quotes and variables", 0);

    free_tab(tab);
    free_test_data(data);
}

void test_no_expansion_needed(void)
{
    t_data *data;
    char **tab;
    int ret;

    printf("\n--- Test 12: No expansion needed ---\n");
    data = create_test_data();
    tab = create_tab(3, "echo", "hello", "world");

    printf("Before:\n");
    print_tab(tab);

    ret = expand_vars(&tab, data);

    printf("After:\n");
    print_tab(tab);

    if (ret == 0 && tab[1] && ft_strncmp(tab[1], "hello", 6) == 0 &&
        tab[2] && ft_strncmp(tab[2], "world", 6) == 0)
        print_test_result("no expansion needed", 1);
    else
        print_test_result("no expansion needed", 0);

    free_tab(tab);
    free_test_data(data);
}

void test_variable_at_start(void)
{
    t_data *data;
    char **tab;
    int ret;

    printf("\n--- Test 13: Non-existent variable with underscore ---\n");
    data = create_test_data();
    tab = create_tab(2, "echo", "$USER_suffix");

    printf("Before:\n");
    print_tab(tab);

    ret = expand_vars(&tab, data);

    printf("After:\n");
    print_tab(tab);

    // $USER_suffix should be treated as one variable name (doesn't exist)
    // This matches bash behavior - use ${USER}_suffix to separate
    if (ret == 0 && tab[1] && ft_strlen(tab[1]) == 0)
        print_test_result("variable name includes underscore (bash behavior)", 1);
    else
        print_test_result("variable name includes underscore (bash behavior)", 0);

    free_tab(tab);
    free_test_data(data);
}

void test_variable_with_spaces(void)
{
    t_data *data;
    char **tab;
    int ret;

    printf("\n--- Test 14: Variable with spaces around ---\n");
    data = create_test_data();
    tab = create_tab(2, "echo", "a $USER b");

    printf("Before:\n");
    print_tab(tab);

    ret = expand_vars(&tab, data);

    printf("After:\n");
    print_tab(tab);

    if (ret == 0 && tab[1] && ft_strncmp(tab[1], "a testuser b", 13) == 0)
        print_test_result("variable with spaces around", 1);
    else
        print_test_result("variable with spaces around", 0);

    free_tab(tab);
    free_test_data(data);
}

int main(void)
{
    print_test_header("Testing EXPAND_VARS function");

    test_simple_variable_expansion();
    test_multiple_variables();
    test_variable_in_text();
    test_single_quotes_no_expansion();
    test_double_quotes_var_expansion();
    test_nonexistent_variable();
    test_empty_variable();
    test_multiple_vars_in_one_arg();
    test_wildcard_expansion();
    test_wildcard_in_double_quotes();
    test_mixed_quotes_and_vars();
    test_no_expansion_needed();
    test_variable_at_start();
    test_variable_with_spaces();

    printf("\n========================================\n");
    printf("  TEST SUMMARY\n");
    printf("========================================\n");
    printf("Total tests: %d\n", g_test_count);
    printf("\033[0;32mPassed: %d\033[0m\n", g_pass_count);
    printf("\033[0;31mFailed: %d\033[0m\n", g_fail_count);

    if (g_fail_count == 0)
        printf("\n\033[0;32mAll tests passed!\033[0m\n");
    else
        printf("\n\033[0;31mSome tests failed.\033[0m\n");

    return (g_fail_count == 0 ? 0 : 1);
}
