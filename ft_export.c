/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 19:57:10 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/19 20:00:01 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_export(char *export)
{
	int	i;

	i = 0;
	if (export[i] != '_' && !ft_isalpha(export[i]))
		return (1);
	i++;
	while (export[i] != '\0' && export[i] != '=')
	{
		if (export[i] == '+' && export[i + 1] == '=')
			break ;
		if (!ft_isalnum(export[i]) && export[i] != '_')
			return (1);
		i++;
	}
	if (export[i] == '=' || (export[i] == '+' && export[i + 1] == '='))
		return (0);
	return (0);
}

int	export_single_var(t_mini *attributes, char *export)
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
		if (helper->key)
		{
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putstr_fd(helper->key, STDOUT_FILENO);
			if (helper->value)
			{
				ft_putstr_fd("=", STDOUT_FILENO);
				ft_putstr_fd("\"", STDOUT_FILENO);
				ft_putstr_fd(helper->value, STDOUT_FILENO);
				ft_putstr_fd("\"", STDOUT_FILENO);
			}
			ft_putstr_fd("\n", STDOUT_FILENO);
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
}
