/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:14:58 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/15 00:52:20 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_error_and_display(int err, t_mini *attribs, const char *cmd)
{
	attribs->exitcode = err;
	ft_putstr_fd((char *)cmd, 2);
	if (err == 126)
		ft_putstr_fd(": Permission denied\n", 2);
	if (err == 127)
		ft_putstr_fd(": command not found\n", 2);
	if (err == 1)
	{
		if (access(cmd, F_OK) == 0)
			ft_putstr_fd(": Permission denied\n", 2);
		else
			ft_putstr_fd(": No such file or directory\n", 2);
	}
}

void	cleanup_child(t_mini *attributes)
{
	if (attributes->cmd_index > 1)
		free_pipes(attributes);
	ft_free_ast(attributes);
	envp_cleanup(attributes);
	if (attributes->pids)
		free(attributes->pids);
}

void	cleanup_parent(t_mini *attributes)
{
	ft_free_ast(attributes);
	if (attributes->pids)
	{
		free(attributes->pids);
		attributes->pids = NULL;
	}
	if (access("here_doc", F_OK) == 0)
		unlink("here_doc");
}

void	syscall_fail(int err_code, t_mini *attributes, const char *message)
{
	update_exitcode(err_code, attributes);
	ft_putstr_fd((char *)message, 2);
	ft_putstr_fd(" call failed\n", 2);
}

void	update_exitcode(int err_code, t_mini *attributes)
{
	if (attributes->cmd_index < 2 || attributes->i == attributes->cmd_index)
		attributes->exitcode = err_code;
}
