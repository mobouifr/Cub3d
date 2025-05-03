/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 11:35:29 by mobouifr          #+#    #+#             */
/*   Updated: 2025/05/03 09:47:51 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*line_catcher(int fd, char *buffer1)
{
	char	*temp_buffer;
	ssize_t	count;

	count = 1;
	temp_buffer = malloc((size_t)BUFFER_SIZE + 1);
	if (!temp_buffer)
		return (NULL);
	while (count > 0)
	{
		count = read(fd, temp_buffer, BUFFER_SIZE);
		if (count < 0)
		{
			free(temp_buffer);
			return (NULL);
		}
		if (count == 0)
			break ;
		temp_buffer[count] = '\0';
		buffer1 = ft_strjoin(buffer1, temp_buffer);
		if (ft_strchr(buffer1, '\n'))
			break ;
	}
	free(temp_buffer);
	return (buffer1);
}

char	*get_new_line(char *buffer)
{
	size_t		i;
	char		*buff;

	i = 0;
	if (buffer[0] == '\0')
		return (NULL);
	while (buffer[i] != '\n' && buffer[i] != '\0')
		i++;
	if (buffer[i] == '\n')
		i++;
	buff = malloc(i + 1);
	if (!buff)
		return (NULL);
	i = 0;
	while (buffer[i] != '\n' && buffer[i] != '\0')
	{
		buff[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\n')
		buff[i++] = '\n';
	buff[i] = '\0';
	return (buff);
}

char	*reset_buffer(char *buffer)
{
	size_t		i;
	size_t		j;
	char		*str;

	i = 0;
	j = 0;
	while (buffer[i] != '\n' && buffer[i] != '\0')
		i++;
	if (buffer[i] == '\0')
	{
		free(buffer);
		return (NULL);
	}
	if (buffer[i] == '\n')
		i++;
	str = malloc(ft_strlen(buffer) - i + 1);
	if (!str)
		return (NULL);
	while (buffer[i] != '\0')
		str[j++] = buffer[i++];
	str[j] = '\0';
	free(buffer);
	return (str);
}

char	*get_next_line(int fd)
{
	char		*buff;
	char		*string;
	static char	*buffer;

	if (fd < 0 || BUFFER_SIZE < 1)
	{
		return (NULL);
	}
	string = line_catcher(fd, buffer);
	if (string == NULL)
	{
		free(buffer);
		buffer = NULL;
		return (NULL);
	}
	buff = get_new_line(string);
	buffer = reset_buffer(string);
	return (buff);
}
