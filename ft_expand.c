/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:11:35 by llaakson          #+#    #+#             */
/*   Updated: 2024/12/18 15:12:20 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_expand(t_mini *attributes)
{
		char *expansion;
		
		t_tokens *token;
		token = attributes->tokens;
		while (token)
		{
			if (token->type == t_dollar)
			{
				expansion = get_env_value(attributes, token->str);
				if (!expansion)
				{
					printf("no value in env\n");
					break ;
				}	
				token->str = ft_strdup(expansion);
				printf("FOUND PATH: %s. \n", expansion);
			}
			token = token->next;
		}
}
