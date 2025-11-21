/* ************************************************************************** */
/*                                                                            */
/*   test_unset.c                                                            */
/*   Standalone test for ft_unset builtin                                    */
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

    data->env = (char **)malloc(sizeof(char *) * 6);
    if (!data->env)
    {
        free(data);
        return (NULL);
    }

    data->env[0] = ft_strdup("PATH=/usr/bin");
    data->env[1] = ft_strdup("HOME=/home/user");
    data->env[2] = ft_strdup("USER=testuser");
    data->env[3] = ft_strdup("VAR1=value1");
    data->env[4] = ft_strdup("VAR2=value2");
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

void test_unset_single_var(t_data *data)
{
    char *to_unset[] = {"VAR1", NULL};

    printf("\n--- Test 1: Unset single variable ---\n");
    printf("Before: ");
    if (find_env_var(data, "VAR1"))
        printf("VAR1 exists\n");

    ft_unset(to_unset, data);

    printf("After:  ");
    if (!find_env_var(data, "VAR1"))
    {
        printf("VAR1 removed\n");
        print_test_result("unset removes VAR1", 1);
    }
    else
    {
        printf("VAR1 still exists\n");
        print_test_result("unset removes VAR1", 0);
    }
}

void test_unset_multiple_vars(t_data *data)
{
    char *to_unset[] = {"HOME", "USER", NULL};

    printf("\n--- Test 2: Unset multiple variables ---\n");
    ft_unset(to_unset, data);

    if (!find_env_var(data, "HOME") && !find_env_var(data, "USER"))
        print_test_result("unset removes HOME and USER", 1);
    else
        print_test_result("unset removes HOME and USER", 0);
}

void test_unset_nonexistent(t_data *data)
{
    char *to_unset[] = {"NONEXISTENT_VAR", NULL};

    printf("\n--- Test 3: Unset non-existent variable ---\n");
    ft_unset(to_unset, data);
    print_test_result("unset handles non-existent var without crash", 1);
}

void test_unset_preserves_other_vars(void)
{
    t_data *data;
    char *to_unset[] = {"VAR1", NULL};

    printf("\n--- Test 4: Unset preserves other variables ---\n");

    data = create_test_data();
    if (!data)
    {
        print_test_result("unset preserves other variables", 0);
        return;
    }

    ft_unset(to_unset, data);

    if (find_env_var(data, "PATH") && find_env_var(data, "VAR2") &&
        !find_env_var(data, "VAR1"))
        print_test_result("unset preserves PATH and VAR2, removes VAR1", 1);
    else
        print_test_result("unset preserves PATH and VAR2, removes VAR1", 0);

    free_test_data(data);
}

void test_unset_empty_array(t_data *data)
{
    char *to_unset[] = {NULL};

    printf("\n--- Test 5: Unset with empty array ---\n");
    ft_unset(to_unset, data);
    print_test_result("unset handles empty array", 1);
}

int main(void)
{
    t_data *data;

    print_test_header("Testing FT_UNSET builtin");

    data = create_test_data();
    if (!data)
    {
        printf("Error: Failed to create test data\n");
        return (1);
    }

    test_unset_single_var(data);
    test_unset_multiple_vars(data);
    test_unset_nonexistent(data);
    test_unset_preserves_other_vars();
    test_unset_empty_array(data);

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
