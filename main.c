#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

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
				draw_tree(tree);
				free_tree(tree);
			}
			free(line);
		}
	}
	clear_history();
	return (0);
}
