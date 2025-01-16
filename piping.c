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

int	create_pipes(t_mini *attributes)
{
	int	i;

	i = 0;
	attributes->num_pipes = attributes->cmd_index - 1;
	attributes->pipe_arr = malloc(attributes->num_pipes * sizeof(int *));
	if (!attributes->pipe_arr)
	{
		printf("malloc fail");
		return (-1);
	}
	while (i < attributes->num_pipes)
	{
		attributes->pipe_arr[i] = malloc(2 * sizeof(int));
		if (!attributes->pipe_arr[i])
		{
			perror("malloc fail");
			while (--i >= 0)
			{
				close(attributes->pipe_arr[i][0]);
				close(attributes->pipe_arr[i][1]);
				free(attributes->pipe_arr[i]);
			}
			free(attributes->pipe_arr);
			return (-1);
		}
		if (pipe(attributes->pipe_arr[i]) == -1)
		{
			perror("piping error\n");
			free(attributes->pipe_arr[i]); // Free the current pip
			while (--i >= 0)
			{
				close(attributes->pipe_arr[i][0]);
				close(attributes->pipe_arr[i][1]);
				free(attributes->pipe_arr[i]);
			}
			free(attributes->pipe_arr);
			return (-1);
		}
		i++;
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
		}
		i++;
	}
	free(attributes->pipe_arr);
	attributes->pipe_arr = NULL;
	attributes->i = 0;
}

void	handle_pipes(t_mini *attributes)
{
	int i;

	i = 0;
	if (attributes->i == 1)
	{
		dup2(attributes->pipe_arr[attributes->i - 1][WRITE], STDOUT_FILENO);
		//close(attributes->pipe_arr[attributes->i - 1][WRITE]);
	}
	else if (attributes->i > 1 && attributes->i < attributes->cmd_index)
	{
		dup2(attributes->pipe_arr[attributes->i - 2][READ], STDIN_FILENO);
		//close(attributes->pipe_arr[attributes->i - 2][READ]);
		dup2(attributes->pipe_arr[attributes->i - 1][WRITE], STDOUT_FILENO);
		//close(attributes->pipe_arr[attributes->i - 1][WRITE]);
	}
	else if (attributes->i == attributes->cmd_index)
	{
		dup2(attributes->pipe_arr[attributes->i - 2][READ], STDIN_FILENO);
		//close(attributes->pipe_arr[attributes->i - 2][READ]);
	}
	while (i < attributes->cmd_index - 1)
	{
		close(attributes->pipe_arr[i][READ]);
		close(attributes->pipe_arr[i][WRITE]);
		i++;
	}
}
