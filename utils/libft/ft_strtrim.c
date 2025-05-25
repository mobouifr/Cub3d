/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 22:52:31 by mobouifr          #+#    #+#             */
/*   Updated: 2025/05/25 14:15:29 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "cub3d.h"

char	*ft_strtrim(char const *s1, char const *set, t_data *data)
{
	size_t	i;
	size_t	j;
	size_t	len;
	char	*s2;

	if (s1 == NULL || set == NULL)
		return (NULL);
	i = 0;
	len = ft_strlen_v2((char *)s1);
	while (s1[i] != '\0' && s1[i] != '\n' && ft_strchr(set, s1[i]))
		i++;
	while (len > i && s1[len - 1] && ft_strchr(set, s1[len - 1]))
		len--;
	if (len == i)
		return (ft_strdup("", data));
	s2 = (char *)ft_gc_malloc(&data->gc, (len - i + 1));
	if (s2 == NULL)
		return (NULL);
	j = 0;
	while (i < len)
		s2[j++] = s1[i++];
	s2[j] = '\0';
	return (s2);
}
/*
#include <stdio.h>

int	main(void)
{
	char const	s1[] = "gvigeeeeeeeeeeegigive";

	printf("%s", ft_strtrim(s1, "give"));
	return (0);
}*/