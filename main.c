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

int	main(void)
{
	t_list	*list;
	t_node	*tree;
	char	*line;

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
