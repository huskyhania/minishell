/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_builtin.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 16:18:06 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/19 19:48:26 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_var_exists(char *export, t_envp **envp_heap)
{
	char	*key;
	t_envp	*current;

	key = get_key(export);
	current = *envp_heap;
	if (!key)
		return (0);
	while (current)
	{
		if (ft_strcmp(key, current->key) == 0)
		{
			free(key);
			return (1);
		}
		current = current->next;
	}
	free(key);
	return (0);
}

int	export_mode(char *export)
{
	int	i;

	i = 0;
	while (export[i] != '\0')
	{
		if (export[i] == '+' && export[i + 1] == '=')
			return (1);
		if (export[i] == '=')
			return (2);
		i++;
	}
	return (2);
}

static void	free_key_value(char *key, char *value)
{
	if (key)
	{
		free(key);
		key = NULL;
	}
	if (value)
	{
		free(value);
		value = NULL;
	}
}

static int	update_value(char *key, char *value, t_envp *current, char *export)
{
	char	*join;

	if (export_mode(export) == 1)
	{
		join = ft_strjoin(current->value, value);
		if (!join)
		{
			free_key_value(key, value);
			return (0);
		}
		check_and_free_string(current->value);
		free_key_value(key, value);
		current->value = join;
		return (1);
	}
	else if (export_mode(export) == 2)
	{
		check_and_free_string(current->value);
		current->value = ft_strdup(value);
		free_key_value(key, value);
		if (!current->value)
			return (0);
		return (1);
	}
	return (0);
}

int	replace_append(char *export, t_envp **envp_heap)
{
	char	*key;
	char	*value;
	t_envp	*current;

	current = *envp_heap;
	key = get_key(export);
	value = get_value(export);
	if (!key || !value)
	{
		free_key_value(key, value);
		return (0);
	}
	while (current)
	{
		if (ft_strcmp(key, current->key) == 0)
			return (update_value(key, value, current, export));
		current = current->next;
	}
	free_key_value(key, value);
	return (0);
}
