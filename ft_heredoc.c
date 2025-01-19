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

void ft_heredoc_error(char *delimit)
{
	printf("here-document at line delimited by end-of-file (wanted `%s')\n", delimit);
}

int	here_doc_handler(t_cmd_table *node, t_mini *attributes, char *delimit)
{
	int	temp_fd;
	int	saved_stdin;
	char	*line;

	(void)node;
	saved_stdin = dup(STDIN_FILENO);//checks for dup
	//fprintf(stderr, "given delimiter is %s\n", delimit);
	temp_fd = open("here_doc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (temp_fd < 0)
	{
		ft_putendl_fd("cannot create temp file", 2);
		attributes->exitcode = 1; //to udpate in struct and delete
		return (-1);
	}
	ft_heresignal();
	while (1)
	{
		line = readline("heredoc> ");//or here?
		//if (!line)
		//	break ;
		if (!line || (delimit[0] == '\0' && line[0] == '\0') || (ft_strncmp(delimit, line, ft_strlen(delimit)) == 0 && ft_strlen(delimit) == ft_strlen(line)))
			break ;
		write(temp_fd, line, ft_strlen(line));
		write(temp_fd, "\n", 1);
		free(line);
		line = NULL;
	}
	close(temp_fd);
	ft_resetsignal();
	if (g_signal == SIGINT) // Control + C should trigger this and exit back to main with exitcode 130
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
		unlink("here_doc");
		attributes->exitcode = 130;
		ft_resetposthere();
		//printf("exit heredoc with ctrl+c\n");
		return (-1);
	}
	if (!line) // Control + D goes here and life continues
	{
		//printf("exit heredoc with ctrl+d\n");
		ft_heredoc_error(delimit);
		ft_resetposthere();
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
		return (0);
	}
	ft_sigint();
	if (line)
		free(line);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
	//close(temp_fd);
	return (0);
}

int	check_for_heredocs(t_cmd_table *node, t_mini *attributes)
{
	int	i;
	i = 0;
	while (node->herefile[i] != NULL)
	{
		if (node->type_arr[i] != 5 || ultimate_check_heredoc(node, attributes, i))
			i++;
		else
			return (1);
	}
	return (0);
}
