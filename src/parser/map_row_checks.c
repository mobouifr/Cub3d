/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_row_checks.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 17:09:01 by mobouifr          #+#    #+#             */
/*   Updated: 2025/05/26 17:57:43 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"

static int	player_char_found(char c)
{
	return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

static void	validate_row_len(t_game *gamevar, int i)
{
	if (i > gamevar->map_width)
		gamevar->map_width++;
}

static void	set_player_direction(t_game *gamevar, char c, int i)
{
	gamevar->player_dir = c;
	gamevar->player_x = i;
	gamevar->player_y = gamevar->map_height - 1;
	gamevar->has_player_dir = 1;
}

static int	line_closed_by_walls(char *str, t_data *data)
{
	int		len;
	char	*trimmed_str;

	trimmed_str = ft_strtrim(str, " ", data);
	len = ft_strlen_v2(trimmed_str);
	if (trimmed_str[0] != '1' || trimmed_str[len - 1] != '1')
	{
		write(2, "map error(map rows should be closed by walls)\n", 46);
		return (0);
	}
	return (1);
}

int	map_row_is_valid(char *str, t_game *gamevar, t_data *data)
{
	int	i;

	i = 0;
	while (str[i] != '\0' && str[i] != '\n')
	{
		if (!line_closed_by_walls(str, data))
			return (0);
		if (player_char_found(str[i]) && gamevar->has_player_dir == 0)
		{
			set_player_direction(gamevar, str[i], i);
			i++;
			validate_row_len(gamevar, i);
		}
		else if (str[i] == ' ' || str[i] == '1' || str[i] == '0')
		{
			i++;
			validate_row_len(gamevar, i);
		}
		else
		{
			write(2, "non valid character\n", 21);
			return (0);
		}
	}
	return (1);
}
