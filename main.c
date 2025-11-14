#include "minishell.h"

int	main(int argc, char *argv[])
{
	t_list	*list;
	t_node	*tree;

	if (argc != 2)
		return (1);
	list = clean_node_list(argv[1]);
	tree = create_logic_tree(list);
	draw_tree(tree);
	free_tree(tree);
	return (0);
}