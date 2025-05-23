/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 16:00:55 by mobouifr          #+#    #+#             */
/*   Updated: 2025/05/23 16:02:36 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2, t_data *data)
{
	size_t	i;
	size_t	j;
	char	*s3;

	i = 0;
	j = 0;
	if (!s1 && !s2)
		return (NULL);
	if (!s1 && s2)
		return (ft_strdup(s2, data));
	else if (!s2 && s1)
		return (ft_strdup(s1, data));
	s3 = ft_gc_malloc(&data->gc, ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!s3)
		return (NULL);
	while (s1[i] != '\0')
		s3[j++] = s1[i++];
	i = 0;
	while (s2[i] != '\0')
		s3[j++] = s2[i++];
	s3[j] = '\0';
	return (s3);
}
/*
int	main(void)
{
	const char	s1[] = "be free ";
	const char	s2[] = "as a bee";

	printf("%s", ft_strjoin(s1, s2));
	return (0);
}
*/
