/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokens.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:06:47 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/10 21:52:22 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_add_operator(t_mini *attributes, char *line)
{
	t_tokens	*new_command;
	int			size;

	size = 1;
	new_command = ft_add_token(attributes);
	if (new_command == NULL)
		return (-1);
	if (line[0] == '|')
		new_command->type = t_pipe;
	if (line[0] == '>')
		new_command->type = t_great;
	if (line[0] == '<')
		new_command->type = t_less;
	if (!(ft_strncmp(line, ">>", 2)))
		new_command->type = t_greatgreat;
	if (!(ft_strncmp(line, "<<", 2)))
		new_command->type = t_lessless;
	if (new_command->type == t_lessless || new_command->type == t_greatgreat)
		size++;
	new_command->str = ft_substr(line, 0, size);
	if (!new_command->str)
		return (-1);
	return (size);
}

int	ft_add_command(char *line, t_mini *attributes)
{
	t_tokens	*new_command;
	char		*temp_line;
	int			i;

	i = 0;
	temp_line = line;
	new_command = ft_add_token(attributes);
	if (new_command == NULL)
		return (-1);
	new_command->type = t_command;
	while (!(ft_is_whitespace(&temp_line[i])) && !(ft_is_special(&temp_line[i])) && temp_line[i] != '"' && temp_line[i] != '\'')
	{
		if (temp_line[i] == '\0')
			break ;
		i++;
	}
	if (!ft_is_whitespace(&temp_line[i]))
		new_command->merge = 1;
	new_command->str = ft_substr(line, 0, i);
	if (new_command->str == NULL)
		return (-1);
	return (i);
}

int	ft_add_quote(char *line, t_mini *attributes)
{
	t_tokens	*new_command;
	char		*temp_line;
	int			i;

	i = 1;
	temp_line = line;
	new_command = ft_add_token(attributes);
	if (new_command == NULL)
		return (-1);
	new_command->type = t_quote;
	while (!ft_is_quote(&line[i]) && temp_line[i] != '\0')
		i++;
	i++;
	if (!ft_is_whitespace(&temp_line[i]))
		new_command->merge = 1;
	new_command->str = ft_substr(line, 1, i-2);
	if (new_command->str == NULL)
		return (-1);
	return (i);
}

int	ft_add_expansion(t_mini *attributes, char *line)
{
	int			i;
	char		*temp_line;
	t_tokens	*new_command;

	temp_line = line;
	i = 1;
	new_command = ft_add_token(attributes);
	if (new_command == NULL)
		return (-1);
	new_command->type = t_command;
	while (temp_line[i] != '"') // removed line check trust in the syntax check
		i++; 
	if (!ft_is_whitespace(&temp_line[i + 1]))
		new_command->merge = 1;
	new_command->str = ft_substr(line, 1, i - 1);
	if (new_command->str == NULL)
		return (-1);
	return (i + 1);
}

int	ft_tokenization(t_mini *attributes)
{
	char	*line;
	int		i;
	int		check;
	
	i = 0;
	attributes->tokens = NULL;
	line = attributes->readret;
	while (line[i] != '\0')
	{
		check = ft_skip_whitespace(&line[i]);
		if (line[i] && ft_is_special(&line[i]))
			check = ft_add_operator(attributes, &line[i]);
		if (line[i] && ft_is_quote(&line[i])) // quote function reduntant?
			check = ft_add_quote(&line[i], attributes);
		if (line[i] && line[i] == '"')
			check = ft_add_expansion(attributes, &line[i]);
		if (line[i] && line[i] != ' ' && !ft_is_special(&line[i]) && line[i] != '"' && line[i] != '\'')
			check = ft_add_command(&line[i], attributes);
		if (check == -1)
			return (0);
		else
			i += check;
	}
	//print_tokens(attributes);
	return (1);
}
