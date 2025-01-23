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

int	save_std(t_mini *attributes, int fd)
{
	int	saved_std;

	saved_std = dup(fd);
	if (saved_std == -1)
	{
		syscall_fail(1, attributes, "dup");
		return (-1);
	}
	else
		return (saved_std);
}

void	restore_std(int saved_fd, int fd, t_mini *attributes)
{
	if (dup2(saved_fd, fd) == -1)
		syscall_fail(1, attributes, "dup2");
	close(saved_fd);
}
