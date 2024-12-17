/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 18:12:26 by llaakson          #+#    #+#             */
/*   Updated: 2024/12/17 14:19:27 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd_table	*ft_find_last_parse(t_cmd_table *stack)
{
	if (!stack)
		return (NULL);
	while (stack->right)
		stack = stack->right;
	return (stack);
}

t_cmd_table *ft_add_new(t_mini *attributes, t_tokens *token)
{
	t_cmd_table *new_node;

	(void)attributes;
	new_node = malloc(sizeof(t_cmd_table));
	if (!new_node)
	{
		printf("Error\n");
		exit(1);
	}
	if (token->type == t_command)
		new_node->str = ft_strdup(token->str);
	new_node->type = (token->type);
	return(new_node);
}

void ft_merge_pipe(t_mini *attributes, t_cmd_table *old_table)
{
	t_cmd_table *new_pipe;
	
	new_pipe = malloc(sizeof(t_cmd_table));
	new_pipe->str = ft_strdup("|");
	new_pipe->type = t_pipe;
	new_pipe->right = old_table;
	new_pipe->left = attributes->commands;
	attributes->commands = new_pipe;
}

t_cmd_table	*ft_merge_command(t_mini *attributes, t_tokens **token)
{
	t_cmd_table *new_node;

	new_node = ft_add_new(attributes, *token);
	while(*token != NULL && (*token)->type != t_pipe)
	{
		printf("Merging\n");
		if ((*token)->type == t_great || (*token)->type == t_less)
		{
			new_node->direction = (*token)->type;
		}
		*token = (*token)->next;
		if (*token && (*token)->type == t_command)
		{
			if (!new_node->str)
				new_node->str = ft_strdup((*token)->str);
			else
				new_node->str = ft_strjoin(new_node->str, (*token)->str);
		}
	}
	printf("Merging done\n");
	return (new_node);
}
void ft_start_parsing(t_mini *attributes)
{
	t_tokens *token;
	token = attributes->tokens;

	//make the first table node
	t_cmd_table *new_table;
	//merge everything before the first pipe
	attributes->commands = ft_merge_command(attributes, &token);
	//continue if there are pipes
	while(token != NULL && token->type == t_pipe)
	{
		printf("hello\n");
		token = token->next;
		new_table = ft_merge_command(attributes, &token);
		ft_merge_pipe(attributes, new_table);
	}
}

void	ft_parsing(t_mini *attributes)
{
	attributes->commands = NULL;
	ft_start_parsing(attributes);
	printf("COMMAND TABLE:\n");
	t_cmd_table *print = attributes->commands;
	while(print)
	{
		printf("first node: %s redirection: %d\n", print->str, print->direction);
		if (print->type == t_command)
			break ;
		if (print->right->str)
			printf("right node : %s redireciton: %d\n", print->right->str, print->right->direction);
		//if (print->left == NULL || print->left->type != t_pipe)
		//	break ;
		if (print->left)
			print = print->left;
	}
	printf("COMMAND TABLE:\n");
}
