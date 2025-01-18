/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:11:35 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/14 21:46:38 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_replace_expansion(char *token, char *path, char *expansion)
{
	char	*temp;
	char	*temp2;
	char	*start;
	int		len;

	len = ft_strlen(token) + ft_strlen(expansion) - ft_strlen(path) + 1;
	temp2 = ft_calloc(len + 1, (sizeof(char))); // too much malloc?
	if (temp2 == NULL)
	{
		free(token);
		return (NULL);
	}
	start = ft_strnstr(token, path, ft_strlen(token)); // no malloc but can return NULL;
	temp = ft_substr(token, 0, (start - token));
	if (temp == NULL)
	{
		free (token);
		free (temp2);
		return (NULL);
	}
	ft_strlcat(temp2, temp, len);
	ft_strlcat(temp2, expansion, len);
	ft_strlcat(temp2, start + ft_strlen(path), len);
	free(token);
	free(temp);
	return (temp2);
}

int	ft_expand_small(t_mini *attributes, t_tokens *token, int j)
{
	char	*exitcode;
	char	*expansion;
	char	*path;

	if (token->str[j + 1] == '?')
	{
		if(!(exitcode = ft_itoa(attributes->exitcode)))
			return (0);
		if(!(path = ft_substr(token->str, j, 2)))
		{
			free(exitcode);
			return (0);
		}
		if(!(token->str = ft_replace_expansion(token->str, path, exitcode)))
		{
			free(exitcode);
			return (0);
		}
		free(exitcode);
	}
	else
	{
		if (!(expansion = get_env_value(attributes, "SYSTEMD_EXEC_PID")))
			return (0);
		if (!(path = ft_substr(token->str, j, 2)))
			return (0);
		if(!(token->str = ft_replace_expansion(token->str, path, expansion)))
		{
			free(path);
			return (0);
		}
	}
	token->failexp = 0;
	free(path);
	return (1);
}

int	ft_expand_big(t_mini *attributes, t_tokens *token, int j, int i)
{
	char	*path;
	char	*expansion;

	i++;
	while (token->str[j + i] != '\0' && (token->str[j + i] == '_'
			|| ft_isalpha(token->str[j + i])))
		i++;
	path = ft_substr(token->str, j, i);
	if (path == NULL)
		return (0);
	expansion = get_env_value(attributes, &path[1]);
	if (!expansion)
	{
		token->str = ft_replace_expansion(token->str, path, "");
		if (token->failexp == -1)
			token->failexp = 1;
		free(path);
		return (1);
	}
	token->str = ft_replace_expansion(token->str, path, expansion);
	if (token->str == NULL)
	{
		free(path);
		return (0);
	}
	token->failexp = 0;
	free(path);
	return (1);
}

int	ft_expand_word(t_mini *attributes, t_tokens *t, int j)
{
	if (!t || !t->str)
		return (0);
	while (t->str[j] != '\0')
	{
		if (t->str[j] == '$' && (ft_isalpha(t->str[j + 1])
				|| t->str[j + 1] == '_'
				|| t->str[j + 1] == '?' || t->str[j + 1] == '$'))
		{
			if (t->str[j + 1] == '?' || t->str[j + 1] == '$')
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

void	ft_convert(t_mini *attributes)
{
	t_tokens        *token;

        token = attributes->tokens;
        while (token)
	{
		if (token->type == t_quote)
			token->type = t_command;
		token = token->next;
	}
}

int	ft_expand(t_mini *attributes)
{		
	t_tokens	*token;

	token = attributes->tokens;
	while (token)
	{
		if (token->type == t_command)
			if (!(ft_expand_word(attributes, token, 0)))
			{
				printf("Parsing error\n");
				return (0);
			}
		if (token->type == t_quote)
			token->failexp = 0;
		if (token->type == 5)
			token = token->next;
		token = token->next;
	}
	//if (!(ft_validate_expansion(attributes)))
	//	return (0);
	ft_merge_tokens(attributes);
	ft_convert(attributes);
	return (1);
}
