/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 13:00:26 by llaakson          #+#    #+#             */
/*   Updated: 2024/12/03 14:22:49 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_parsing(t_mini *attributes)
{
	//printf("Parsin & String: %s\n", attributes->tokens->str);
	if (attributes->tokens->type == t_command)
	{
		printf("Parsin & String: %s\n", attributes->tokens->str);
	}
}
