/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing_tools.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:06:34 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/03 18:23:53 by llaakson         ###   ########.fr       */
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

t_cmd_table *ft_add_new(t_mini *attributes, t_tokens *token)
{
	t_cmd_table *new_node;

	(void)attributes;
	new_node = malloc(sizeof(t_cmd_table));
	if (!new_node)
	{
		printf("Error\n");
		exit(1);
	}
	//if (token->type == t_command)
	//new_node->str = ft_strdup(token->str);
	new_node->right = NULL;
	new_node->left = NULL;
	new_node->cmd_arr= NULL;
	new_node->outfile = NULL;
	new_node->infile = NULL;
	new_node->here = NULL;
	new_node->append = NULL;
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

int	ft_count_array(char **old_array)
{
	int i;

	i = 0;
	if (!old_array || !old_array[i])
		return (0);
	while (old_array && old_array[i])
		i++;
	return (i);
}
	
char **ft_add_command_array(char **old_array, char *str)
{
	char **new_array;
	int size;
	int i;
	
	size = ft_count_array(old_array);
	//printf("Counting done size %d\n",size);
	new_array = malloc((size + 2) * sizeof(char *));
	if (!new_array)
		printf("no new array");//add malloc check
	printf("Malloc done\n");
	i = 0;
	while (old_array && old_array[i])
	{
		new_array[i] = ft_strdup(old_array[i]);
		i++;
	}
	printf("array done\n");
	new_array[i] = ft_strdup(str);
	new_array[i+1] = NULL;
	//free_array(old_array);
	return (new_array);
}
