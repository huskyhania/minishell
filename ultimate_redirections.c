/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ultimate_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 19:54:23 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/11 21:11:33 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// outfile 2, infile 3, append 4, heredoc 5

static int	ultimate_check_infile(t_cmd_table *node, t_mini *attributes, int index)
{
	(void)attributes;
	int	input;
	if (node->input_fd > 0)
		close(node->input_fd);
	input = open(node->herefile[index], O_RDONLY);
	if (input < 0)
	{
		perror(node->herefile[index]);
		return (1);
	}
	if (node->cmd_arr)
	{
		node->input_fd = input;
		node->in1 = node->herefile[index];//test
	}
	return (0);
}
static int	ultimate_check_outfile(t_cmd_table *node, t_mini *attributes, int index)
{
	(void)attributes;
	int	output;
	if (node->output_fd > 1)
		close(node->output_fd);
	output = open(node->herefile[index], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (output < 0)
	{
		perror(node->herefile[index]);
		return (1);
	}
	if (node->cmd_arr)
	{
		node->output_fd = output;
		node->out1 = node->herefile[index];
	}
	return (0);
}
static int	ultimate_check_append(t_cmd_table *node, t_mini *attributes, int index)
{
	(void)attributes;
	int	ap_output;
	if (node->output_fd > 1)
		close(node->output_fd);
	ap_output = open(node->herefile[index], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (ap_output < 0)
	{
		perror(node->herefile[index]);
		return (1);
	}
	if (node->cmd_arr)
	{
		node->output_fd = ap_output;
		node->out1 = node->herefile[index];
	}
	return (0);
}
int	check_files(t_cmd_table *node, t_mini *attributes)
{
	int	i;
	i = 0;
	node->in1 = NULL;
	node->out1 = NULL;
	while (node->herefile[i] != NULL)
	{
		if (node->type_arr[i] == 3)
		{
			if (ultimate_check_infile(node, attributes, i))
				return (1);
		}
		if (node->type_arr[i] == 2)
		{
			if (ultimate_check_outfile(node, attributes, i))
				return (1);
		}
		if (node->type_arr[i] == 4)
		{
			if (ultimate_check_append(node, attributes, i))
			{
				//printf("append check fail");
				return (1);
			}
		}
		/*if (node->type_arr[i] == 5)
		{
			if (ultimate_check_heredoc(node, i))
				return (1);
		}*/
		i++;
	}
	return (0);
}
