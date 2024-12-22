/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:11:35 by llaakson          #+#    #+#             */
/*   Updated: 2024/12/22 13:34:28 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
char	*ft_replace_expansion(char *token, char *path, char *expansion)
{
	char *temp;
	char *temp2;
	char *start;
	int len;

	len = ft_strlen(token) + ft_strlen(expansion) - ft_strlen(path);
	printf("len %d\n", len);
	temp2 = malloc((len + 1)* sizeof(char));
	if (!temp2)
		printf("poop\n");
	start = ft_strnstr(token, path, ft_strlen(token));
	temp = ft_substr(token, 0, (start - token));
	ft_strlcat(temp2, temp, len+1);
	//printf("TEMP2 %s\n", temp2);
	ft_strlcat(temp2, expansion, len+1);
	//printf("TEMP2 %s\n", temp2);
	ft_strlcat(temp2, start + ft_strlen(path), len+1);
	//printf("TEMP2 %s\n", temp2);
	return (temp2);

}

void	ft_expand_word(t_mini *attributes, t_tokens *token)
{
	char *path;
	int i;
	int j;
	char *expansion;

	j = 0;
	i = 1;
	while (token->str[j] != '\0')
	{
		if (token->str[j] == '$')
		{
			while (token->str[j+i] != '\0' && token->str[j+i] != ' ') // more whitespace?
				i++;
			path = ft_substr(token->str, j, i);
			//if (!path)
			//	return (NULL);
			//printf("path }%s{\n", path);
			expansion = get_env_value(attributes, &path[1]);
			if (!expansion)
			{
				printf("no value in env\n");
				break ;
			}
			token->str = ft_replace_expansion(token->str,path,expansion);
			break ;
		}
		j++;
	}
	if(ft_strchr(token->str,36))
		ft_expand_word(attributes, token);
	//free(path);
}

void	ft_expand(t_mini *attributes)
{
		char *expansion;
		char *exitcode;
		
		t_tokens *token;
		token = attributes->tokens;
		while (token)
		{
			if (token->type == t_dollar)
			{
				if (token->str[0] == '?'  && (ft_strlen(token->str) == 1))
				{
					exitcode = ft_itoa(attributes->exitcode);
					printf("Exitcode %d\n", attributes->exitcode);
					token->str = exitcode;
				}
				else
				{
					expansion = get_env_value(attributes, token->str);
					if (!expansion)
					{
						printf("no value in env\n");
						token->str = "";
						break ;
					}
					else
					{
						token->str = ft_strdup(expansion);
						printf("FOUND PATH: %s. \n", expansion);
					}
				}
			}
			else if (token->type == t_command)
				ft_expand_word(attributes, token);
			token = token->next;
		}
}
