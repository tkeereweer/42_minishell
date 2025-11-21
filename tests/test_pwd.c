/* ************************************************************************** */
/*                                                                            */
/*   test_pwd.c                                                              */
/*   Standalone test for ft_pwd builtin                                      */
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

void test_pwd_basic(void)
{
    char expected[PATH_MAX];
    int ret;

    getcwd(expected, PATH_MAX);

    printf("Expected: %s\n", expected);
    printf("Got:      ");
    ret = ft_pwd();
    printf("\n");

    if (ret == 0)
        print_test_result("pwd returns success code", 1);
    else
        print_test_result("pwd returns success code", 0);
}

void test_pwd_after_chdir(void)
{
    char original[PATH_MAX];
    char expected[PATH_MAX];
    int ret;

    getcwd(original, PATH_MAX);

    chdir("/tmp");
    getcwd(expected, PATH_MAX);

    printf("After chdir to /tmp:\n");
    printf("Expected: %s\n", expected);
    printf("Got:      ");
    ret = ft_pwd();
    printf("\n");

    chdir(original);  // Restore

    if (ret == 0)
        print_test_result("pwd after chdir to /tmp", 1);
    else
        print_test_result("pwd after chdir to /tmp", 0);
}

void test_pwd_at_root(void)
{
    char original[PATH_MAX];
    int ret;

    getcwd(original, PATH_MAX);

    chdir("/");

    printf("After chdir to /:\n");
    printf("Expected: /\n");
    printf("Got:      ");
    ret = ft_pwd();
    printf("\n");

    chdir(original);  // Restore

    if (ret == 0)
        print_test_result("pwd at root directory", 1);
    else
        print_test_result("pwd at root directory", 0);
}

int main(void)
{
    print_test_header("Testing FT_PWD builtin");

    test_pwd_basic();
    test_pwd_after_chdir();
    test_pwd_at_root();

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
