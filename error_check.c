/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:14:58 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/02 16:31:08 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_error_and_display(int error_code, t_mini *attributes, const char *cmd)
{
	attributes->exitcode = error_code;
	ft_putstr_fd((char *)cmd, 2);
	if (error_code == 126)
		ft_putstr_fd(": permissions denied", 2);
	if (error_code == 127)
		ft_putstr_fd(": command not found\n", 2);
	if (error_code == 1)
	{
		if (access(cmd, F_OK) == 0)
			ft_putstr_fd(": Permission denied\n", 2);
		else
			ft_putstr_fd(": No such file or directory\n", 2);
	}
}
