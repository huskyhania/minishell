/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 17:07:16 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/20 10:49:05 by llaakson         ###   ########.fr       */
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

void ft_check_ast_array(t_cmd_table *print)
{
	if (!print)
		return ;
	if (print->cmd_arr)
		ft_triple_pointer(&print->cmd_arr);
	if (print && print->outfile)
		ft_free_cmd_array(print->outfile);
	if (print && print->infile)
		ft_free_cmd_array(print->infile);
	if (print && print->here)
		ft_free_cmd_array(print->here);
	if (print && print->append)
		ft_free_cmd_array(print->append);
	if (print && print->herefile)
		ft_triple_pointer(&print->herefile);
	free(print->type_arr);
}

int ft_free_ast(t_mini *attributes)
{
	t_cmd_table *table;
	t_cmd_table *temp;

	table = attributes->commands;
	if (!attributes->commands)
		return (0);
	while (table)
	{
		//printf("free loop\n");
		if (table->right)
		{
			ft_check_ast_array(table->right);
			free(table->right);
		}
		temp = table->left;
		if (table->type != t_pipe)
		{
			ft_check_ast_array(table);
			free(table);
		}
		else
			free(table);
		table = temp;
	}
	attributes->commands = NULL;
	return (1);
}
