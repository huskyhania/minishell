/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 17:31:31 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/05 23:29:07 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//CTRL + C – SIGINT /* Interrupt the process */
//CTRL + D – SIGINT / GIves EOF 
//CTRL + \ - SIGQUIT
//SIGCHLD is the number of the signal sent to the parent process when the child terminates.

#include "minishell.h"

void ft_handle_sigint(int sig)
{
	(void)sig;
	//if (isatty(STDIN_FILENO))
	//		printf("exit\n");
	printf("\n");
	rl_on_new_line();
	//rl_replace_line("", 0); // do you really need this?
	rl_redisplay();
}

void ft_sigint(void)
{
	signal(SIGINT, ft_handle_sigint);
	//signal(SIGQUIT, SIG_IGN); // SIG_IGN ignores the signal
}

void ft_pop(int sig)
{
	if (sig == SIGINT)
	{
		printf("HERE\n");
		close(STDIN_FILENO);
		g_signal = SIGINT;
	}
}

void ft_heresignal(void)
{
	signal(SIGINT, ft_pop);
}

//SIGCHLD      P1990      Ign     Child stopped or terminated
 //SIGCLD         -        Ign     A synonym for SIGCHLD
/*void sigchld_handler(int sig) //for handling the SIGCHLD signal, which is sent when a child process terminates.
{
}*/

//However, it is true to say that Ctrl+D signals an End of Transmission (EOT) event which will generally cause a program reading input to close the input file descriptor.

//Ctrl+D is used for this because its place on the ASCII table corresponds to the analogous End of File control character, even though the actual EOF control character is not actually transmitted in this case.


