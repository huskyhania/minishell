/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokens_memory.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 17:11:35 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/20 09:15:25 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_tokens(t_mini *attributes)
{
	t_tokens	*token;
	t_tokens	*temp;

	token = attributes->tokens;
	while (token)
	{
		temp = token->next;
		if (token->str)
		{
			free(token->str);
			token->str = NULL;
		}
		free(token);
		token = temp;
	}
}

t_tokens	*ft_find_last(t_tokens *stack)
{
	if (!stack)
		return (NULL);
	while (stack->next)
		stack = stack->next;
	return (stack);
}

t_tokens	*ft_add_token(t_mini *attributes)
{
	t_tokens	*new_token;
	t_tokens	*last_token;

	new_token = malloc(sizeof(t_tokens));
	if (!new_token)
		return (NULL);
	new_token->next = NULL;
	new_token->merge = 0;
	new_token->failexp = -1;
	new_token->dollar = 0;
	new_token->str = NULL;
	if (!attributes->tokens)
	{
		attributes->tokens = new_token;
		new_token->prev = NULL;
	}
	else
	{
		last_token = ft_find_last(attributes->tokens);
		last_token->next = new_token;
		new_token->prev = last_token;
	}
	return (new_token);
}
