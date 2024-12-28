/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokens_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 13:22:07 by llaakson          #+#    #+#             */
/*   Updated: 2024/12/29 00:01:49 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_skip_whitespace(char *line)
{
	int i;

	i = 0;
	while (line[i] == ' ' || line[i] == '\t' || line[i] == '\n' || line[i] == '\v' || line[i] == '\f' || line[i] == '\r')
		i++;
	return (i);
}

int	ft_is_special(char *line)
{
	if (*line == '|' || *line == '>' || *line == '<' || *line == '(' || *line == ')')
		return (1);
	return (0);
}

int	ft_is_whitespace(char *line)
{
	if (*line == ' ' || *line == '\t' || *line == '\n' || *line == '\v' || *line == '\f' || *line == '\r')
		return (1);
	return (0);
}

int ft_is_quote(char *line)
{
	if (*line == 39) // 34 = "
		return (1);
	return (0);
}
