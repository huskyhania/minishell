/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 19:56:10 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/14 21:27:37 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// [optional cmd] << DELIMITER
// cmd << DELIMITER | cmd1 >> file
//open temporary file with flags O_CREAT | O_WRONLY | O_TRUNCT, 0644
//int here_doc_fd should it be in attributes?

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
		if (!line)
			break ;
		if ((delimit[0] == '\0' && line[0] == '\0') || (ft_strncmp(delimit, line, ft_strlen(delimit)) == 0 && ft_strlen(delimit) == ft_strlen(line)))
			break ;
		write(temp_fd, line, ft_strlen(line));
		write(temp_fd, "\n", 1);
		free(line);
		line = NULL;
	}
	close(temp_fd);
	if (g_signal == SIGINT)
	{
		attributes->exitcode = 130;
		// reset g_signal to 0 here or later
		ft_sigint();
		printf("exit heredoc with ctrl+c\n");
		// stop execution in here after ctrl+c in heredoc
	}
	ft_sigint();
	if (!line)
	{
		//close(temp_fd);
		//unlink("here_doc");
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
		return (-1); // or exit
	}
	if (line)
		free(line);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
	close(temp_fd);
	return (0);
}

int	process_heredocs(t_cmd_table *node, t_mini *attributes)
{
	int	i;

	i = 0;
	while (node->here[i] != NULL)
	{
		if (attributes->here_fd > 0)
			close(attributes->here_fd);
		if (here_doc_handler(node, attributes, node->here[i]) < 0)
		{
			perror(node->here[i]);
			attributes->exitcode = 1;
			return (1);
		}
		attributes->here_fd = open("here_doc", O_RDONLY);
		if (attributes->here_fd < 0)
		{
			perror(node->here[i]);
			attributes->exitcode = 1;
			return (1);
		}
		i++;
	}
	attributes->exitcode = 0;
	return (0);
}

int	check_for_heredocs(t_cmd_table *node, t_mini *attributes)
{
	int	i;
	i = 0;
	while (node->herefile[i] != NULL)
	{
		if (node->type_arr[i] != 5)
			i++;
		else if (ultimate_check_heredoc(node, attributes, i))
			return (1);
		i++;
	}
	return (0);
}