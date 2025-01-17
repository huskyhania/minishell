/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 21:36:36 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/17 21:44:18 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo(t_cmd_table *node, t_mini *attributes)
{
	int	i;
	int	newline;
	int	saved_std;

	i = 1;
	newline = 1;
	if (attributes->cmd_index == 1)
		saved_std = dup(STDOUT_FILENO);
	if (attributes->cmd_index > 1)
	{
		if (attributes->i < attributes->cmd_index)
		{
			dup2(attributes->pipe_arr[attributes->i - 1][WRITE], STDOUT_FILENO);
			close(attributes->pipe_arr[attributes->i - 1][WRITE]);
		}
		if (attributes->i > 1)
			close(attributes->pipe_arr[attributes->i - 2][READ]);
	}
	if (node->out1)
	{
		if (redir_out(node, attributes))
		{
			attributes->exitcode = 1;
			return ;
		}
	}
	if (node->cmd_arr[i] && ft_strcmp(node->cmd_arr[i], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	while (node->cmd_arr[i] != NULL)
	{
		printf("%s", node->cmd_arr[i]);
		if (node->cmd_arr[i + 1] != NULL)
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	if (attributes->cmd_index == 1)
	{
		dup2(saved_std, STDOUT_FILENO);
		close(saved_std);
		attributes->exitcode = 0;
	}
}
