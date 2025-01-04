/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 17:07:16 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/04 18:24:09 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_check_ast_array(t_cmd_table *print)
{
	if (print->cmd_arr && print->cmd_arr[0])
		ft_free_cmd_array(print->cmd_arr);
	if (print && print->outfile && print->outfile[0])
		ft_free_cmd_array(print->outfile);
	if (print && print->infile && print->infile[0])
		ft_free_cmd_array(print->infile);
	if (print && print->here && print->here[0])
		ft_free_cmd_array(print->here);
	if (print && print->append && print->append[0])
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
		printf("free loop\n");
		if (table->right)
		{
			ft_check_ast_array(table->right);
			free(table->right);
		}
		temp = table->left;
		if (table)
		{
			ft_check_ast_array(table);
			free(table);
		}
		table = temp;
	}
}
