/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_builtin.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 16:18:06 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/12/07 19:24:45 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_var(char *entry)
{
	char *eq_sign;

	ft_putstr_fd("declare -x", STDOUT_FILENO);
	eq_sign = ft_strchr(entry, '=');
	if (eq_sign)
	{
		write(STDOUT_FILENO, entry, (eq - entry) + 1);
		printf("\"%s\"\n", eq_sign + 1);
	}
	else
		printf("%s\n", entry);
}

int	print_export(char **envp)
{
	size_t	i;
	char	**sorted_env;

	i = 0;
	sorted_env = env_clone(envp);
	if (!sorted_env)
		return (1);
	env_sort(sorted_env);
	while (sorted_env[i])
	{
		if (sorted_env[i][0] != '\0')
			print_var(sorted_env[i]);
		i++;
	}
	free_array(sorted_env);
	return (0);
}

void	remove_eq_sign(char **envp, char *arg)
{
	char	*ptr;
	ptr = NULL;

	ptr = find_in_env(envp, arg);
	if (!ptr)
		return ;
	*(ptr - 1) = '\0';
}

int	export_single_var(char **envp, char *arg)
{
	char	*value;
	char	*entry_exist;

	if (!is_export_valid(arg))
		return (1);
	val = ft_strchr(arg, '=');
	if (val)
		*val++ = '\0';
	entry_exist = find_in_envp(*envp, arg);
	if (entry_exist)
		remove_env_var(cmd_array, envp);// potrzebuje calej array?!


int	ft_export(char **cmd_array, char **envp_copy)
{
	int	i;
	int	ret;

	i = 1;
	ret = 0;
	//if export is called without any parameter - it should print similar to env
	if (!cmd_array[i])
		print_export(envp_copy);
	while (cmd_array[i] != NULL)
	{
		if (export_single_var(envp_copy, cmd_array[i]) == 1)
		{
			printf("export error");
			ret = 1;
		}
		i++;
	}
	return (ret);
}
