/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirections.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 13:41:36 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/04 14:04:16 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_infile(t_cmd_table *node, t_mini *attributes)
{
	int	input;
	int	i;

	i = 0;
	while (node->infile[i] != NULL)
	{
		input = open(node->infile[i], O_RDONLY);
		if (input < 0)
		{
			perror(node->infile[i]);
			attributes->exitcode = 1;
			return (1);
		}
		i++;
	}
	dup2(input, STDIN_FILENO);
	close(input);
	return (0);
}

int	check_outfile(t_cmd_table *node, t_mini *attributes)
{
	int	output;
	int	i;

	i = 0;
	while (node->outfile[i] != NULL)
	{
		output = open(node->outfile[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (output < 0)
		{
			perror(node->outfile[i]);
			attributes->exitcode = 1;
			return (1);
		}
		i++;
	}
	dup2(output, STDOUT_FILENO);
	close(output);
	return (0);
}

//int	check_heredoc(t_cmd_table *node, t_mini *attributes)

int	check_redirs(t_cmd_table *node, t_mini *attributes)
{
	//int	here_doc_fd;
	if (node->infile && node->infile[0])
	{
		if (check_infile(node, attributes))
			exit(1);
	}
	/*if (node->here && node->here[0])
	{
		here_doc_fd = here_doc_handler(node, attributes);
		if (here_doc_fd < 0)
		{
			perror("here doc function fail");
			exit (1);
		}
		printf("%d here doc fd\n", here_doc_fd);
		if (dup2(here_doc_fd, STDIN_FILENO) < 0)
			perror("dup2 error");
		close(here_doc_fd);
	}*/
	if (node->outfile && node->outfile[0])
	{
		if (check_outfile(node, attributes))
			exit(1);
	}
	return (0);
}
