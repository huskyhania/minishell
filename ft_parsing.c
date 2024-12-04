/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 13:00:26 by llaakson          #+#    #+#             */
/*   Updated: 2024/12/04 14:24:22 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd_table *ft_new_table_node()
{
	t_cmd_table *new_node;

	new_node = malloc(sizeof(t_cmd_table));
	return (new_node);
}

void ft_parsing(t_mini *attributes)
{

	//new_table = ft_new_table_node();	
	//printf("Parsin & String: %s\n", attributes->tokens->str);
	if (attributes->tokens->type == t_command)
	{
		//printf("Parsin & String: %s\n", attributes->tokens->str);
		attributes->commands[0].str = attributes->tokens->str;
		printf("Parsin & String: %s\n", attributes->commands[0].str);
	}
}
