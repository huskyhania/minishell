/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokens.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:06:47 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/06 18:24:01 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		while (!(ft_is_whitespace(&temp_line[i])) && !(ft_is_special(&temp_line[i])) && temp_line[i] != '"' && temp_line[i] != '\'')
		{
			if (temp_line[i] == '\0')
				break ;
			i++;
		}
		if (!ft_is_whitespace(&temp_line[i]))
			new_command->merge = 1;
		new_command->str = ft_substr(line, 0, i); // malloc here remeber to check and free after parsing
		return (line+i);
}

char	*ft_add_quote(char *line, t_mini *attributes)
{
	t_tokens *new_command;
	char *temp_line;
	int i;

	i = 1;
	temp_line = line;
	new_command = ft_add_token(attributes);
	new_command->type = t_quote;
	// feels wrong
	while (!ft_is_quote(&line[i]) && temp_line[i] != '\0')
		i++;
	i++;
	if (!ft_is_whitespace(&temp_line[i]))
		new_command->merge = 1;
	new_command->str = ft_substr(line, 1, i-2);
	//new_command->str = ft_strjoin(new_command->str, " "); // This is shit ??
	return (line+i);
}

char *ft_add_expansion(t_mini *attributes, char *line)
{
	int i;
	char *temp_line;
	t_tokens *new_command;

	new_command = ft_add_token(attributes);
	temp_line = line;
	i = 0;
	i++;
	new_command->type = t_command;
	while (temp_line[i] != '"') // removed line check trust in the syntax check
		i++;
	//printf("char\n"); 
	if (!ft_is_whitespace(&temp_line[i+1]))
		new_command->merge = 1;
	new_command->str = ft_substr(line, 1, i-1); // malloc here remeber to check and free after parsing
	return (line + i + 1);
}

void	ft_tokenization(t_mini *attributes)
{
	char		*line;

	attributes->tokens = NULL;
	line = attributes->readret;
	//printf("line |%s|\n",line);
	while (*line)
	{
		line += ft_skip_whitespace(line); // function that skips whitespace
		if (*line && ft_is_special(line))
			ft_add_pipe(attributes, &line);
		if (*line && ft_is_quote(line))
			line = ft_add_quote(line, attributes);
		if (*line && *line == '"')
			line = ft_add_expansion(attributes, line);
		if (*line && *line != ' ' && !ft_is_special(line) && *line != '"' && *line != '\'') // change this to else?
			line = ft_add_command(line, attributes);
	}
	//print_tokens(attributes);
}
