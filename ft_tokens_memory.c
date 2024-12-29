/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokens_memory.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 17:11:35 by llaakson          #+#    #+#             */
/*   Updated: 2024/12/29 16:52:21 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_tokens(t_mini *attributes)
{
	t_tokens *token;
	t_tokens *temp;

	token = attributes->tokens;
	while (token)
	{
		temp = token->next;
		if (token->str)
			free(token->str);
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

void	print_tokens(t_mini *attributes)
{
	//helper fucntion will be deleted from final product
	t_tokens *temp;
	temp = attributes->tokens;

	while (temp != NULL)
	{
		printf("Node type: %u Merge %d String %s \n", temp->type, temp->merge, temp->str);
		temp = temp->next;
	}
}

t_tokens	*ft_add_token(t_mini *attributes)
{
		t_tokens *new_token;
		t_tokens *last_token;

		new_token = malloc(sizeof(t_tokens)); // malloc here the list needs to be freed after parsing.
		if (!new_token)
		{
			printf("Error\n");
			exit(1);
		}
		new_token->next = NULL;
		new_token->merge = 0;
		if (!attributes->tokens)
		{
			//printf("Made first token\n");
			attributes->tokens = new_token;
			new_token->prev = NULL;
		}
		else
		{
			//printf("Made additional token\n");
			last_token = ft_find_last(attributes->tokens);
			last_token->next = new_token;
			new_token->prev = last_token;
		}
		return (new_token);
}
