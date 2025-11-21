/* ************************************************************************** */
/*                                                                            */
/*   test_cd.c                                                               */
/*   Standalone test for ft_cd builtin                                       */
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

void run_test(const char *test_name, char *path, int should_succeed)
{
    char original_dir[PATH_MAX];
    char new_dir[PATH_MAX];
    int ret;

    getcwd(original_dir, PATH_MAX);
    ret = ft_cd(path);

    if (should_succeed)
    {
        if (ret == 0)
        {
            getcwd(new_dir, PATH_MAX);
            printf("  -> Changed to: %s\n", new_dir);
            print_test_result(test_name, 1);
        }
        else
        {
            printf("  -> Expected success but got error\n");
            print_test_result(test_name, 0);
        }
    }
    else
    {
        if (ret == -1)
        {
            printf("  -> Failed as expected\n");
            print_test_result(test_name, 1);
        }
        else
        {
            printf("  -> Expected failure but succeeded\n");
            print_test_result(test_name, 0);
        }
    }

    chdir(original_dir);
}

int main(void)
{
    char *home;

    print_test_header("Testing FT_CD builtin");

    home = getenv("HOME");
    printf("HOME environment: %s\n\n", home ? home : "(not set)");

    run_test("cd(NULL) - go to HOME", NULL, home != NULL);
    run_test("cd to /tmp", "/tmp", 1);
    run_test("cd to / (root)", "/", 1);
    run_test("cd to ..", "..", 1);
    run_test("cd to non-existent directory", "/nonexistent_xyz123", 0);

    if (home)
    {
        run_test("cd with tilde (~)", "~", 1);
        run_test("cd with ~/.", "~/.", 1);
    }
    else
    {
        printf("Skipping tilde tests (HOME not set)\n");
    }

    run_test("cd to /usr", "/usr", 1);
    run_test("cd to /etc", "/etc", 1);
    run_test("cd with ../../", "../../", 1);
    run_test("cd with trailing slash /tmp/", "/tmp/", 1);
    run_test("cd to empty string", "", 0);

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
