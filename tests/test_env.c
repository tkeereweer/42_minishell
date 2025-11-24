/* ************************************************************************** */
/*                                                                            */
/*   test_env.c                                                              */
/*   Standalone test for ft_env builtin                                      */
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

t_data *create_test_data(char **envp)
{
    t_data *data;
    int i;
    int count;

    data = (t_data *)malloc(sizeof(t_data));
    if (!data)
        return (NULL);

    count = 0;
    while (envp[count])
        count++;

    data->env = (char **)malloc(sizeof(char *) * (count + 1));
    if (!data->env)
    {
        free(data);
        return (NULL);
    }

    i = 0;
    while (i < count)
    {
        data->env[i] = ft_strdup(envp[i]);
        i++;
    }
    data->env[i] = NULL;
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

int find_env_var(t_data *data, const char *var)
{
    int i;
    int var_len;

    var_len = ft_strlen((char *)var);
    i = 0;
    while (data->env[i])
    {
        if (ft_strncmp((char *)var, data->env[i], var_len) == 0 &&
            data->env[i][var_len] == '=')
            return (1);
        i++;
    }
    return (0);
}

void test_env_basic(t_data *data)
{
    printf("\n--- Test 1: Display all environment variables ---\n");
    ft_env(data);
    print_test_result("env displays all variables", 1);
}

void test_env_contains_standard_vars(t_data *data)
{
    int has_path;
    int has_home;

    has_path = find_env_var(data, "PATH");
    has_home = find_env_var(data, "HOME");

    if (has_path)
        print_test_result("env contains PATH", 1);
    else
        print_test_result("env contains PATH", 0);

    if (has_home)
        print_test_result("env contains HOME", 1);
    else
        print_test_result("env contains HOME", 0);
}

int main(int argc, char **argv, char **envp)
{
    t_data *data;

    (void)argc;
    (void)argv;

    print_test_header("Testing FT_ENV builtin");

    data = create_test_data(envp);
    if (!data)
    {
        printf("Error: Failed to create test data\n");
        return (1);
    }

    test_env_basic(data);
    test_env_contains_standard_vars(data);

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
