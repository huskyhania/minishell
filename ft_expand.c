/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:11:35 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/24 11:20:16 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_expand_big2(t_mini *attributes, t_tokens *token, char *path)
{
	char	*expansion;

	expansion = get_env_value(attributes, &path[1]);
	if (!expansion)
	{
		token->str = ft_replace_expansion(token->str, path, "");
		if (token->str == NULL)
			return (check_and_free_string(path), 0);
		if (token->failexp == -1)
			token->failexp = 1;
		free(path);
		return (1);
	}
	token->str = ft_replace_expansion(token->str, path, expansion);
	if (token->str == NULL)
		return (check_and_free_string(path), 0);
	token->failexp = 0;
	free(path);
	path = NULL;
	return (1);
}

int	ft_expand_big(t_mini *attributes, t_tokens *token, int j, int i)
{
	char	*path;

	i++;
	while (token->str[j + i] != '\0' && (token->str[j + i] == '_'
			|| ft_isalpha(token->str[j + i])))
		i++;
	path = ft_substr(token->str, j, i);
	if (path == NULL)
		return (0);
	if (!(ft_expand_big2(attributes, token, path)))
		return (0);
	return (1);
}

int	ft_expand_word(t_mini *attributes, t_tokens *t, int j)
{
	if (!t || !t->str)
		return (0);
	while (t->str[j] != '\0')
	{
		if (ft_check_expansion(t->str, j))
		{
			if (t->str[j + 1] == '?'
				|| t->str[j + 1] == '$' || ft_isdigit(t->str[j + 1]))
			{
				if (!ft_expand_small(attributes, t, j))
					return (0);
			}
			else
				if (!ft_expand_big(attributes, t, j, 0))
					return (0);
			break ;
		}
		j++;
	}
	if (ft_strchr(t->str + j, 36))
		if (!(ft_expand_word(attributes, t, 0)))
			return (0);
	if (t->failexp == -1)
		t->failexp = 0;
	return (1);
}

int	ft_expand_command(t_tokens *token, t_mini *attributes)
{
	if (ft_strncmp(token->str, "$", 2) == 0 && token->merge == 1
		&& token->next != NULL && token->dollar == 0)
	{
		token->str = ft_replace_expansion(token->str, token->str, "");
		if (token->str == NULL)
			return (syscall_fail(2, attributes, "malloc"), 0);
	}				
	else if (!(ft_expand_word(attributes, token, 0)))
		return (syscall_fail(2, attributes, "malloc"), 0);
	return (1);
}

int	ft_expand(t_mini *attributes)
{		
	t_tokens	*token;

	token = attributes->tokens;
	while (token)
	{
		if (token->type == t_command)
		{
			if (!(ft_expand_command(token, attributes)))
				return (0);
		}
		else if (token->type == t_quote)
			token->failexp = 0;
		else if (token->type == 5)
		{
			while (token->next && (token->next->type == t_command
					|| token->next->type == t_quote))
			{
				token = token->next;
				token->failexp = 0;
			}
		}
		token = token->next;
	}
	return (1);
}
