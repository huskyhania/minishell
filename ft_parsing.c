/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 18:12:26 by llaakson          #+#    #+#             */
/*   Updated: 2024/12/19 22:39:24 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

t_cmd_table *ft_merge_redirection(t_tokens **token, t_cmd_table *old_table)
{
	//t_cmd_table *new_node;
	
	//new_node = malloc(sizeof(t_cmd_table));
	if ((*token)->type == 2)
		old_table->outfile = ft_strdup((*token)->next->str);
	if ((*token)->type == 3)
		old_table->infile = ft_strdup((*token)->next->str);
	*token = (*token)->next;
}

t_cmd_table	*ft_merge_command(t_mini *attributes, t_tokens **token)
{
	t_cmd_table *new_node;

	new_node = ft_add_new(attributes, *token);
	while(*token != NULL && (*token)->type != t_pipe)
	{
		//printf("Merging\n");
		if (*token && ((*token)->type == t_great || (*token)->type == t_less))
			new_node = ft_merge_redirection(token, new_node);
		else
		{
			if (!new_node->str)
				new_node->str = ft_strdup((*token)->str);
			else
				new_node->str = ft_strjoin(new_node->str, (*token)->str);
		}
		*token = (*token)->next;
	}
	//new_node->type = 20;
	//printf("Merging done\n");
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
		//printf("hello\n");
		token = token->next;
		new_table = ft_merge_command(attributes, &token);
		ft_merge_pipe(attributes, new_table);
	}
}

void print_helper(t_cmd_table *print)
{
	t_cmd_table *print_temp = print;
	print_temp = print_temp->left;
	while(print_temp)
	{
		printf("Redirection node: %s type: %d\n", print_temp->str, print_temp->type);
		print_temp = print_temp->left;
	}
}

void	ft_parsing(t_mini *attributes)
{
	ft_expand(attributes);
	attributes->commands = NULL;
	ft_start_parsing(attributes);


	printf("COMMAND TABLE:\n");
	t_cmd_table *print = attributes->commands;
	while(print)
	{
		printf("first node: %s type: %d\n", print->str, print->type);
		if (print->type == 20)
			printf("Infile: %s Outfile: %s\n", print->infile, print->outfile);
		if (print->type == t_pipe)
		{
			printf("right node : %s type: %d\n", print->right->str, print->right->type);
			if (print->right->type == 20)
				printf("Infile: %s Outfile: %s\n", print->right->infile, print->right->outfile);
		}
		if (print->type == t_pipe)
			print = print->left;
		else
			break ;
	}
	printf("COMMAND TABLE FINITO:\n");
}
