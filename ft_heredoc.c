/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 19:56:10 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/03 20:13:44 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// [optional cmd] << DELIMITER
// cmd << DELIMITER | cmd1 >> file
//open temporary file with flags O_CREAT | O_WRONLY | O_TRUNCT, 0644
//int here_doc_fd should it be in attributes?

int	here_doc_handler(t_cmd_table *node, t_mini *attributes)
{
	int	temp_fd;
	char	*line;

	temp_fd = open("here_doc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (temp_fd < 0)
	{
		ft_putendl_fd("cannot create temp file", 2);
		attributes->exitcode = 1; //to udpate in struct and delete
		exit(1);
	}
	while (1)
	{
		write(1, "heredoc> ", 9);//will fd ever change?
		line = get_next_line(0);//or here?
		if (!line)
			break ;
		if (ft_strncmp(node->here[0], line, ft_strlen(node->here[0])) == 0 && line[ft_strlen(node->here[0])] == '\n')
			break ;
		write(temp_fd, line, ft_strlen(line));
		free(line);
	}
	if (!line)
	{
		printf("error from gnl");
		close(temp_fd);
		return (1); // or exit
	}
	if (line)
		free(line);
	return (temp_fd);
}
