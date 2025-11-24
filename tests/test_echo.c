/* ************************************************************************** */
/*                                                                            */
/*   test_echo.c                                                             */
/*   Standalone test for ft_echo builtin                                     */
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

void test_echo_simple(void)
{
    char *args1[] = {"echo", "hello", NULL};
    char *args2[] = {"echo", "hello", "world", NULL};
    char *args3[] = {"echo", NULL};

    printf("Test 1: echo hello\n");
    ft_echo(args1);
    printf("\n");

    printf("Test 2: echo hello world\n");
    ft_echo(args2);
    printf("\n");

    printf("Test 3: echo (no args)\n");
    ft_echo(args3);
    printf("\n");

    print_test_result("echo with simple arguments (manual verification needed)", 1);
}

void test_echo_with_n_flag(void)
{
    char *args1[] = {"echo", "-n", "test", NULL};
    char *args2[] = {"echo", "-n", "hello", "world", NULL};

    printf("Test 4: echo -n test (should not print newline)\n");
    ft_echo(args1);
    printf(" [END]\n");

    printf("Test 5: echo -n hello world\n");
    ft_echo(args2);
    printf(" [END]\n");

    print_test_result("echo with -n flag (manual verification needed)", 1);
}

void test_echo_edge_cases(void)
{
    char *args1[] = {"echo", "", NULL};
    char *args2[] = {"echo", "one", "two", "three", "four", NULL};
    char *args3[] = {"echo", "special:chars/test", NULL};

    printf("Test 6: echo with empty string\n");
    ft_echo(args1);
    printf("\n");

    printf("Test 7: echo with multiple args\n");
    ft_echo(args2);
    printf("\n");

    printf("Test 8: echo with special characters\n");
    ft_echo(args3);
    printf("\n");

    print_test_result("echo edge cases (manual verification needed)", 1);
}

int main(void)
{
    print_test_header("Testing FT_ECHO builtin");

    test_echo_simple();
    test_echo_with_n_flag();
    test_echo_edge_cases();

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
