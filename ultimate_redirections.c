/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ultimate_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 19:54:23 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/15 16:21:43 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// outfile 2, infile 3, append 4, heredoc 5

static int	ultimate_check_infile(t_cmd_table *node, t_mini *attribs, int index)
{
	int	input;

	if (node->input_fd > 0)
		close(node->input_fd);
	if (node->failexp == 1)
	{
		ft_putstr_fd("ambiguous redirect\n", 2);
		update_exitcode(1, attribs);
		return (1);
	}
	input = open(node->herefile[index], O_RDONLY);
	if (input < 0)
	{
		perror(node->herefile[index]);
		return (1);
	}
	close(input);
	if (node->cmd_arr)
		node->in1 = node->herefile[index];
	return (0);
}

static int	ultimate_check_outfile(t_cmd_table *node, t_mini *atrbs, int index)
{
	int	output;

	if (node->output_fd > 1)
		close(node->output_fd);
	output = open(node->herefile[index], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (node->failexp == 1)
	{
		ft_putstr_fd("ambiguous redirect\n", 2);
		update_exitcode(1, atrbs);
		return (1);
	}
	if (output < 0)
	{
		perror(node->herefile[index]);
		return (1);
	}
	close(output);
	if (node->cmd_arr)
		node->out1 = node->herefile[index];
	return (0);
}

static int	ultimate_check_append(t_cmd_table *node, t_mini *atrbs, int index)
{
	int	a_output;

	if (node->output_fd > 1)
		close(node->output_fd);
	a_output = open(node->herefile[index], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (node->failexp == 1)
	{
		ft_putstr_fd("ambiguous redirect\n", 2);
		update_exitcode(1, atrbs);
		return (1);
	}
	if (a_output < 0)
	{
		perror(node->herefile[index]);
		return (1);
	}
	close(a_output);
	if (node->cmd_arr)
		node->out1 = node->herefile[index];
	return (0);
}

int	ultimate_check_heredoc(t_cmd_table *node, t_mini *atrbs, int index)
{
	if (node->input_fd > 0)
		close (node->input_fd);
	if (here_doc_handler(atrbs, node->herefile[index]) < 0)
		return (1);
	if (node->cmd_arr)
	{
		node->in1 = "here_doc";
		return (0);
	}
	else
	{
		unlink("here_doc");
		return (0);
	}
}

int	check_files(t_cmd_table *node, t_mini *atrbs, int i)
{
	if (!node->herefile)
		return (0);
	while (node->herefile[++i] != NULL)
	{
		if (node->type_arr[i] == 3 && ultimate_check_infile(node, atrbs, i))
			return (1);
		if (node->type_arr[i] == 2 && ultimate_check_outfile(node, atrbs, i))
			return (1);
		if (node->type_arr[i] == 4 && ultimate_check_append(node, atrbs, i))
			return (1);
		if (node->type_arr[i] == 5 && (atrbs->cmd_index == 1 || !node->cmd_arr))
		{
			if (ultimate_check_heredoc(node, atrbs, i))
				return (1);
		}
	}
	return (0);
}
