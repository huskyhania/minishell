///* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainh.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:37:20 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/06 17:31:34 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_signal = 0;

int ft_check_everything(t_mini *attributes)
{
	if(is_empty_or_space(attributes->readret))
		return (0);
	if(!(ft_check_quotes(attributes->readret)))
		return (0);
	ft_tokenization(attributes);
	if (!(ft_syntax_check(attributes)))
		return (0);
	ft_expand(attributes);
	ft_parsing(attributes);
	return (1);
}

void	minishell_init(char **envp, t_mini *attributes)
{
	attributes->readret = NULL;
	attributes->envp_heap = NULL;
	attributes->envp_heap = envp_to_list(envp);
	attributes->envp_arr = envp_to_array(attributes->envp_heap);
	attributes->commands = NULL;
	attributes->input_fd = STDIN_FILENO;
	attributes->output_fd = STDOUT_FILENO;
	attributes->here_fd = -1;
	attributes->i = 0;
	attributes->exitcode = 0;
	attributes->cmd_index = 0;
	
	/*attributes->array;
	attributes->pipe_arr; // not initialized yet / should they be?
	attributes->num_pipes;
	attributes->here_fd;*/
}

void	ft_readline_loop(t_mini *attributes)
{
	while (1)
	{
		if(!(attributes->readret = readline(PROMPT)))
		{
			printf("exit\n");
			rl_clear_history();
			envp_cleanup(attributes);
			exit(0); // check that the exit number is correct for ctrl+d, also million things need to be freed here??
		}
		if (attributes->readret && attributes->readret[0] != '\0')
		{
			add_history(attributes->readret);
			printf("\nyour shitty input was %s\n", attributes->readret);
			if (ft_check_everything(attributes))
			{
				puts("exe");
				ft_execution(attributes);
			}
		}
 	}
	//clean history after breaking the loop???
	envp_cleanup(attributes);
}

int main(int argc, char **argv, char **envp)
{
	t_mini  attributes;

	(void)argv;
	if (argc > 1)
	{
		ft_putstr_fd("minishell: Too many arguments\n", 2);
		return (127);
	}
	else
	{
		minishell_init(envp, &attributes);
		ft_sigint();
		ft_readline_loop(&attributes);
	}
	return (0);
}
