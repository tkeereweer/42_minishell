//this is a vibe coded tree visualiser
//it's not part of the project to be evaluated

#include "minishell.h"
#include <string.h>

// Get string representation of a node
char	*get_node_label(t_node *node)
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
		{
			snprintf(buf, sizeof(buf), "\"%s\"", node->content.str);
			return buf;
		}
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

// Calculate tree height
int	get_height(t_node *node)
{
	int left_h, right_h;
	
	if (!node)
		return 0;
	
	left_h = get_height(node->left_child);
	right_h = get_height(node->right_child);
	
	return 1 + (left_h > right_h ? left_h : right_h);
}

// Print spaces
void	print_spaces(int n)
{
	int i;
	
	i = 0;
	while (i < n)
	{
		printf(" ");
		i++;
	}
}

// Print a level of the tree
void	print_level(t_node *node, int level, int space)
{
	int i;
	
	if (!node)
	{
		print_spaces(space);
		return;
	}
	
	if (level == 1)
	{
		char *label = get_node_label(node);
		int len = strlen(label);
		int padding = (space - len) / 2;
		
		print_spaces(padding);
		printf("%s", label);
		print_spaces(space - len - padding);
	}
	else if (level > 1)
	{
		print_level(node->left_child, level - 1, space / 2);
		print_level(node->right_child, level - 1, space / 2);
	}
}

// Print connectors for a level
void	print_connectors(t_node *node, int level, int space)
{
	if (!node || level < 1)
	{
		print_spaces(space);
		return;
	}
	
	if (level == 1)
	{
		int mid = space / 2;
		int i;
		
		// Left connector
		if (node->left_child)
		{
			print_spaces(mid / 2);
			printf("/");
			print_spaces(mid / 2 - 1);
		}
		else
			print_spaces(mid);
		
		// Right connector
		if (node->right_child)
		{
			print_spaces(mid / 2);
			printf("\\");
			print_spaces(mid / 2 - 1);
		}
		else
			print_spaces(mid);
	}
	else
	{
		print_connectors(node->left_child, level - 1, space / 2);
		print_connectors(node->right_child, level - 1, space / 2);
	}
}

// Main drawing function
void	draw_tree(t_node *root)
{
	int height, level, space;
	int i;
	
	if (!root)
	{
		printf("Empty tree\n");
		return;
	}
	
	height = get_height(root);
	space = 80;
	
	// Calculate initial spacing based on height
	i = 1;
	while (i < height)
	{
		space *= 2;
		i++;
	}
	
	printf("\nLogical AST:\n\n");
	
	level = 1;
	while (level <= height)
	{
		print_level(root, level, space);
		printf("\n");
		
		if (level < height)
		{
			print_connectors(root, level, space);
			printf("\n");
		}
		
		level++;
	}
	
	printf("\n");
}

// Example usage - create a sample tree
t_node	*create_node(t_type type)
{
	t_node *node = malloc(sizeof(t_node));
	
	node->type = type;
	node->left_child = NULL;
	node->right_child = NULL;
	node->parent = NULL;
	
	return node;
}

int	main(void)
{
	t_node *root, *left, *right;
	
	// Example: (echo hello | grep h) && ls || pwd
	// Tree structure:
	//           ||
	//          /  \
	//        &&    "pwd"
	//       /  \
	//   "echo hello | grep h"  "ls"
	
	root = create_node(LOGIC);
	root->content.logic = OR;
	
	left = create_node(LOGIC);
	left->content.logic = AND;
	left->parent = root;
	root->left_child = left;
	
	right = create_node(PIPELINE);
	right->content.str = strdup("pwd");
	right->parent = root;
	root->right_child = right;
	
	t_node *ll = create_node(PIPELINE);
	ll->content.str = strdup("echo hello | grep h");
	ll->parent = left;
	left->left_child = ll;
	
	t_node *lr = create_node(PIPELINE);
	lr->content.str = strdup("ls");
	lr->parent = left;
	left->right_child = lr;
	
	// Draw the tree
	draw_tree(root);
	
	// Cleanup (simplified - real code should free all nodes)
	free(ll->content.str);
	free(lr->content.str);
	free(right->content.str);
	free(ll);
	free(lr);
	free(left);
	free(right);
	free(root);
	
	return 0;
}