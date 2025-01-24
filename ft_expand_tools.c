/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:28:25 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/24 11:57:12 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_expansion2(char *token, char *path, char *exp, char *temp2)
{
	char	*temp;
	char	*start;
	int		len;

	len = ft_strlen(temp2) - 1;
	start = ft_strnstr(token, path, ft_strlen(token));
	if (start == NULL)
		return (check_and_free_double(token, temp2), NULL);
	temp = ft_substr(token, 0, (start - token));
	if (temp == NULL)
		return (check_and_free_double(token, temp2), NULL);
	ft_strlcat(temp2, temp, len);
	ft_strlcat(temp2, exp, len);
	ft_strlcat(temp2, start + ft_strlen(path), len);
	free(token);
	token = NULL;
	free(temp);
	temp = NULL;
	return (temp2);
}

char	*ft_replace_expansion(char *token, char *path, char *expansion)
{
	char	*temp2;
	int		len;

	len = ft_strlen(token) + ft_strlen(expansion) - ft_strlen(path) + 1;
	temp2 = ft_calloc(len + 1, (sizeof(char)));
	if (temp2 == NULL)
		return (check_and_free_string(token), NULL);
	temp2 = ft_expansion2(token, path, expansion, temp2);
	if (temp2 == NULL)
		return (NULL);
	return (temp2);
}

int	ft_expand_exitcode(t_mini *attributes, t_tokens *token, int j)
{
	char	*exitcode;
	char	*path;

	exitcode = ft_itoa(attributes->exitcode);
	if (exitcode == NULL)
		return (0);
	path = ft_substr(token->str, j, 2);
	if (path == NULL)
		return (check_and_free_string(exitcode), 0);
	token->str = ft_replace_expansion(token->str, path, exitcode);
	if (token->str == NULL)
		return (check_and_free_double(path, exitcode), 0);
	free(path);
	path = NULL;
	free(exitcode);
	exitcode = NULL;
	return (1);
}

int	ft_expand_pid(t_mini *attributes, t_tokens *token, int j)
{
	char	*path;
	char	*expansion;

	expansion = get_env_value(attributes, "SYSTEMD_EXEC_PID");
	if (expansion == NULL)
		return (0);
	path = ft_substr(token->str, j, 2);
	if (path == NULL)
		return (0);
	token->str = ft_replace_expansion(token->str, path, expansion);
	if (token->str == NULL)
		return (check_and_free_string(path), 0);
	free(path);
	return (1);
}

int	ft_expand_small(t_mini *attributes, t_tokens *token, int j)
{
	char	*path;

	if (token->str[j + 1] == '?')
	{
		if (!(ft_expand_exitcode(attributes, token, j)))
			return (0);
	}
	else if (ft_isdigit(token->str[j + 1]))
	{
		path = ft_substr(token->str, j, 2);
		if (path == NULL)
			return (0);
		token->str = ft_replace_expansion(token->str, path, "");
		if (token->str == NULL)
			return (check_and_free_string(path), 0);
		free(path);
		path = NULL;
	}
	else
	{
		if (!(ft_expand_pid(attributes, token, j)))
			return (0);
	}
	token->failexp = 0;
	return (1);
}
