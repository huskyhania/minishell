/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 13:00:26 by llaakson          #+#    #+#             */
/*   Updated: 2024/12/04 16:10:31 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd_table *ft_new_table_node()
{
	t_cmd_table *new_node;

	new_node = malloc(sizeof(t_cmd_table)); // malloc
	return (new_node);
}

void ft_add_new(t_mini *attributes, int i,t_tokens *tokens)
{
	t_cmd_table *new_node;

	new_node = ft_new_table_node();
	attributes->commands[i].str = tokens->str;
	attributes->commands[i].type = tokens->type;	
}

void ft_parsing(t_mini *attributes)
{	
	t_tokens *temp;
	temp = attributes->tokens;
	int i = 0;
 
	while (temp != NULL)
	{
		//printf("NODE type: %u STRING %s\n", temp->type, temp->str);
		ft_add_new(attributes,i,temp);
		i++;
		temp = temp->next;
	}
	attributes->commands[i].str = NULL;
	i = 0;
	while (attributes->commands[i].str != NULL)
	{
		printf("STR: %s TYPE: %u\n", attributes->commands[i].str, attributes->commands[i].type);
		i++;
	}
}
