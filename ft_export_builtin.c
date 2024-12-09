/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_builtin.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 16:18:06 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/12/09 12:49:33 by hskrzypi         ###   ########.fr       */
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
		write(STDOUT_FILENO, entry, (eq_sign - entry) + 1);
		printf("\"%s\"\n", eq_sign + 1);
	}
	else
		printf("%s\n", entry);
}

int	print_export(char **envp)
{
	size_t	i;
	//char	**sorted_env;

	i = 0;
	while (envp[i])
	{
		print_var(envp[i]);
		i++;
	}
	return (0);
}
/*
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
	char	*new_entry;

	if (!is_export_valid(arg))
		return (1);
	value = ft_strchr(arg, '=');
	if (value)
		*value++ = '\0';
	entry_exist = find_in_envp(*envp, arg);
	if (entry_exist)
	{
		if (value)
		{
			free(entry_exist);
			new_entry = ft_strjoin(arg, '=');
*/


int	ft_export(char **cmd_array, char **envp_copy)
{
	int	i;
	int	ret;

	i = 1;
	ret = 0;
	//if export is called without any parameter - it should print similar to env
	if (!cmd_array[i])
		print_export(envp_copy);
	//while (cmd_array[i] != NULL)
	//{
	//	if (export_single_var(envp_copy, cmd_array[i]) == 1)
	//	{
	//		printf("export error");
	//		ret = 1;
	//	}
	//	i++;
	//}
	return (ret);
}
