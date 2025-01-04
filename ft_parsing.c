/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 18:12:26 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/04 18:39:36 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_merge_pipe(t_mini *attributes, t_cmd_table *old_table)
{
	t_cmd_table *new_pipe;
	
	new_pipe = malloc(sizeof(t_cmd_table));
	new_pipe->str = ft_strdup("|");
	new_pipe->infile = NULL;
	new_pipe->outfile = NULL;
	new_pipe->type = t_pipe;
	new_pipe->right = old_table;
	new_pipe->left = attributes->commands;
	attributes->commands = new_pipe;
}

t_cmd_table *ft_merge_redirection(t_tokens **token, t_cmd_table *old_table)
{
	if ((*token)->type == t_great)
		old_table->outfile = ft_add_command_array(old_table->outfile, (*token)->next->str);
	if ((*token)->type == t_less)
		old_table->infile = ft_add_command_array(old_table->infile, (*token)->next->str);
	if ((*token)->type == t_lessless)
		old_table->here = ft_add_command_array(old_table->here, (*token)->next->str);
	if ((*token)->type == t_greatgreat)
		old_table->append = ft_add_command_array(old_table->append, (*token)->next->str);
	*token = (*token)->next;
	if ((*token)->prev->type == t_great || (*token)->prev->type == t_greatgreat)
		old_table->last_outfile = (*token)->prev->type;
	if ((*token)->prev->type == t_less || (*token)->prev->type == t_lessless)
		old_table->last_infile = (*token)->prev->type;
	old_table->type = (*token)->prev->type;
	return (old_table);
}

t_cmd_table	*ft_merge_command(t_mini *attributes, t_tokens **token)
{
	t_cmd_table *new_node;

	new_node = ft_add_new(attributes, *token);
	while(*token != NULL && (*token)->type != t_pipe)
	{
		printf("Merging\n");
		if (*token && ((*token)->type == t_great || (*token)->type == t_less || (*token)->type == t_lessless || (*token)->type == t_greatgreat))
			new_node = ft_merge_redirection(token, new_node);
		else
			new_node->cmd_arr = ft_add_command_array(new_node->cmd_arr, (*token)->str);
		*token = (*token)->next;
	}
	//new_node->type = 20;
	printf("Merging done\n");
	attributes->cmd_index += 1;
	return (new_node);
}
void ft_start_parsing(t_mini *attributes)
{
	t_tokens *token;
	token = attributes->tokens;
	attributes->cmd_index = 0;

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

void print_array(t_cmd_table *print)
{
	int i = 0;
	while (print && print->cmd_arr[i])
	{
		printf("Index %d string|%s|\n", i, print->cmd_arr[i]);
		printf("Type: %d ", print->type);
		i++;
	}
}

void print_file(t_cmd_table *print)
{
	int i = 0;
	printf("Last infile %d Last outfile %d\n", print->last_infile, print->last_outfile);
	while (print->cmd_arr && print->cmd_arr[0])
	{
		if (!print->cmd_arr[i])
			break ;
		printf("String type: %d string|%s|\n",print->type,print->cmd_arr[i]);
		i++;
	}
	i=0;
	while (print && print->outfile && print->outfile[0])
	{
		if (!print->outfile[i])
			break ;
		printf("Type: %d Index %d Outfile:|%s|\n", print->type ,i, print->outfile[i]);
		i++;
	}
	i=0;
	while (print && print->infile && print->infile[0])
	{
		if (!print->infile[i])
			break ;
		printf("Type: %d Index %d Infile:|%s|\n", print->type ,i, print->infile[i]);
		i++;
	}
	i=0;
	while (print && print->here && print->here[0])
	{
		if (!print->here[i])
			break ;
		printf("Type: %d Index %d Here:|%s|\n", print->type ,i, print->here[i]);
		i++;
	}
	i=0;
	while (print && print->append && print->append[0])
	{
		if (!print->append[i])
			break ;
		printf("Type: %d Index %d Append:|%s|\n", print->type ,i, print->append[i]);
		i++;
	}
}

void	ft_parsing(t_mini *attributes)
{
	attributes->commands = NULL;
	ft_start_parsing(attributes);

	printf("COMMAND TABLE:\n");
	t_cmd_table *print = attributes->commands;
	printf("CMD COUNT %d\n",attributes->cmd_index);
	while(print)
	{
		if (print->type == t_command)
			print_array(print);
		//printf("first node: %s type: %d\n", print->str, print->type);
		if (print->type == t_great || print->type == t_less || print->type == t_greatgreat || print->type == t_lessless)
		{
			print_file(print);
			//printf("string|%s|\n", print->cmd_arr[0]);
			//printf("Type: %d Infile:|%s| Outfile:|%s|\n Deli:|%s|\n", print->type, print->infile, print->outfile[0], print->here);
		}
		if (print->type == t_pipe && print->right)
		{
			print_file(print->right);
			//printf("right node : %s type: %d\n", print->right->str, print->right->type);
		}
		if (print->type == t_pipe)
		{
			printf("PIPE\n");
			print = print->left;
		}
		else
			break ;
	}
	printf("COMMAND TABLE FINITO:\n");
	ft_free_tokens(attributes);
}
