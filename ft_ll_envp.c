/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ll_envp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 12:28:37 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/12/12 17:59:33 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//write list freeing for malloc fails/exiting minishell

void	free_envp_list(t_envp *envp_heap)
{
	t_envp	*temp;
	while (envp_heap)
	{
		temp = envp_heap;
		envp_heap = envp_heap->next;
		if (temp->key)
			free(temp->key);
		if (temp->value)
			free(temp->value);
		free(temp);
	}
}

t_envp	*find_last(t_envp *envp_heap)
{
	if (!envp_heap)
		return (NULL);
	while (envp_heap->next)
		envp_heap = envp_heap->next;
	return (envp_heap);
}

char	*get_key(char *s)
{
	int	i;
	char	*key;

	i = 0;
	while (s[i] != '+' && s[i] != '=' && s[i] != '\0')
		i++;
	key = malloc((i + 1) * sizeof(char));
	if (!key)
		return (NULL);
	i = 0;
	while (s[i] != '+' && s[i] != '=' && s[i] != '\0')
	{
		key[i] = s[i];
		i++;
	}
	key[i] = '\0';
	return (key);
}

char	*get_value(char *s)
{
	char	*value;
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i] && s[i] != '=')
		i++;
	if (s[i] == '=')
		i++;
	value = malloc((ft_strlen(s + i) + 1) * sizeof(char));
	if (!value)
		return (NULL);
	while (s[i] != '\0')
		value[j++] = s[i++];
	value[j] = '\0';
	return (value);
}

t_envp	*create_node(char *s, t_envp **envp_heap)
{
	t_envp	*node;
	t_envp	*last_node;

	node = malloc(sizeof(t_envp));
	if (!node)
		return (NULL);
	node->key = get_key(s);
	node->value = get_value(s);
	if (!node->key || !node->value)
	{
		printf("malloc error in evnp creation");
		if (node->key)
			free(node->key);
		if (node->value)
			free(node->value);
		free(node);
		return (NULL);
	}
	node->next = NULL;
	if (!(*envp_heap))
	{
		*envp_heap = node;
		node->prev = NULL;
	}
	else
	{
		last_node = find_last(*envp_heap);
		last_node->next = node;
		node->prev = last_node;
	}
	return (node);
}

void	print_envp_list(t_envp *envp_heap)
{
	t_envp	*helper;
	helper = envp_heap;
	while (helper)
	{
		if (helper->value && helper->value[0] != '\0')
			printf("%s=%s\n", helper->key, helper->value);
		helper = helper->next;
	}
}

char	**envp_to_array(t_envp *envp_heap)
{
	int	len;
	len = 0;
	t_envp	*current;
	current = envp_heap;
	int key_value_len;
	while (current)
	{
		len++;
		current = current->next;
	}
	char	**envp_array = malloc((len + 1) * sizeof(char *));
	if (!envp_array)
		return (NULL);
	current = envp_heap;
	int i = 0;
	while (i < len)
	{
		key_value_len = ft_strlen(current->key) + ft_strlen(current->value) + 2;
		envp_array[i] = malloc(key_value_len);
		if (!envp_array[i])
		{
			//free array up to that point
			return (NULL);
		}
		ft_strlcpy(envp_array[i], current->key, ft_strlen(current->key) + 1);
		ft_strlcat(envp_array[i], "=", key_value_len);
		ft_strlcat(envp_array[i], current->value, key_value_len);
		current = current->next;
		i++;
	}
	envp_array[len] = NULL;
	return (envp_array);
}
t_envp	*envp_to_list(char **envp)
{
	t_envp	*envp_heap;
	int	i;

	envp_heap = NULL;
	i = 0;
	while (envp[i] != NULL)
	{
		if (!create_node(envp[i], &envp_heap))
		{
			printf("error creating node %d of envp", i);
			free_envp_list(envp_heap);
			return (NULL);
		}
		i++;
	}
	//print_envp_list(envp_heap);
	return (envp_heap);
}
