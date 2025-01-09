/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:48:06 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/09 17:47:06 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_final_exit(t_mini *attributes, int exit_number)
{
	envp_cleanup(attributes);
	ft_free_ast(attributes);
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
	printf("exit\n");
	if (cmd_array[1] == NULL)
	{
		exit_number = attributes->exitcode;
		ft_final_exit(attributes, exit_number);
	}
	if (ft_is_exit_digit(cmd_array[1]))
	{
		exit_number = 2;
		printf("exit: %s: numeric argument required\n", cmd_array[1]);
		ft_final_exit(attributes, exit_number);
	}
	if (cmd_array[1] && cmd_array[2])
	{
		printf("exit: too many arguments\n");
		attributes->exitcode = 1;
		return ;
	}
	else if (cmd_array[1] && exit_number == 0)
		exit_number = ft_atoi(cmd_array[1]);
	ft_final_exit(attributes, exit_number);
}

void	ft_child_exit(char **cmd_array, t_mini *attributes)
{
	int	exit_number;

	exit_number = 0;
	if (cmd_array[1] == NULL)
		exit_number = attributes->exitcode;
	else if (ft_is_exit_digit(cmd_array[1]))
	{
		exit_number = 2;
		printf("exit: %s: numeric argument required\n", cmd_array[1]);
	}
	else if (cmd_array[1] && cmd_array[2])
	{
		printf("exit: too many arguments\n");
		exit_number = 1;
	}
	else if (cmd_array[1] && exit_number == 0)
		exit_number = ft_atoi(cmd_array[1]);
	attributes->exitcode = exit_number;
}
