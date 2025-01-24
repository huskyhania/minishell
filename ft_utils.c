/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 19:34:02 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/24 11:28:48 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_and_free_double(char *str, char *str2)
{
	if (str)
	{
		free(str);
		str = NULL;
	}
	if (str2)
	{
		free(str2);
		str2 = NULL;
	}
}

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

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] == s2[i] && (s1[i] != '\0' || s2[i] != '\0'))
		i++;
	return (s1[i] - s2[i]);
}

void	check_and_free_string(char *str)
{
	if (!str)
		return ;
	free(str);
	str = NULL;
}
