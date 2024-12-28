/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 14:59:32 by llaakson          #+#    #+#             */
/*   Updated: 2024/12/29 00:08:44 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_check_quotes(char *line)
{
	int i;
	char c;
	int ret;

	ret = 1;
	i = 0;
	while (line[i] != '\0')
	{
		if (ret && (line[i] == '\'' || line[i] == '"'))
		{
			c = line[i];
			ret = 0;
		}
		else if (!ret && line[i] == c)
			ret = 1;
		i++;
	}
	if (!ret)
		printf("syntax error unclosed quotes\n");
	return (ret);
}

int ft_check_redirection(t_tokens *token, t_mini *attributes)
{
	if (token->next == NULL)
	{
		printf("syntax error near unexpected token `newline'\n"); // needs it own token type?
		attributes->exitcode = 2;
		return (0);
	}
	if ((token->type == t_great || token->type == t_greatgreat) && token->next->type != t_command)
	{
		if (token->next->type == t_great)
			printf("syntax error near unexpected token `>'\n");
		else if (token->next->type == t_greatgreat)
			printf("syntax error near unexpected token `>>'\n");
		else if (token->next->type == t_less)
			printf("syntax error near unexpected token `<'\n");
		else if (token->next->type == t_lessless)
			printf("syntax error near unexpected token `<'\n");
		attributes->exitcode = 2;
		return (0);
	}
	return (1);
}

int ft_check_pipe(t_tokens *token, t_mini *attributes)
{
	if (token->next == NULL || token->prev == NULL || token->next->type == t_pipe || token->prev->type == t_less || token->prev->type == t_lessless)
	{
		printf("syntax error near unexpected token `|'\n");
		attributes->exitcode = 2;
		ft_free_tokens(attributes);
		return (0);
	}
	return (1);
}

int ft_syntax_check(t_mini *attributes)
{
	t_tokens *token;
	
	token = attributes->tokens;
	while (token)
	{
		printf("Token can merge %d\n",token->merge);
		if (token->type == t_pipe)
			if(!(ft_check_pipe(token, attributes)))
				return (0);
		if (token->type == t_great || token->type == t_less || token->type == t_greatgreat || token->type == t_lessless)
			if(!(ft_check_redirection(token, attributes)))
				return (0);
		token = token->next;
	}
	//puts("checks done");
	return (1);
}
