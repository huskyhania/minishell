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
		free(key);
	if (value)
		free(value);
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
		free(current->value);
		free_key_value(key, value);
		current->value = join;
		return (1);
	}
	else if (export_mode(export) == 2)
	{
		free(current->value);
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

/*int	export_single_var(t_mini *attributes, char *export)
{
	if (is_valid_export(export))
	{
		ft_putstr_fd(export, 2);
		ft_putstr_fd(": not a valid identifier\n", 2);
		return (1);
	}
	if (env_var_exists(export, &attributes->envp_heap))
	{
		if (!replace_append(export, &attributes->envp_heap))
		{
			printf("variable update fail\n");
			return (1);
		}
	}
	else if (!create_node(export, &attributes->envp_heap))
	{
		printf("malloc fail in node creation\n");
		return (1);
	}
	return (0);
}

void	print_export(t_mini *attributes)
{
	t_envp	*helper;

	helper = attributes->envp_heap;
	while (helper)
	{
		if (helper->key && helper->key[0] != '\0')
		{
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putstr_fd(helper->key, STDOUT_FILENO);
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putendl_fd(helper->value, STDOUT_FILENO);
		}
		helper = helper->next;
	}
}

int	ft_export(char **cmd_array, t_mini *attributes)
{
	int	i;
	int	ret;

	i = 1;
	ret = 0;
	attributes->exitcode = 0;
	if (!cmd_array[i])
		print_export(attributes);
	while (cmd_array[i] != NULL)
	{
		if (export_single_var(attributes, cmd_array[i]) == 1)
		{
			attributes->exitcode = 1;
			ret = 1;
		}
		i++;
	}
	return (ret);
}*/
