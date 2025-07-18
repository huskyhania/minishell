/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command_checks.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 17:47:32 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/08 19:54:02 by hskrzypi         ###   ########.fr       */
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
		if (attributes->cmd_index > 1)
			close_pipes_in_parent(attributes);
		return (1);
	}
	return (0);
}
