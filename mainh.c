/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainh.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:37:20 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/12/02 17:59:49 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	mini	attributes;
	attributes.readret = NULL;
	while (1)
	{
		attributes.readret = readline(PROMPT);
		if (attributes.readret)
		{
			add_history(attributes.readret);
			printf("\n your shitty input was %s\n", attributes.readret);
		}
	}
	return (0);
}
