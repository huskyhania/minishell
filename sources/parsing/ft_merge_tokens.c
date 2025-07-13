/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_merge_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 09:41:29 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/22 15:08:47 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_convert(t_mini *attributes)
{
	t_tokens	*token;

	token = attributes->tokens;
	while (token)
	{
		if (token->type == t_quote)
			token->type = t_command;
		token = token->next;
	}
}

void	ft_delete_token(t_tokens *token)
{
	t_tokens	*temp;

	temp = token->next;
	token->next = token->next->next;
	free(temp);
}

int	ft_merge_tokens(t_mini *attributes)
{
	t_tokens	*token;
	char		*new_str;

	token = attributes->tokens;
	while (token && token->next)
	{
		if (token->next && token->merge == 1
			&& (token->next->type == t_command || token->next->type == t_quote))
		{
			new_str = ft_strjoin(token->str, token->next->str);
			if (!new_str)
				return (syscall_fail(2, attributes, "malloc"), 0);
			free(token->str);
			free(token->next->str);
			token->str = NULL;
			token->str = new_str;
			if (token->failexp == 0 || token->next->failexp == 0)
				token->failexp = 0;
			token->merge = token->next->merge;
			ft_delete_token(token);
		}
		else
			token = token->next;
	}
	return (1);
}
