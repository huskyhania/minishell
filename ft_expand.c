/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:11:35 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/08 17:17:05 by llaakson         ###   ########.fr       */
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
	temp2 = ft_calloc(len + 1,(sizeof(char)));
	if (!temp2)
		printf("poop\n");
	start = ft_strnstr(token, path, ft_strlen(token));
	temp = ft_substr(token, 0, (start - token));
	ft_strlcat(temp2, temp, len);
	ft_strlcat(temp2, expansion, len);
	ft_strlcat(temp2, start + ft_strlen(path), len);
	free(token);
	free(temp);
	return (temp2);
}

void ft_expand_small(t_mini *attributes, t_tokens *token, int j)
{
	char *exitcode;
	char *expansion;
	char *path;

	if (token->str[j+1] == '?')
	{
		exitcode = ft_itoa(attributes->exitcode);
		path = ft_substr(token->str,j,2);
		token->str = ft_replace_expansion(token->str,path,exitcode);
		free(exitcode);
	}
	else
	{
		expansion = get_env_value(attributes, "SYSTEMD_EXEC_PID");
		path = ft_substr(token->str,j,2);
		token->str = ft_replace_expansion(token->str,path,expansion);
	}
	free(path);
}

void ft_expand_big(t_mini *attributes, t_tokens *token, int j, int i)
{
	char *path;
	char *expansion;

	i++;
	while (token->str[j+i] != '\0' && (token->str[j+i] == '_' || ft_isalpha(token->str[j+i])))
		i++;
	path = ft_substr(token->str, j, i);
	expansion = get_env_value(attributes, &path[1]);
	if (!expansion)
	{
		token->str = ft_replace_expansion(token->str,path,"");
		free(path);
		return ;
	}
	token->str = ft_replace_expansion(token->str,path,expansion);
	free(path);
}

void	ft_expand_word(t_mini *attributes, t_tokens *token)
{
	int i;
	int j;

	j = 0;
	i = 0;
	while (token->str[j] != '\0')
	{
		if (token->str[j] == '$' && (ft_isalpha(token->str[j+1]) || token->str[j+1] == '_' || token->str[j+1] == '?' || token->str[j+1] == '$'))
		{
			if (token->str[j+1] == '?' || token->str[j+1] == '$')
			{
				ft_expand_small(attributes, token, j);
				break ;
			}
			else
			{
				ft_expand_big(attributes, token, j, i);
				break ;
			}
		}
		j++;
	}
	if(ft_strchr(token->str+j,36)) // 36 = $
		ft_expand_word(attributes, token);
}

int	ft_expand(t_mini *attributes)
{		
		t_tokens *token;
		token = attributes->tokens;
		while (token)
		{
			if (token->type == t_command)
				ft_expand_word(attributes, token);
			token = token->next;
		}
		ft_merge_tokens(attributes);
		return (1);
}
