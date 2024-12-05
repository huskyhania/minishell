/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainh.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:37:20 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/12/05 14:15:51 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	t_mini	attributes;
	attributes.readret = NULL;
	attributes.envp_copy = envp;
	while (1)
	{
		attributes.readret = readline(PROMPT);
		if (attributes.readret)
		{
			add_history(attributes.readret);
			printf("\n your shitty input was %s\n", attributes.readret);
			ft_tokenization(&attributes);
			ft_parsing(&attributes);
			ft_execution(&attributes);
		}
	}
	return (0);
}
