///* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainh.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:37:20 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/12/17 12:11:05 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
	(void)argv;
	t_mini	attributes;

	if (argc > 1)
	{
		ft_putstr_fd("minishell: No such file or directory\n", 2);
		return (127);
	}
	attributes.readret = NULL;
	attributes.envp_heap = NULL;
	attributes.envp_heap = envp_to_list(envp);
	attributes.envp_arr = envp_to_array(attributes.envp_heap);
	attributes.commands = NULL;
	while (1)
	{
		attributes.readret = readline(PROMPT);
		if (attributes.readret)
		{
			add_history(attributes.readret);
			printf("\nyour shitty input was %s\n", attributes.readret);
			ft_tokenization(&attributes);
			ft_parsing(&attributes);
			ft_execution(&attributes);
		}
	}
	return (0);
}
