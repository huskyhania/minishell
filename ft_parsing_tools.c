/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing_tools.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 20:07:51 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/14 18:57:20 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd_table	*ft_find_last_parse(t_cmd_table *stack)
{
	if (!stack)
		return (NULL);
	while (stack->right)
		stack = stack->right;
	return (stack);
}

t_cmd_table	*ft_add_new(t_tokens *token)
{
	t_cmd_table	*new_node;

	new_node = malloc(sizeof(t_cmd_table));
	if (!new_node)
	{
		printf("Error\n");
		exit(1);
	}
	new_node->right = NULL;
	new_node->left = NULL;
	new_node->cmd_arr= NULL;
	new_node->outfile = NULL;
	new_node->infile = NULL;
	new_node->here = NULL;
	new_node->herefile = NULL;
	new_node->append = NULL;
	new_node->type_arr = NULL;
	new_node->last_outfile = 0;
	new_node->last_infile = 0;
	new_node->type = (token->type);
	return(new_node);
}

void	ft_free_cmd_array(char **arr)
{
	int i;

	if (!arr)
		return ;
	i = 0;
	while (arr && arr[i])
	{
		if (arr[i])
		{
			free(arr[i]);
			arr[i] = NULL;
		}
		i++;
	}
	if (arr)
	{
		free(arr);
		arr = NULL;
	}
}

void	ft_triple_pointer(char ***arr)
{
	int	i;

	if (!arr || !*arr)
		return ;
	i = 0;
	while ((*arr)[i])
	{
		free((*arr)[i]);
		(*arr)[i] = NULL;
		i++;
	}
	free(*arr);
	*arr = NULL;
}

int	ft_count_array(char **old_array)
{
	int	i;

	i = 0;
	if (!old_array || !old_array[i])
		return (0);
	while (old_array && old_array[i])
		i++;
	return (i);
}
	
char	**ft_add_command_array(char **old_array, char *str)
{
	char	**new_array;
	int		size;
	int		i;

	//if (str[0] == '\0')
	//	return (old_array);
	size = ft_count_array(old_array);
	new_array = malloc((size + 2) * sizeof(char *));
	if (!new_array)
	{	
		ft_free_cmd_array(old_array);
		return (NULL);
	}
	i = 0;
	while (old_array && old_array[i])
	{	
		new_array[i] = ft_strdup(old_array[i]); //aa
		i++;
	}
	new_array[i] = ft_strdup(str); //aaa
	new_array[i+1] = 0;
	free_array(old_array);
	return (new_array);
}
