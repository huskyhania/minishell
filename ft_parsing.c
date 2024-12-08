/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 13:00:26 by llaakson          #+#    #+#             */
/*   Updated: 2024/12/08 17:54:20 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd_table	*ft_find_last_parse(t_cmd_table *stack)
{
	if (!stack)
		return (NULL);
	while (stack->next)
		stack = stack->next;
	return (stack);
}

void ft_add_new(t_mini *attributes, t_tokens *temp)
{
	t_cmd_table *new_node;
	t_cmd_table *last_node;

	new_node = malloc(sizeof(t_cmd_table)); // malloc here the list needs to be freed after parsing.
	if (!new_node)
	{
		printf("Error\n");
		exit(1);
	}
	new_node->next = NULL;
	if (!attributes->commands)
	{
		//printf("Made first token\n");
		attributes->commands = new_node;
		new_node->prev = NULL;
	}
	else
	{
		//printf("Made additional token\n");
		last_node = ft_find_last_parse(attributes->commands);
		last_node->next = new_node;
		new_node->prev = last_node;
	}
	new_node->str = ft_strdup(temp->str);
	new_node->type = temp->type;
	//return (new_node);
}

void ft_command_merge(t_mini *attributes, t_tokens *temp)
{
	t_cmd_table *temp_table;

	temp_table = ft_find_last_parse(attributes->commands);
	//printf("STR: %s TYPE: %s\n", attributes->commands->str, temp->str);
	strcat(temp_table->str, temp->str); // system function
	//attributes->commands->str = temp_str;
}

void ft_start_parsing(t_mini *attributes, t_tokens *temp)
{
	while (temp != NULL)
	{
		if (attributes->commands && temp->type == t_command && temp->prev->type == t_command)
			ft_command_merge(attributes, temp);
		else
			ft_add_new(attributes, temp);
		temp = temp->next;
	}
}

void ft_parsing(t_mini *attributes)
{	
	t_tokens *temp;
	temp = attributes->tokens;
	
	attributes->commands = NULL;
	ft_start_parsing(attributes, temp);
	printf("HERE\n");
	t_cmd_table *print = attributes->commands;
	while (print != NULL)
	{
		printf("STR: %s TYPE: %u\n", print->str, print->type);
		print = print->next;
	}
}
