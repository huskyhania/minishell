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
			ft_putstr_fd("variable update fail\n", 2);
			return (1);
		}
	}
	else if (!create_node(export, &attributes->envp_heap))
	{
		ft_putstr_fd("malloc fail in node creation\n", 2);
		return (1);
	}
	return (0);
}

static void	print_var(t_envp *var)
{
	if (var && var->key)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(var->key, STDOUT_FILENO);
		if (var->value)
		{
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putstr_fd("\"", STDOUT_FILENO);
			ft_putstr_fd(var->value, STDOUT_FILENO);
			ft_putstr_fd("\"", STDOUT_FILENO);
		}
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
}

void	print_export(t_cmd_table *node, t_mini *attributes)
{
	t_envp	*helper;
	int		saved_std;

	helper = attributes->envp_heap;
	saved_std = save_std(attributes, STDOUT_FILENO);
	if (node->out1)
	{
		if (redir_out(node, attributes))
			return ;
	}
	while (helper)
	{
		print_var(helper);
		helper = helper->next;
	}
	restore_std(saved_std, STDOUT_FILENO, attributes);
}

int	ft_export(t_cmd_table *node, t_mini *attributes)
{
	int	i;
	int	ret;

	i = 1;
	ret = 0;
	attributes->exitcode = 0;
	if (!node->cmd_arr[i])
		print_export(node, attributes);
	while (node->cmd_arr[i] != NULL)
	{
		if (export_single_var(attributes, node->cmd_arr[i]) == 1)
		{
			attributes->exitcode = 1;
			ret = 1;
		}
		i++;
	}
	return (ret);
}
