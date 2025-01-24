/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 13:33:13 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/24 13:33:17 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = 0;

int	ft_check_everything(t_mini *attributes)
{
	if (is_empty_or_space(attributes->readret))
		return (0);
	if (!(ft_check_quotes(attributes)))
		return (0);
	if (!(ft_tokenization(attributes)))
	{
		ft_free_tokens(attributes);
		ft_putstr_fd("Tokenization error\n", 2);
		return (update_exitcode(2, attributes), 0);
	}
	if (!(ft_syntax_check(attributes)))
	{
		ft_free_tokens(attributes);
		return (0);
	}
	if (!(ft_expand(attributes)))
	{
		ft_free_tokens(attributes);
		return (0);
	}
	if (!(ft_merge_tokens(attributes)))
		return (0);
	if (!(ft_parsing(attributes)))
		return (0);
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
	attributes->i = 0;
	attributes->exitcode = 0;
	attributes->cmd_index = 0;
	attributes->type_count = 0;
}

void	ft_readline_loop(t_mini *attributes)
{
	while (1)
	{
		attributes->readret = readline(PROMPT);
		if (!(attributes->readret))
		{
			ft_putstr_fd("exit\n", 1);
			ft_final_exit(attributes, attributes->exitcode);
		}
		if (attributes->readret && attributes->readret[0] != '\0')
		{
			add_history(attributes->readret);
			if (ft_check_everything(attributes))
			{
				if (attributes->commands)
					ft_execution(attributes);
				ft_print_signal_error(attributes->exitcode);
			}
		}
	}
}

t_mini	*ft_attributes(void)
{
	static t_mini	attributes;

	return (&attributes);
}

int	main(int argc, char **argv, char **envp)
{
	t_mini	*attributes;

	(void)argv;
	if (argc > 1)
	{
		ft_putstr_fd("minishell: Too many arguments\n", 2);
		return (127);
	}
	else
	{
		attributes = ft_attributes();
		minishell_init(envp, attributes);
		ft_sigint();
		ft_readline_loop(attributes);
	}
	return (0);
}
