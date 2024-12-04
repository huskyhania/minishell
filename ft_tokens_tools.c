/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokens_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 13:22:07 by llaakson          #+#    #+#             */
/*   Updated: 2024/12/04 13:46:05 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_skip_whitespace(char **line)
{
	while (**line == ' ' || **line == '\t' || **line == '\n' || **line == '\v' || **line == '\f' || **line == '\r')
		line++;	
}
