/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 19:56:10 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/19 13:27:52 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// [optional cmd] << DELIMITER
// cmd << DELIMITER | cmd1 >> file
//open temporary file with flags O_CREAT | O_WRONLY | O_TRUNCT, 0644
//int here_doc_fd should it be in attributes?

void	ft_heredoc_error(char *delimit)
{
	printf("here-document at line delimited");
	printf(" by end-of-file (wanted `%s')\n", delimit);
}

static int	here_doc_open(t_mini *attributes, int *temp_fd)
{
	*temp_fd = open("here_doc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (*temp_fd < 0)
	{
		ft_putendl_fd("cannot create heredoc file", 2);
		update_exitcode(1, attributes);
		return (-1);
	}
	return (0);
}

static int	signal_interrupt_here(t_mini *attribs, int saved_fd, char *del)
{
	if (g_signal == SIGINT)
	{
		if (dup2(saved_fd, STDIN_FILENO) == -1)
			syscall_fail(1, attribs, "dup2");
		close(saved_fd);
		unlink("here_doc");
		attribs->exitcode = 130;
		ft_resetposthere();
		return (-1);
	}
	else
	{
		ft_heredoc_error(del);
		ft_resetposthere();
		dup2(saved_fd, STDIN_FILENO);
		close(saved_fd);
		return (0);
	}
}

int	here_doc_handler(t_mini *attributes, char *delimit)
{
	int		temp_fd;
	int		saved_stdin;
	char	*line;

	saved_stdin = save_stdin(attributes);
	if (here_doc_open(attributes, &temp_fd) < 0)
		return (-1);
	ft_heresignal();
	while (1)
	{
		line = readline("heredoc> ");
		if (!line || (delimit[0] == '\0' && line[0] == '\0')
			|| (ft_strncmp(delimit, line, ft_strlen(delimit)) == 0
				&& ft_strlen(delimit) == ft_strlen(line)))
			break ;
		ft_putendl_fd(line, temp_fd);
		check_and_free_string(line);
	}
	close(temp_fd);
	if (g_signal == SIGINT || !line)
		return (signal_interrupt_here(attributes, saved_stdin, delimit));
	ft_sigint();
	check_and_free_string(line);
	restore_stdin(saved_stdin, attributes);
	return (0);
}

int	check_for_heredocs(t_cmd_table *node, t_mini *attribs)
{
	int	i;

	i = 0;
	while (node->herefile[i] != NULL)
	{
		if (node->type_arr[i] != 5 || !ultimate_check_heredoc(node, attribs, i))
			i++;
		else
			return (1);
	}
	return (0);
}
