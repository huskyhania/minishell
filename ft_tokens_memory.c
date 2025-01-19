/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokens_memory.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 17:11:35 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/19 10:02:52 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_validate_expansion(t_mini *attributes)
{
	t_tokens	*token;
	int			i;

	i = 1;
	token = attributes->tokens;
	while (token)
	{
		if (token->type == t_command && token->str[0] != '\0') // break removed;
			i = 1;
		if (token->type == t_lessless)
			i = 1;
		if ((token->type == t_great || token->type == t_less
				|| token->type == t_greatgreat ) && (token->next->str[0] == '\0' && token->next->failexp == 1))
		{
			//ft_putstr_fd("ambiguous redirect\n", 2);
			//attributes->exitcode = 1;
			i = 1;
		}
		token = token->next;
	}
	return (i);
}

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

t_tokens	*ft_find_last(t_tokens *stack) //tokens and parsing have same function ?? (remove one)
{
	if (!stack)
		return (NULL);
	while (stack->next)
		stack = stack->next;
	return (stack);
}

void	print_tokens(t_mini *attributes) //helper fucntion will be deleted from final product
{
	t_tokens	*temp;

	temp = attributes->tokens;
	while (temp != NULL)
	{
		printf("Node type: %u Merge %d String %s \n", temp->type, temp->merge, temp->str);
		temp = temp->next;
	}
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
	new_token->str = NULL; // ned addition might fuck up things?
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
