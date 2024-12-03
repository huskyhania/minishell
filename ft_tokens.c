/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokens.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:06:47 by llaakson          #+#    #+#             */
/*   Updated: 2024/12/03 20:47:52 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tokens	*ft_find_last(t_tokens *stack)
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
	while (attributes->tokens->next != NULL)
	{
		printf("node type: %u string %s\n", attributes->tokens->type, attributes->tokens->str);
		attributes->tokens = attributes->tokens->next;
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

void	ft_add_pipe(t_mini *attributes)
{
	t_tokens *new_command;

	new_command = ft_add_token(attributes);
	new_command->type = t_pipe;
	new_command->str = "|";	
}

int		ft_add_special(char *line, t_mini *attributes)
{
		if (*line == '|')
		{
			ft_add_pipe(attributes);
			return (1);
		}
		return (0);
	
}

char	*ft_add_command(char *line, t_mini *attributes)
{
		t_tokens *new_command;
		char *temp_line;
		int i = 0;
		
		temp_line = line;
		new_command = ft_add_token(attributes);
		new_command->type = t_command;
		while (temp_line[i] != ' ')
		{
			if (temp_line[i] == '\0')
				break ;
			i++;
		}
		new_command->str = ft_substr(line, 0, i); // malloc here remeber to check and free after parsing
		return (line+i);
}
void	ft_tokenization(t_mini *attributes)
{
	char		*line;

	attributes->tokens = NULL;
	line = attributes->readret;
	while (*line)
	{
		if (*line == ' ') // add all whitespace later here as function and put it in as while instead of if
			line++;
		if	(ft_add_special(line, attributes))
			line++;
		if (*line != ' ')
			line = ft_add_command(line, attributes);
		if (*line)
			line++;
	}
	print_tokens(attributes);
}
