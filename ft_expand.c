/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:11:35 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/09 19:34:35 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_replace_expansion(char *token, char *path, char *expansion)
{
	char *temp;
	char *temp2;
	char *start;
	int len;

	len = ft_strlen(token) + ft_strlen(expansion) - ft_strlen(path) + 1; 
	temp2 = ft_calloc(len + 1,(sizeof(char))); // too much malloc?
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

int ft_expand_small(t_mini *attributes, t_tokens *token, int j)
{
	char *exitcode;
	char *expansion;
	char *path;

	if (token->str[j+1] == '?')
	{
		if(!(exitcode = 0 )) //ft_itoa(attributes->exitcode)))
			return (0);
		if(!(path = ft_substr(token->str,j,2)))
		{
			free(exitcode);
			return (0);
		}
		if(!(token->str = ft_replace_expansion(token->str,path,exitcode)))
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
		if (!(path = ft_substr(token->str,j,2)))
			return (0);
		if(!(token->str = ft_replace_expansion(token->str,path,expansion)))
		{
			free(path);
			return (0);
		}
	}
	free(path);
	return (1);
}

int	ft_expand_big(t_mini *attributes, t_tokens *token, int j, int i)
{
	char *path;
	char *expansion;

	i++;
	while (token->str[j+i] != '\0' && (token->str[j+i] == '_' || ft_isalpha(token->str[j+i])))
		i++;
	path = ft_substr(token->str, j, i);
	if (path == NULL)
		return (0);
	expansion = get_env_value(attributes, &path[1]);
	if (!expansion)
	{
		token->str = ft_replace_expansion(token->str,path,"");
		free(path);
		return (1);
	}
	if (!(token->str = ft_replace_expansion(token->str,path,expansion)))
	{
		free(path);
		return (0);
	}
	free(path);
	return (1);
}

int	ft_expand_word(t_mini *attributes, t_tokens *token)
{
	int i;
	int j;

	j = 0;
	i = 0;
	if (!token || !token->str)
		return (0);
	while (token->str[j] != '\0')
	{
		if (token->str[j] == '$' && (ft_isalpha(token->str[j+1]) || token->str[j+1] == '_' || token->str[j+1] == '?' || token->str[j+1] == '$'))
		{
			if (token->str[j+1] == '?' || token->str[j+1] == '$')
			{
				if (!ft_expand_small(attributes, token, j))
					return (0);
				break ;
			}
			else
			{
				if (!ft_expand_big(attributes, token, j, i))
					return (0);
				break ;
			}
		}
		j++;
	}
	return (1);
}

int	ft_expand(t_mini *attributes)
{		
		t_tokens *token;
		int i;

		i = 0;
		token = attributes->tokens;
		while(token)
		{
			while (token && ft_strchr(token->str+i, '$'))
			{
				if (token->type == t_command)
					if(!ft_expand_word(attributes, token))
					{
						printf("Parsing error\n");
						return (0);
					}
				i++;
			}
			token = token->next;
		}
		ft_merge_tokens(attributes);
		return (1);
}
