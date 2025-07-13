/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 17:07:16 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/24 12:35:40 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_cmd_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array && array[i])
	{
		if (array[i])
		{
			free(array[i]);
			array[i] = NULL;
		}
		i++;
	}
	free(array);
	array = NULL;
}

void	ft_triple_pointer(char ***array)
{
	int	i;

	if (!array || !*array)
		return ;
	i = 0;
	while ((*array)[i])
	{
		free((*array)[i]);
		(*array)[i] = NULL;
		i++;
	}
	free(*array);
	*array = NULL;
}

void	ft_check_ast_array(t_cmd_table *print)
{
	if (!print)
		return ;
	if (print->cmd_arr)
		ft_triple_pointer(&print->cmd_arr);
	if (print && print->herefile)
		ft_triple_pointer(&print->herefile);
	if (print->type_arr)
	{
		free(print->type_arr);
		print->type_arr = NULL;
	}
}

void	ft_free_left(t_cmd_table *table)
{
	if (table->type != t_pipe)
	{
		ft_check_ast_array(table);
		free(table);
		table = NULL;
	}
	else
	{
		free(table);
		table = NULL;
	}
}

int	ft_free_ast(t_mini *attributes)
{
	t_cmd_table	*table;
	t_cmd_table	*temp;

	table = attributes->commands;
	if (!attributes->commands)
		return (0);
	while (table)
	{
		if (table->right)
		{
			ft_check_ast_array(table->right);
			free(table->right);
			table->right = NULL;
		}
		temp = table->left;
		ft_free_left(table);
		table = temp;
	}
	attributes->commands = NULL;
	return (1);
}
