/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 17:07:16 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/06 18:30:28 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_check_ast_array(t_cmd_table *print)
{
	if (print->cmd_arr)
		ft_free_cmd_array(print->cmd_arr);
	if (print && print->outfile)
		ft_free_cmd_array(print->outfile);
	if (print && print->infile)
		ft_free_cmd_array(print->infile);
	if (print && print->here)
		ft_free_cmd_array(print->here);
	if (print && print->append)
		ft_free_cmd_array(print->append);
}

void	ft_free_ast(t_mini *attributes)
{
	t_cmd_table *table;
	t_cmd_table *temp;

	table = attributes->commands;
	if (!attributes->commands)
		printf("IDIOT\n");
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
}
