/* ************************************************************************** */
/*                                                                            */
/*   test_export.c                                                           */
/*   Standalone test for ft_export builtin                                   */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <string.h>

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

    data->env = (char **)malloc(sizeof(char *) * 5);
    if (!data->env)
    {
        free(data);
        return (NULL);
    }

    data->env[0] = ft_strdup("PATH=/usr/bin");
    data->env[1] = ft_strdup("HOME=/home/user");
    data->env[2] = ft_strdup("USER=testuser");
    data->env[3] = NULL;
    data->tree = NULL;

    return (data);
}

void free_test_data(t_data *data)
{
    int i;

    if (!data)
        return;

    i = 0;
    while (data->env[i])
    {
        free(data->env[i]);
        i++;
    }
    free(data->env);
    free(data);
}

int find_env_var(t_data *data, const char *var, const char *expected_value)
{
    int i;
    int var_len;

    var_len = ft_strlen((char *)var);
    i = 0;
    while (data->env[i])
    {
        if (ft_strncmp((char *)var, data->env[i], var_len) == 0 &&
            data->env[i][var_len] == '=')
        {
            if (expected_value)
            {
                int expected_len = ft_strlen((char *)expected_value);
                return (ft_strncmp(&data->env[i][var_len + 1], (char *)expected_value, expected_len + 1) == 0);
            }
            return (1);
        }
        i++;
    }
    return (0);
}

void test_export_new_variable(t_data *data)
{
    int ret;

    printf("\n--- Test 1: Export new variable ---\n");
    ret = ft_export("MY_VAR=test123", data);

    if (ret == 0 && find_env_var(data, "MY_VAR", "test123"))
        print_test_result("export creates new variable MY_VAR=test123", 1);
    else
        print_test_result("export creates new variable MY_VAR=test123", 0);
}

void test_export_update_variable(t_data *data)
{
    int ret;

    printf("\n--- Test 2: Update existing variable ---\n");
    ret = ft_export("PATH=/new/path", data);

    if (ret == 0 && find_env_var(data, "PATH", "/new/path"))
        print_test_result("export updates PATH to /new/path", 1);
    else
        print_test_result("export updates PATH to /new/path", 0);
}

void test_export_empty_value(t_data *data)
{
    int ret;

    printf("\n--- Test 3: Export with empty value ---\n");
    ret = ft_export("EMPTY_VAR=", data);

    if (ret == 0 && find_env_var(data, "EMPTY_VAR", ""))
        print_test_result("export creates EMPTY_VAR with empty value", 1);
    else
        print_test_result("export creates EMPTY_VAR with empty value", 0);
}

void test_export_special_chars(t_data *data)
{
    int ret;

    printf("\n--- Test 4: Export with special characters ---\n");
    ret = ft_export("SPECIAL=/usr/bin:/usr/local/bin", data);

    if (ret == 0 && find_env_var(data, "SPECIAL", "/usr/bin:/usr/local/bin"))
        print_test_result("export with colons in value", 1);
    else
        print_test_result("export with colons in value", 0);
}

void test_export_invalid_format(t_data *data)
{
    int ret;

    printf("\n--- Test 5: Export invalid format (starts with =) ---\n");
    ret = ft_export("=invalid", data);

    if (ret != 0)
        print_test_result("export rejects =invalid", 1);
    else
        print_test_result("export rejects =invalid", 0);
}

void test_export_no_equals(t_data *data)
{
    int ret;

    printf("\n--- Test 6: Export without = sign ---\n");
    ret = ft_export("NOEQUALS", data);

    if (ret != 0)
        print_test_result("export rejects NOEQUALS (no =)", 1);
    else
        print_test_result("export rejects NOEQUALS (no =)", 0);
}

void test_export_overwrite(t_data *data)
{
    int ret1, ret2;

    printf("\n--- Test 7: Export overwrite check ---\n");
    ret1 = ft_export("OVERWRITE=first", data);
    ret2 = ft_export("OVERWRITE=second", data);

    if (ret1 == 0 && ret2 == 0 && find_env_var(data, "OVERWRITE", "second"))
        print_test_result("export overwrites previous value", 1);
    else
        print_test_result("export overwrites previous value", 0);
}

int main(void)
{
    t_data *data;

    print_test_header("Testing FT_EXPORT builtin");

    data = create_test_data();
    if (!data)
    {
        printf("Error: Failed to create test data\n");
        return (1);
    }

    test_export_new_variable(data);
    test_export_update_variable(data);
    test_export_empty_value(data);
    test_export_special_chars(data);
    test_export_invalid_format(data);
    test_export_no_equals(data);
    test_export_overwrite(data);

    free_test_data(data);

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
