#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

void	print_tree(t_node *node)
{
	int	i;

	i = 0;
	if (node == NULL)
		return ;
	print_tree(node->left_child);
	if (node->type == PIPELINE)
		ft_printf("%s\n", "PIPELINE");
	else if (node->type == CMD)
		ft_printf("%s\n", "CMD");
	else if (node->type == LOGIC)
	{
		if (node->content.logic == AND)
			ft_printf("%s\n", "AND");
		else if (node->content.logic == OR)
			ft_printf("%s\n", "OR");
	}
	else if (node->type == ARGS)
	{
		ft_printf("%s: ", "ARGS");
		while (node->content.tab[i] != NULL)
		{
			ft_printf("%s; ", node->content.tab[i]);
			i++;
		}
		ft_printf("%c", '\n');
	}
	else if (node->type == REDIR)
	{
		ft_printf("%s: ", "REDIR");
		ft_printf("%s; ", "**kind**");
		ft_printf("%s;\n", node->content.redir.path);
	}
	print_tree(node->right_child);
}

int	copy_env(t_data *data, char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
		i++;
	data->env = (char **) malloc((i + 2) * sizeof(char *));
	if (data->env == NULL)
		return (1);
	i = 0;
	while (envp[i] != NULL)
	{
		data->env[i] = ft_strdup(envp[i]);
		if (data->env[i] == NULL)
		{
			free_split(data->env);
			return (1);
		}
		i++;
	}
	data->env[i] = NULL;
	return (0);
}

int	main(int argc, char *argv[], char **envp)
{
	t_list	*list;
	t_node	*tree;
	char	*line;
	t_data	data;
	char	*str1;
	char	*str2;
	char	*tab[2];

	(void) argc;
	(void) argv;
	str1 = ft_strdup("hey=hello");
	str2 = ft_strdup("test=bye");
	tab[0] = ft_strdup("hey");
	tab[1] = NULL;
	if (copy_env(&data, envp) == 1)
		return (1);
	ft_export(str1, &data);
	ft_export(str2, &data);
	ft_unset(tab, &data);
	ft_env(&data);
	while (1)
	{
		line = readline("enter prompt: ");
		if (line != NULL)
		{
			add_history(line);
			list = clean_node_list(line);
			if (list != NULL)
			{
				tree = create_logic_tree(list);
				if (create_cmd_trees(tree) == 1)
					return (1);
				// draw_tree(tree);
				print_tree(tree);
				free_tree(tree);
			}
			free(line);
		}
	}
	clear_history();
	return (0);
}
