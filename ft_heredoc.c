/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 19:56:10 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/12/18 16:23:53 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// [optional cmd] << DELIMITER
// cmd << DELIMITER | cmd1 >> file
//open temporary file with flags O_CREAT | O_WRONLY | O_TRUNCT, 0644
//int here_doc_fd should it be in attributes?

int	here_doc_handler(char **cmd_array)
{
	int	temp_fd;
	char	*line;

	temp_fd = open("here_doc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (temp_fd < 0)
	{
		ft_putendl_fd("cannot create temp file", 2);
		ft_putendl_fd("exitcode should be 1", 2); //to udpate in struct and delete
		return (1);//or exit
	}
	while (1)
	{
		write(1, "heredoc> ", 9);//will fd ever change?
		line = get_next_line(0);//or here?
		if (!line)
			break ;
		if (ft_strncmp(cmd_array[1], line, ft_strlen(cmd_array[1])) == 0 && line[ft_strlen(cmd_array[1])] == '\n')
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
	close(temp_fd);
	//this might need to use unlink to delete the file
	return (0);
}
