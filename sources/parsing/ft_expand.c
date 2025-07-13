/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:11:35 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/24 16:01:34 by llaakson         ###   ########.fr       */
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

int	ft_expand_word(t_mini *attributes, t_tokens *tk, int j)
{
	if (!tk || !tk->str)
		return (0);
	while (tk->str[j] != '\0')
	{
		if (ft_check_expansion(tk->str, j))
		{
			if (tk->str[j + 1] == '?'
				|| tk->str[j + 1] == '$' || ft_isdigit(tk->str[j + 1]))
			{
				if (!ft_expand_small(attributes, tk, j))
					return (0);
			}
			else
				if (!ft_expand_big(attributes, tk, j, 0))
					return (0);
			break ;
		}
		j++;
	}
	if (ft_strchr(tk->str + j, 36))
		if (!(ft_expand_word(attributes, tk, 0)))
			return (0);
	if (tk->failexp == -1)
		tk->failexp = 0;
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
