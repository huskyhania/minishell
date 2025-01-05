/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirections.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 13:41:36 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/05 16:21:53 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_infile(t_cmd_table *node, t_mini *attributes)
{
	int	input;
	int	i;

	i = 0;
	input = -1;
	while (node->infile[i] != NULL)
	{
		if (input >= 0)
			close(input);
		input = open(node->infile[i], O_RDONLY);
		if (input < 0)
		{
			perror(node->infile[i]);
			attributes->exitcode = 1;
			return (1);
		}
		i++;
	}
	if (node->last_infile == 3 && input >= 0)
		dup2(input, STDIN_FILENO);
	if (input >= 0)
		close(input);
	return (0);
}

int	check_outfile(t_cmd_table *node, t_mini *attributes)
{
	int	output;
	int	i;

	i = 0;
	output = -1;
	while (node->outfile[i] != NULL)
	{
		if (output >= 0)
			close(output);
		output = open(node->outfile[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (output < 0)
		{
			perror(node->outfile[i]);
			attributes->exitcode = 1;
			return (1);
		}
		i++;
	}
	if (node->last_outfile == 2 && output >= 0)
		dup2(output, STDOUT_FILENO);
	if (output >= 0)
		close(output);
	return (0);
}

int	check_append(t_cmd_table *node, t_mini *attributes)
{
	int	output;
	int	i;

	i = 0;
	output = -1;
	while(node->append[i] != NULL)
	{
		if (output >= 0)
			close(output);
		output = open(node->append[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (output < 0)
		{
			perror(node->append[i]);
			attributes->exitcode = 1;
			return (1);
		}
		i++;
	}
	if (node->last_outfile == 4 && output >= 0)
		dup2(output, STDOUT_FILENO);
	if (output >= 0)
		close(output);
	return (0);
}

int	check_heredoc(t_cmd_table *node, t_mini *attributes)
{
	attributes->here_fd = open("here_doc", O_RDONLY);
	if (attributes->here_fd < 0)
	{
		perror("failed to open heredoc file");
		return (1);
	}
	if (node->last_infile == 5)
	{
		if (dup2(attributes->here_fd, STDIN_FILENO) < 0)
		{
			perror("dup2 error");
			return (1);
		}
	}
	close(attributes->here_fd);
	return (0);
}

int	check_redirs(t_cmd_table *node, t_mini *attributes)
{
	if (node->infile && node->infile[0])
	{
		perror("got into infile check");
		if (check_infile(node, attributes))
			exit (1);
	}
	if (node->here && node->here[0])
	{
		perror("got into heredoc check");
		if (check_heredoc(node, attributes))
		{
			perror("heredoc check fail");
			exit (1);
		}
	}
	if (node->outfile && node->outfile[0])
	{
		perror("got into outfile check");
		if (check_outfile(node, attributes))
			exit (1);
	}
	if (node->append && node->append[0])
	{
		perror("got into append check");
		if (check_append(node, attributes))
			exit (1);
	}
	return (0);
}
