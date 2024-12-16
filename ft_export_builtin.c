/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_builtin.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 16:18:06 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/12/16 19:24:09 by hskrzypi         ###   ########.fr       */
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
	while (export[i] != '+' && export[i] != '=' && export[i] != '\0')
	{
		if (!ft_isalnum(export[i]) && export [i] != '_')
			return (1);
		i++;
	}
	if (export[i] == '+')
	{
		if (export[i + 1] != '=')
			return (1);
	}
	if (export[i] == '=')
		return (0);
	return (0);
}

int	export_single_var(t_mini *attributes, char *export)
{
	if (is_valid_export(export))
	{
		printf("%s is not a valid identifier\n", export);
		//exitcode 1;
		return (1);
	}
	if (!create_node(export, &attributes->envp_heap))
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
	//if export is called without any parameter - it should print similar to env
	if (!cmd_array[i])
		print_export(attributes);
	while (cmd_array[i] != NULL)
	{
		if (export_single_var(attributes, cmd_array[i]) == 1)
		{
			printf("export error");
			ret = 1;
		}
		i++;
	}
	//print_export(attributes);test if export worked
	return (ret);
}
