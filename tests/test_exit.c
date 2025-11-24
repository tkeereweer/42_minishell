/* ************************************************************************** */
/*                                                                            */
/*   test_exit.c                                                             */
/*   Standalone test for ft_exit builtin                                     */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <string.h>
#include <sys/wait.h>

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

void test_exit_code(unsigned int code, const char *test_name)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        // Child process
        ft_exit(code);
        // Should never reach here
        exit(255);
    }
    else if (pid > 0)
    {
        // Parent process
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            int exit_code = WEXITSTATUS(status);
            unsigned char expected = (unsigned char)code;
            if (exit_code == expected)
            {
                printf("  -> Exit code: %d (expected: %d)\n", exit_code, expected);
                print_test_result(test_name, 1);
            }
            else
            {
                printf("  -> Exit code: %d (expected: %d)\n", exit_code, expected);
                print_test_result(test_name, 0);
            }
        }
        else
        {
            print_test_result(test_name, 0);
        }
    }
    else
    {
        printf("Fork failed\n");
        print_test_result(test_name, 0);
    }
}

int main(void)
{
    print_test_header("Testing FT_EXIT builtin");

    printf("\n--- Test 1: Exit with code 0 ---\n");
    test_exit_code(0, "exit(0) returns 0");

    printf("\n--- Test 2: Exit with code 1 ---\n");
    test_exit_code(1, "exit(1) returns 1");

    printf("\n--- Test 3: Exit with code 42 ---\n");
    test_exit_code(42, "exit(42) returns 42");

    printf("\n--- Test 4: Exit with code 255 ---\n");
    test_exit_code(255, "exit(255) returns 255");

    printf("\n--- Test 5: Exit with code > 255 (wraps) ---\n");
    test_exit_code(256, "exit(256) wraps to 0");

    printf("\n--- Test 6: Exit with code 300 ---\n");
    test_exit_code(300, "exit(300) wraps to 44");

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
