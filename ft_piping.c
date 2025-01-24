/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 13:52:07 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/15 15:24:48 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_pipe(t_mini *attribs)
{
	if (attribs->i < attribs->cmd_index)
	{
		if (dup2(attribs->pipe_arr[attribs->i - 1][WRITE], STDOUT_FILENO) == -1)
			syscall_fail(1, attribs, "dup2");
		close(attribs->pipe_arr[attribs->i - 1][WRITE]);
	}
	if (attribs->i > 1)
		close(attribs->pipe_arr[attribs->i - 2][READ]);
}

void	close_pipes(t_mini *attributes)
{
	int	i;

	i = 0;
	while (i < attributes->cmd_index - 1)
	{
		if (attributes->pipe_arr[i])
		{
			if (attributes->pipe_arr[i][READ] > 0)
				close(attributes->pipe_arr[i][READ]);
			if (attributes->pipe_arr[i][WRITE] > 1)
				close(attributes->pipe_arr[i][WRITE]);
		}
		i++;
	}
}

int	create_pipes(t_mini *attributes)
{
	int	i;

	i = -1;
	attributes->num_pipes = attributes->cmd_index - 1;
	attributes->pipe_arr = malloc(attributes->num_pipes * sizeof(int *));
	if (!attributes->pipe_arr)
		return (-1);
	while (++i < attributes->num_pipes)
	{
		attributes->pipe_arr[i] = malloc(2 * sizeof(int));
		if (!attributes->pipe_arr[i])
		{
			free_pipes(attributes);
			return (syscall_fail(1, attributes, "malloc"), -1);
		}
		if (pipe(attributes->pipe_arr[i]) == -1)
		{
			free_pipes(attributes);
			return (syscall_fail(1, attributes, "pipe"), -1);
		}
	}
	return (0);
}

void	free_pipes(t_mini *attributes)
{
	int	i;

	i = 0;
	if (!attributes->pipe_arr)
		return ;
	while (i < attributes->num_pipes)
	{
		if (attributes->pipe_arr[i])
		{
			if (attributes->pipe_arr[i][0] > 0)
				close(attributes->pipe_arr[i][0]);
			if (attributes->pipe_arr[i][1] > 0)
				close(attributes->pipe_arr[i][1]);
			free(attributes->pipe_arr[i]);
			attributes->pipe_arr[i] = NULL;
		}
		i++;
	}
	free(attributes->pipe_arr);
	attributes->pipe_arr = NULL;
	attributes->i = 0;
}

void	handle_pipes(t_mini *attribs)
{
	if (attribs->i == 1)
	{
		if (dup2(attribs->pipe_arr[attribs->i - 1][WRITE], STDOUT_FILENO) == -1)
			syscall_fail(1, attribs, "dup2");
	}
	else if (attribs->i > 1 && attribs->i < attribs->cmd_index)
	{
		if (dup2(attribs->pipe_arr[attribs->i - 2][READ], STDIN_FILENO) == -1)
			syscall_fail(1, attribs, "dup2");
		if (dup2(attribs->pipe_arr[attribs->i - 1][WRITE], STDOUT_FILENO) == -1)
			syscall_fail(1, attribs, "dup2");
	}
	else if (attribs->i == attribs->cmd_index)
	{
		if (dup2(attribs->pipe_arr[attribs->i - 2][READ], STDIN_FILENO) == -1)
			syscall_fail(1, attribs, "dup2");
	}
	close_pipes(attribs);
}
