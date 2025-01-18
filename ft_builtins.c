/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 16:00:04 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/15 15:30:05 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(t_mini *attributes)
{
	char	*cwd;

	cwd = NULL;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_putstr_fd("pwd: error retrieving current directory", 2);
		attributes->exitcode = 1;
		return ;
	}
	printf("%s\n", cwd);
	free(cwd);
	attributes->exitcode = 0;
}

void	ft_env(t_cmd_table *node, t_mini *attributes)
{
	int	saved_std;

	saved_std = dup(STDOUT_FILENO);
	if (node->out1)
	{
		if (redir_out(node, attributes))
			return ; 
	}
	print_envp_list(attributes->envp_heap);
	update_exitcode(0, attributes);
	dup2(saved_std, STDOUT_FILENO);
	close(saved_std);
}

void	handle_builtin(t_cmd_table *node, int flag, t_mini *attributes)
{
	if (flag == BUILTIN_ECHO)
		ft_echo(node, attributes);
	if (flag == BUILTIN_CD)
		ft_cd(node->cmd_arr, attributes);
	if (flag == BUILTIN_PWD)
		ft_pwd(attributes);
	if (flag == BUILTIN_EXPORT)
		ft_export(node->cmd_arr, attributes);
	if (flag == BUILTIN_UNSET)
		remove_env_var(node->cmd_arr, attributes);
	if (flag == BUILTIN_ENV)
		ft_env(node, attributes);
	if (flag == BUILTIN_EXIT)
		ft_exit(node->cmd_arr, attributes);
	if (flag != BUILTIN_EXIT && attributes->cmd_index > 1)
	{
		cleanup_child(attributes);
		exit(attributes->exitcode);
	}
}

int	is_builtin(char *cmd_text)
{
	if (ft_strcmp(cmd_text, "echo") == 0)
		return (BUILTIN_ECHO);
	if (ft_strcmp(cmd_text, "cd") == 0)
		return (BUILTIN_CD);
	if (ft_strcmp(cmd_text, "pwd") == 0)
		return (BUILTIN_PWD);
	if (ft_strcmp(cmd_text, "export") == 0)
		return (BUILTIN_EXPORT);
	if (ft_strcmp(cmd_text, "unset") == 0)
		return (BUILTIN_UNSET);
	if (ft_strcmp(cmd_text, "env") == 0)
		return (BUILTIN_ENV);
	if (ft_strcmp(cmd_text, "exit") == 0)
		return (BUILTIN_EXIT);
	else
		return (0);
}
