///* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainh.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:37:20 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/01 21:28:57 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
int ft_check_everything(t_mini *attributes)
{
	if(!(ft_check_quotes(attributes->readret)))
		return (0);
	ft_tokenization(attributes);
	if (!(ft_syntax_check(attributes)))
		return (0);
	ft_expand(attributes);
	ft_parsing(attributes);
	return (1);
}
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
	attributes.i = 0;
	attributes.exitcode = 0;
	while (1)
	{
		if(!(attributes.readret = readline(PROMPT)))
		{
			printf("CTRL+D found");
			exit(0); // check that the exit number is correct for ctrl+d, also million things need to be freed here??
		}
		if (attributes.readret && attributes.readret[0] != '\0')
		{
			add_history(attributes.readret);
			printf("\nyour shitty input was %s\n", attributes.readret);
			if (ft_check_everything(&attributes))
				ft_execution(&attributes);
		}
 	}
	//clean history after breaking the loop???
	envp_cleanup(&attributes);
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
	{
		ft_sigint();
		minishell_init(envp);
	}
	return (0);
}
