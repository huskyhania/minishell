/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command_checks.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 17:47:32 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/07 19:14:01 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_if_valid_command(t_cmd_table *node, t_mini *attributes)
{
	if (is_empty_or_space(node->cmd_arr[0]))
	{
		write(2, node->cmd_arr[0], ft_strlen(node->cmd_arr[0]));
		write(2, ": command not found\n", 21);
		attributes->exitcode = 127;
		return (1);
	}
	if (access(node->cmd_arr[0], X_OK) == 0)
		return (0);
	write(2, node->cmd_arr[0], ft_strlen(node->cmd_arr[0]));
	if (access(node->cmd_arr[0], F_OK) == 0)
	{
		attributes->exitcode = 126;
		write(2, ": Permission denied\n", 22);
	}
	else 
	{
		attributes->exitcode = 127;
		write(2, ":command not found\n", 17);
	}
	return (1);
}
