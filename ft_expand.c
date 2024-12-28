/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:11:35 by llaakson          #+#    #+#             */
/*   Updated: 2024/12/28 17:28:35 by llaakson         ###   ########.fr       */
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
	printf("len %d\n", len);
	temp2 = ft_calloc(len + 1,(sizeof(char)));
	if (!temp2)
		printf("poop\n");
	printf("token: %s< path: %s<\n",token,path);
	start = ft_strnstr(token, path, ft_strlen(token));
	printf("token: %s< start: %s<\n",token,start);
	temp = ft_substr(token, 0, (start - token));
	printf("temp2: %s< temp: %s<\n",temp2,temp);
	ft_strlcat(temp2, temp, len);
	//printf("TEMP2 %s\n", temp2);
	ft_strlcat(temp2, expansion, len);
	//printf("TEMP2 %s\n", temp2);
	ft_strlcat(temp2, start + ft_strlen(path), len);
	//printf("TEMP2 %s\n", temp2);
	return (temp2);

}

void	ft_expand_word(t_mini *attributes, t_tokens *token)
{
	char *path;
	int i;
	int j;
	char *expansion;
	char *exitcode;

	j = 0;
	i = 0;
	while (token->str[j] != '\0')
	{
		if (token->str[j] == '$' && (ft_isalpha(token->str[j+1]) || token->str[j+1] == '_' || token->str[j+1] == '?' || token->str[j+1] == '$'))
		{
			if (token->str[j+1] == '?')
			{
					exitcode = ft_itoa(attributes->exitcode);
					printf("Exitcode %d\n", attributes->exitcode);
					token->str = ft_replace_expansion(token->str,ft_substr(token->str,j,2),exitcode);
					break ;
			}
			if (token->str[j+1] == '$')
			{
				expansion = get_env_value(attributes, "SYSTEMD_EXEC_PID");
				printf("Case $$ %s\n", expansion);
				token->str = ft_replace_expansion(token->str,ft_substr(token->str,j,2),expansion);
				break ;
			}
			while (token->str[j+i] != '\0' && (token->str[j+i] == '$' || token->str[j+i] == '_' || ft_isalpha(token->str[j+i])))
				i++;
			path = ft_substr(token->str, j, i);
			printf("path }%s{\n", path);
			expansion = get_env_value(attributes, &path[1]);
			if (!expansion)
			{
				//printf("no value in env\n");
				token->str = ft_replace_expansion(token->str,path,"");
				free(path);
				break ;
			}
			token->str = ft_replace_expansion(token->str,path,expansion);
			free(path);
			break ;
		}
		j++;
	}
	if(ft_strchr(token->str+j,36)) // 36 = $
		ft_expand_word(attributes, token);
	//free(path);
}

void	ft_expand(t_mini *attributes)
{		
		t_tokens *token;
		token = attributes->tokens;
		while (token && token->type == t_command)
		{
				ft_expand_word(attributes, token);
			token = token->next;
		}
}
