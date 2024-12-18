/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing_tools.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:06:34 by llaakson          #+#    #+#             */
/*   Updated: 2024/12/18 20:33:47 by llaakson         ###   ########.fr       */
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
	//if (token->type == t_command)
	//new_node->str = ft_strdup(token->str);
	new_node->type = (token->type);
	return(new_node);
}
