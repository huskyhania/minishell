/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_and_restore_std.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 18:36:17 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/22 18:54:59 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	save_stdin(t_mini *attributes)
{
	int	saved_stdin;

	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin == -1)
	{
		syscall_fail(1, attributes, "dup");
		return (-1);
	}
	else
		return (saved_stdin);
}

void	restore_stdin(int saved_stdin, t_mini *attributes)
{
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
		syscall_fail(1, attributes, "dup2");
	close(saved_stdin);
}
