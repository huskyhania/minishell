/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 19:34:02 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/12/23 14:31:06 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i] != NULL)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

int	is_empty_or_space(const char *cmd)
{
	if (!cmd || cmd[0] == '\0')
		return (1);
	while (*cmd)
	{
		if (*cmd != ' ')
			return (0);
		cmd++;
	}
	return (1);
}

char	**check_if_valid_command(const char *cmd)
{
	char **cmd_arr; // to be in struct
	if (is_empty_or_space(cmd))
	{
		write(2, cmd, ft_strlen(cmd));
		write(2, ": command not found\n", 21);
		//exitcode 127, possibly error flag?
		return (NULL);
	}
	cmd_arr = ft_split(cmd, ' ');
	if (!cmd_arr)
	{
		perror("cmd split error");
		return (NULL); // make sure to close fds if necessary, free cmd-related memory
	}
	int i = 0;
	while (cmd_arr[i] != NULL)
	{
		printf("cmd_arr[%d]: %s\n", i, cmd_arr[i]);
		i++;
	}
	return (cmd_arr);
}
