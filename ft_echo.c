/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 21:36:36 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/18 19:44:32 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_n_flags(char **cmd_arr, int *i)
{
	int	newline;
	int	j;

	newline = 1;
	*i = 1;
	while (cmd_arr[*i] && cmd_arr[*i][0] == '-')
	{
		j = 1;
		while (cmd_arr[*i][j] == 'n')
			j++;
		if (cmd_arr[*i][j] != '\0')
			break ;
		newline = 0;
		(*i)++;
	}
	return (newline);
}

static void	print_arguments(char **cmd_arr, int i, int newline)
{
	while (cmd_arr[i] != NULL)
	{
		printf("%s", cmd_arr[i]);
		if (cmd_arr[i + 1] != NULL)
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
}

void	ft_echo(t_cmd_table *node, t_mini *attributes)
{
	int	i;
	int	newline;
	int	saved_std;

	if (attributes->cmd_index == 1)
		saved_std = dup(STDOUT_FILENO);
	if (node->out1)
	{
		if (redir_out(node, attributes))
			return (syscall_fail(1, attributes, "redir out"));
	}
	newline = process_n_flags(node->cmd_arr, &i);
	print_arguments(node->cmd_arr, i, newline);
	if (attributes->cmd_index == 1)
	{
		dup2(saved_std, STDOUT_FILENO);
		close(saved_std);
		attributes->exitcode = 0;
	}
}
