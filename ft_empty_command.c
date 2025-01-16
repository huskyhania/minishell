/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_empty_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 13:51:27 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/15 00:59:53 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*#include "minishell.h"

static int	infile_check(t_cmd_table *node)
{
	int	i;
	int	input;

	i = 0;
	input = -1;
	if (!node->infile)
		return (0);
	while (node->infile[i] != NULL)
	{
		if (input > 0)
			close(input);
		input = open(node->infile[i], O_RDONLY);
		if (input < 0)
		{
			perror(node->infile[i]);
			return (1);
		}
		i++;
	}
	if (input > 0)
		close(input);
	return (0);
}

static int	outfile_check(t_cmd_table *node)
{
	int	output;
	int	i;

	i = 0;
	output = -1;
	if (!node->outfile)
		return (0);
	while (node->outfile[i] != NULL)
	{
		if (output > 1)
			close(output);
		output = open(node->outfile[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (output < 0)
		{
			perror(node->outfile[i]);
			return (1);
		}
		i++;
	}
	if (output > 1)
		close(output);
	return (0);
}

static int	append_check(t_cmd_table *node)
{
	int	output;
	int	i;

	i = 0;
	output = -1;
	if (!node->append)
		return (0);
	while (node->append[i] != NULL)
	{
		if (output > 1)
			close(output);
		output = open(node->append[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (output < 0)
		{
			perror(node->append[i]);
			return (1);
		}
		i++;
	}
	if (output > 1)
		close(output);
	return (0);
}

int	redir_empty(t_cmd_table *node, t_mini *attributes)
{
	if (!infile_check(node) && !outfile_check(node) && !append_check(node))
	{
		attributes->exitcode = 0;
		return (0);
	}
	else
	{
		attributes->exitcode = 1;
		return (1);
	}
}
*/