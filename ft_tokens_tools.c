/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokens_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 13:22:07 by llaakson          #+#    #+#             */
/*   Updated: 2024/12/16 16:40:05 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_skip_whitespace(char **line)
{
	while (**line == ' ' || **line == '\t' || **line == '\n' || **line == '\v' || **line == '\f' || **line == '\r')
		line++;	
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
	if (*line == 34 || *line == 39)
		return (1);
	return (0);
}
