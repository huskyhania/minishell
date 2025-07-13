/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:48:06 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/24 16:16:03 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_final_exit(t_mini *attributes, int exit_number)
{
	rl_clear_history();
	envp_cleanup(attributes);
	ft_free_ast(attributes);
	if (attributes->pids)
	{
		free(attributes->pids);
		attributes->pids = NULL;
	}
	if (attributes->cmd_index > 1)
		free_pipes(attributes);
	exit(exit_number);
}

int	ft_is_exit_digit(char *exit_str)
{
	int	i;
	int	ret;

	ret = 0;
	i = 0;
	while (exit_str[i] != '\0')
	{
		if (ft_isdigit(exit_str[i]) == 0)
			ret++;
		i++;
	}
	if (ret > 1)
		return (ret);
	if (ft_strlen(exit_str) > 19)
		return (1);
	if (exit_str[0] == '-')
		return (ft_strcmp(exit_str, "-9223372036854775808") > 0);
	else
	{
		if (exit_str[0] == '+')
			exit_str++;
		return (ft_strcmp(exit_str, "9223372036854775807") > 0);
	}
}

void	ft_exit(char **cmd_array, t_mini *attributes)
{
	int	exit_number;

	exit_number = 0;
	ft_putstr_fd("exit\n", 1);
	if (cmd_array[1] == NULL)
		ft_final_exit(attributes, attributes->exitcode);
	if (ft_is_exit_digit(cmd_array[1]))
	{
		exit_number = 2;
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(cmd_array[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		ft_final_exit(attributes, exit_number);
	}
	if (cmd_array[1] && cmd_array[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		attributes->exitcode = 1;
		return ;
	}
	else if (cmd_array[1] && exit_number == 0)
		exit_number = ft_atoi(cmd_array[1]);
	ft_final_exit(attributes, exit_number);
}
