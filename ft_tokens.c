/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokens.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:06:47 by llaakson          #+#    #+#             */
/*   Updated: 2024/12/03 13:59:44 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_add_token(char *line, t_mini *attributes)
{
		t_tokens *new_token;
		
		new_token = malloc(sizeof(t_tokens));
		attributes->tokens = new_token;
		if (!new_token)
		{
			printf("Error\n");
			exit(1);
		}
		new_token->type = t_command;
		new_token->str = ft_strdup(line);
}

void	ft_tokenization(t_mini *attributes)
{
	char		*line;
	//printf("Tokens & String: %s\n", attributes->readret);
	line = attributes->readret;
	ft_add_token(line, attributes);
}
