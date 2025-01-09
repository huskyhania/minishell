/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokens.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:06:47 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/09 21:24:18 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_add_token_type(t_tokens *new_command, char **line, int size)
{
	new_command->str = ft_substr(*line, 0, size);
}

int	ft_add_operator(t_mini *attributes, char *line)
{
	t_tokens *new_command;
	int size;
	
	size = 1;
	new_command = ft_add_token(attributes);
	if (new_command == NULL)
		return (0);
	if (line[0] == '|')
		new_command->type = t_pipe;
	if (line[0] == '>')
		new_command->type = t_great;
	if (line[0] == '<')
		new_command->type = t_less;
	if (line[0] == '(')
		new_command->type = t_bracketleft;
	if (line[0] == ')')
		new_command->type = t_bracketrigth;
	if (!(ft_strncmp(line, ">>", 2)))
		new_command->type = t_greatgreat;
	if (!(ft_strncmp(line, "<<", 2)))
		new_command->type = t_lessless;
	if (new_command->type == t_lessless || new_command->type == t_greatgreat)
		size++;
	new_command->str = ft_substr(line, 0, size);
	if (!new_command->str)
		return (0);
	//(*line) += size;
	return (size);
}

int		ft_add_special(char *line, t_mini *attributes)
{
		if (line[0] == '|' || line[0] == '>' || line[0] == '<' || line[0] == '(' || line[0] == ')')
		{
			ft_add_operator(attributes, line);
			return (1);
		}
		return (0);
	
}
//Bug?? This functions can add < > () | to the command arg. if middle or at the end of the arg.
int	ft_add_command(char *line, t_mini *attributes)
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
		return (i);
}

int	ft_add_quote(char *line, t_mini *attributes)
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
	return (i);
}

int ft_add_expansion(t_mini *attributes, char *line)
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
	if (!ft_is_whitespace(&temp_line[i+1]))
		new_command->merge = 1;
	new_command->str = ft_substr(line, 1, i-1); // malloc here remeber to check and free after parsing
	return (i + 1);
}

int	ft_tokenization(t_mini *attributes)
{
	char		*line;
	int i;
	int check;
	
	i = 0;
	attributes->tokens = NULL;
	line = attributes->readret;
	while (line[i] != '\0')
	{
		check = ft_skip_whitespace(&line[i]);
		if (line[i] && ft_is_special(&line[i]))
			check = ft_add_operator(attributes, &line[i]);
		if (line[i] && ft_is_quote(&line[i]))
			check= ft_add_quote(&line[i], attributes);
		if (line[i] && *line == '"')
			check = ft_add_expansion(attributes, &line[i]);
		if (line[i] && line[i] != ' ' && !ft_is_special(&line[i]) && *line != '"' && *line != '\'')
			check = ft_add_command(&line[i], attributes);
		if (check == 0)
			return (0);
		else
			i += check;
	}
	//print_tokens(attributes);
	return (1);
}
