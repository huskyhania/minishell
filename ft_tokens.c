/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokens.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:06:47 by llaakson          #+#    #+#             */
/*   Updated: 2024/12/04 21:43:42 by llaakson         ###   ########.fr       */
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
	t_tokens *temp;
	temp = attributes->tokens;
 
	while (temp != NULL)
	{
		printf("node type: %u string %s\n", temp->type, temp->str);
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
void	ft_add_token_type(t_tokens *new_command, char **line, int size)
{
	new_command->str = ft_substr(*line, 0, size);
}

void	ft_add_pipe(t_mini *attributes, char **line)
{
	t_tokens *new_command;
	int size;
	
	size = 1;
	new_command = ft_add_token(attributes); // Can't handle << or >>
	if (**line == '|')
		new_command->type = t_pipe;
	if (**line == '>')
		new_command->type = t_great;
	if (**line == '<')
		new_command->type = t_less;
	if (**line == '(')
		new_command->type = t_bracketleft;
	if (**line == ')')
		new_command->type = t_bracketrigth;
	if (!(ft_strncmp(*line, ">>", 2)))
		new_command->type = t_greatgreat;
	if (!(ft_strncmp(*line, "<<", 2)))
		new_command->type = t_lessless;
	if (new_command->type == t_lessless || new_command->type == t_greatgreat)
		size++;
	new_command->str = ft_substr(*line, 0, size);
	(*line) += size;
}

int		ft_add_special(char **line, t_mini *attributes)
{
		if (**line == '|' || **line == '>' || **line == '<' || **line == '(' || **line == ')')
		{
			ft_add_pipe(attributes, line);
			return (1);
		}
		return (0);
	
}
//Bug?? This functions can add < > () | to the command arg. if middle or at the end of the arg.
char	*ft_add_command(char *line, t_mini *attributes)
{
		t_tokens *new_command;
		char *temp_line;
		int i = 0;
		
		temp_line = line;
		new_command = ft_add_token(attributes);
		new_command->type = t_command;
		while (!(ft_is_whitespace(&temp_line[i]))) //Might need to check different whitspaces && what if space in middle of string
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
		ft_skip_whitespace(&line); // function that skips whitespace
		//ft_add_pipe(attributes, &line);
		if	(ft_is_special(line))
			ft_add_pipe(attributes, &line);
		if (*line != ' ' && *line)
			line = ft_add_command(line, attributes);
		if (*line)
			line++;
	}
	print_tokens(attributes);
}
