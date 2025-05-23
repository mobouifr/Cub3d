/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:45:23 by mamir             #+#    #+#             */
/*   Updated: 2025/05/23 16:47:34 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	*ft_gc_malloc(t_gc *gc, size_t size)
{
	void		*mem;
	t_gc_node	*node;

	mem = malloc(size);
	if (!mem)
		return (NULL);
	node = malloc(sizeof(t_gc_node));
	if (!node)
	{
		free(mem);
		return (NULL);
	}
	node->ptr = mem;
	node->next = gc->head;
	gc->head = node;
	return (mem);
}

void	ft_gc_free_all(t_gc *gc)
{
	t_gc_node	*node;
	t_gc_node	*next;

	node = gc->head;
	while (node)
	{
		next = node->next;
		free(node->ptr);
		free(node);
		node = next;
	}
	gc->head = NULL;
}
