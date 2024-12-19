///* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainh.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:37:20 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/12/19 14:30:52 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	minishell_init(char **envp)
{
	t_mini	attributes;
	attributes.readret = NULL;
	attributes.envp_heap = NULL;
	attributes.envp_heap = envp_to_list(envp);
	attributes.envp_arr = envp_to_array(attributes.envp_heap);
	attributes.commands = NULL;
	attributes.input_fd = STDIN_FILENO;
	attributes.output_fd = STDOUT_FILENO;
	while (1)
	{
		attributes.readret = readline(PROMPT);
		if (attributes.readret && attributes.readret[0] != '\0')
		{
			add_history(attributes.readret);
			printf("\nyour shitty input was %s\n", attributes.readret);
			ft_tokenization(&attributes);
			ft_parsing(&attributes);
			//printf("node content %s\n", attributes.commands->str);
			//printf("left node %s\n", attributes.commands->left->str);
			ft_execution(&attributes);
		}
 	}
	//clean history after breaking the loop???
 }

int main(int argc, char **argv, char **envp)
{
	(void)argv;
	if (argc > 1)
	{
		ft_putstr_fd("minishell: Too many arguments\n", 2);
		return (127);
	}
	else 
		minishell_init(envp);
	return (0);
}
