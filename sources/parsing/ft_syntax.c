/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 14:59:32 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/22 19:07:16 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_check_quotes(t_mini *attributes)
{
	int		i;
	char	c;
	int		ret;

	ret = 1;
	i = 0;
	while (attributes->readret[i] != '\0')
	{
		if (ret && (attributes->readret[i] == '\''
				|| attributes->readret[i] == '"'))
		{
			c = attributes->readret[i];
			ret = 0;
		}
		else if (!ret && attributes->readret[i] == c)
			ret = 1;
		i++;
	}
	if (!ret)
	{
		attributes->exitcode = 2;
		ft_putstr_fd("Syntax error unclosed quotes\n", 2);
	}
	return (ret);
}

int	ft_check_redirection(t_tokens *token, t_mini *attributes)
{
	if (token->next == NULL)
	{
		ft_putstr_fd(" syntax error near unexpected token `newline'\n", 2);
		attributes->exitcode = 2;
		return (0);
	}
	if ((token->type >= t_great && token->type <= t_lessless)
		&& token->next->type != t_command)
	{
		if (token->next->type == t_great)
			ft_putstr_fd(" syntax error near unexpected token `>'\n", 2);
		else if (token->next->type == t_greatgreat)
			ft_putstr_fd(" syntax error near unexpected token `>>'\n", 2);
		else if (token->next->type == t_less)
			ft_putstr_fd(" syntax error near unexpected token `<'\n", 2);
		else if (token->next->type == t_lessless)
			ft_putstr_fd(" syntax error near unexpected token `<'\n", 2);
		else if (token->next->type == t_pipe || token->next->type == t_quote)
			return (1);
		attributes->exitcode = 2;
		return (0);
	}
	return (1);
}

int	ft_check_pipe(t_tokens *token, t_mini *attributes)
{
	if (token->next == NULL || token->prev == NULL
		|| token->next->type == t_pipe || token->prev->type == t_less
		|| token->prev->type == t_lessless || token->prev->type == t_greatgreat)
	{
		ft_putstr_fd(" syntax error near unexpected token `|'\n", 2);
		attributes->exitcode = 2;
		return (0);
	}
	if (token->prev->type == t_great)
	{
		ft_putstr_fd(" syntax error near unexpected token `|'\n", 2);
		attributes->exitcode = 2;
		return (0);
	}
	return (1);
}

int	ft_syntax_check(t_mini *attributes)
{
	t_tokens	*token;

	token = attributes->tokens;
	while (token)
	{
		if (token->type == t_pipe)
		{
			if (!(ft_check_pipe(token, attributes)))
				return (0);
		}
		else if (token->type >= t_great && token->type <= t_lessless)
		{
			if (!(ft_check_redirection(token, attributes)))
				return (0);
		}
		token = token->next;
	}
	return (1);
}
