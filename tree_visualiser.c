#include "minishell.h"
#include <string.h>

char    *get_node_label(t_node *node)
{
    static char buf[256];

    if (!node)
        return "(null)";

    if (node->type == LOGIC)
    {
        if (node->content.logic == AND)
            return "&&";
        else
            return "||";
    }
    else if (node->type == PAR)
    {
        snprintf(buf, sizeof(buf), "(%c)", node->content.parenthesis);
        return buf;
    }
    else if (node->type == PIPELINE)
    {
        if (node->content.str)
            return node->content.str;
        return "PIPELINE";
    }
    else if (node->type == CMD)
        return "CMD";
    else if (node->type == ARGS)
        return "ARGS";
    else if (node->type == REDIR)
        return "REDIR";

    return "?";
}

void	print_tree_recursive(t_node *node, char *prefix, int is_left)
{
	char *label;
	char *new_prefix;
	int len;
	
	if (node == NULL)
		return;
	
	label = get_node_label(node);
	
	// Print current node
	printf("%s", prefix);
	printf("%s", is_left ? "├── " : "└── ");
	printf("%s\n", label);
	
	// Prepare prefix for children
	len = strlen(prefix) + 5;
	new_prefix = malloc(len);
	if (!new_prefix)
		return;
	
	strcpy(new_prefix, prefix);
	strcat(new_prefix, is_left ? "│   " : "    ");
	
	// Print children
	if (node->left_child || node->right_child)
	{
		if (node->left_child)
			print_tree_recursive(node->left_child, new_prefix, 
				node->right_child != NULL);
		
		if (node->right_child)
			print_tree_recursive(node->right_child, new_prefix, 0);
	}
	
	free(new_prefix);
}

// Main drawing function
void	draw_tree(t_node *root)
{
	if (!root)
	{
		printf("Empty tree\n");
		return;
	}
	
	printf("\nLogical AST:\n");
	printf("%s\n", get_node_label(root));
	
	if (root->left_child || root->right_child)
	{
		if (root->left_child)
			print_tree_recursive(root->left_child, "", 
				root->right_child != NULL);
		
		if (root->right_child)
			print_tree_recursive(root->right_child, "", 0);
	}
	
	printf("\n");
}