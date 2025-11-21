/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 09:21:20 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/20 09:30:46 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*str_realloc(char *ptr, size_t size)
{
	char	*out;

	out = (char *) malloc(size);
	if (out == NULL)
	{
		if (ptr)
			free(ptr);
		return (NULL);
	}
	if (ptr)
	{
		ft_strlcpy(out, ptr, size);
		free(ptr);
	}
	return (out);
}

int ft_cd(char *path)
{
	int 	ret;
	char	*home;
	char	*new_str;

	ret = -1;
	if (path == NULL)
	{
		home = getenv("HOME");
		// maybe still execute if HOME is empty
		if (home != NULL)
			ret = chdir(home);
	}
	// maybe not necessary to handle ~ expansion
	else if (path[0] == '~')
	{
		home = getenv("HOME");
		if (home != NULL)
		{
			new_str = (char *) malloc((ft_strlen(home) + ft_strlen(&path[1]) + 1) * sizeof(char));
			if (new_str == NULL)
				return (-1);
			ft_strlcpy(new_str, home, ft_strlen(home) + ft_strlen(&path[1]) + 1);
			ft_strlcat(new_str, &path[1], ft_strlen(home) + ft_strlen(&path[1]) + 1);
			ret = chdir(new_str);
		}
		else
			ret = chdir(&path[1]);
	}
	else
		ret = chdir(path);
	return (ret);
}

#ifndef TESTING
// int main(int argc, char *argv[])
// {
// 	char    buf[1000];
// 	int     ret;

// 	if (argc > 2)
// 		return (1);
// 	if (argc == 1)
// 		ret = cd(NULL);
// 	else
// 		ret = cd(argv[1]);
// 	if (ret == -1)
// 	{
// 		perror("cd");
// 		return (1);
// 	}
// 	getcwd(buf, 1000);
// 	printf("%s\n", buf);
// 	return (0);
// }

void print_test_result(const char *test_name, int passed)
{
    printf("[%s] %s\n", passed ? "PASS" : "FAIL", test_name);
}

void run_test(const char *test_name, char *path, int should_succeed)
{
    char    original_dir[1000];
    char    new_dir[1000];
    int     ret;
    
    getcwd(original_dir, 1000);
    ret = ft_cd(path);
    
    if (should_succeed)
    {
        if (ret == 0)
        {
            getcwd(new_dir, 1000);
            printf("[PASS] %s -> Changed to: %s\n", test_name, new_dir);
        }
        else
        {
            printf("[FAIL] %s -> Expected success but got error\n", test_name);
        }
    }
    else
    {
        if (ret == -1)
        {
            printf("[PASS] %s -> Failed as expected\n", test_name);
        }
        else
        {
            printf("[FAIL] %s -> Expected failure but succeeded\n", test_name);
        }
    }
    
    // Try to restore original directory
    chdir(original_dir);
}

int main(int argc, char *argv[])
{
    char    buf[1000];
    char    *home;
    
    printf("=== CD Function Comprehensive Test Suite ===\n\n");
    
    // Get initial directory
    getcwd(buf, 1000);
    printf("Starting directory: %s\n", buf);
    home = getenv("HOME");
    printf("HOME environment: %s\n\n", home ? home : "(not set)");
    
    // Test 1: cd with NULL (should go to HOME)
    printf("--- Test 1: cd(NULL) - Go to HOME ---\n");
    run_test("cd(NULL)", NULL, home != NULL);
    printf("\n");
    
    // Test 2: cd to HOME explicitly
    printf("--- Test 2: cd to HOME explicitly ---\n");
    if (home)
        run_test("cd(HOME)", home, 1);
    else
        printf("[SKIP] HOME not set\n");
    printf("\n");
    
    // Test 3: cd to /tmp
    printf("--- Test 3: cd to /tmp ---\n");
    run_test("cd(\"/tmp\")", "/tmp", 1);
    printf("\n");
    
    // Test 4: cd to /
    printf("--- Test 4: cd to / (root) ---\n");
    run_test("cd(\"/\")", "/", 1);
    printf("\n");
    
    // Test 5: cd to current directory (.)
    printf("--- Test 5: cd to current directory (.) ---\n");
    run_test("cd(\".\")", ".", 1);
    printf("\n");
    
    // Test 6: cd to parent directory (..)
    printf("--- Test 6: cd to parent directory (..) ---\n");
    run_test("cd(\"..\")", "..", 1);
    printf("\n");
    
    // Test 7: cd to non-existent directory
    printf("--- Test 7: cd to non-existent directory ---\n");
    run_test("cd(\"/nonexistent_dir_12345\")", "/nonexistent_dir_12345", 0);
    printf("\n");
    
    // Test 8: cd with ~ (tilde expansion to HOME)
    printf("--- Test 8: cd with ~ (tilde expansion) ---\n");
    if (home)
        run_test("cd(\"~\")", "~", 1);
    else
        printf("[SKIP] HOME not set\n");
    printf("\n");
    
    // Test 9: cd with ~/path
    printf("--- Test 9: cd with ~/path ---\n");
    if (home)
        run_test("cd(\"~/.\")", "~/.", 1);
    else
        printf("[SKIP] HOME not set\n");
    printf("\n");
    
    // Test 10: cd with ~/ and subdirectory
    printf("--- Test 10: cd with ~/Documents (may not exist) ---\n");
    if (home)
        run_test("cd(\"~/Documents\")", "~/Documents", 1); // May fail if doesn't exist
    else
        printf("[SKIP] HOME not set\n");
    printf("\n");
    
    // Test 11: cd with relative path
    printf("--- Test 11: cd with relative path (../tmp) ---\n");
    run_test("cd(\"../tmp\")", "../tmp", 0); // May succeed or fail depending on location
    printf("\n");
    
    // Test 12: cd to /usr
    printf("--- Test 12: cd to /usr ---\n");
    run_test("cd(\"/usr\")", "/usr", 1);
    printf("\n");
    
    // Test 13: cd with multiple ../..
    printf("--- Test 13: cd with ../../ ---\n");
    run_test("cd(\"../../\")", "../../", 1);
    printf("\n");
    
    // Test 14: cd with trailing slash
    printf("--- Test 14: cd with trailing slash /tmp/ ---\n");
    run_test("cd(\"/tmp/\")", "/tmp/", 1);
    printf("\n");
    
    // Test 15: cd to empty string
    printf("--- Test 15: cd to empty string ---\n");
    run_test("cd(\"\")", "", 0);
    printf("\n");
    
    // Test 16: cd with ~ but HOME not set (simulate)
    printf("--- Test 16: cd with complex tilde path ~/../../tmp ---\n");
    if (home)
        run_test("cd(\"~/../../tmp\")", "~/../../tmp", 1);
    else
        printf("[SKIP] HOME not set\n");
    printf("\n");
    
    // Test 17: cd to /etc
    printf("--- Test 17: cd to /etc ---\n");
    run_test("cd(\"/etc\")", "/etc", 1);
    printf("\n");
    
    // Test 18: Multiple .. navigations
    printf("--- Test 18: cd to ../../../../tmp ---\n");
    run_test("cd(\"../../../../../../../tmp\")", "../../../../../../../tmp", 1);
    printf("\n");
    
    printf("=== Test Suite Complete ===\n");
    
    // Restore to original directory
    getcwd(buf, 1000);
    printf("Final directory: %s\n", buf);

    return (0);
}
#endif
