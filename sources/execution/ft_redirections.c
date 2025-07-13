/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirections.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 13:41:36 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/18 20:07:33 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redir_in(t_cmd_table *node, t_mini *attributes)
{
	if (node->last_infile == 3)
		node->input_fd = open(node->in1, O_RDONLY);
	else if (node->last_infile == 5)
		node->input_fd = open("here_doc", O_RDONLY);
	if (node->input_fd < 0)
	{
		syscall_fail(1, attributes, "open");
		return (1);
	}
	if (dup2(node->input_fd, STDIN_FILENO) == -1)
	{
		syscall_fail(1, attributes, "input dup2");
		return (1);
	}
	close(node->input_fd);
	return (0);
}

int	redir_out(t_cmd_table *node, t_mini *attributes)
{
	if (node->last_outfile == 2)
		node->output_fd = open(node->out1, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (node->last_outfile == 4)
		node->output_fd = open(node->out1, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (node->output_fd < 0)
	{
		syscall_fail(1, attributes, "open");
		return (1);
	}
	if (dup2(node->output_fd, STDOUT_FILENO) == -1)
	{
		syscall_fail(1, attributes, "output dup2");
		return (1);
	}
	close(node->output_fd);
	return (0);
}
