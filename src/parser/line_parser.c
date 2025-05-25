/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 18:50:09 by mobouifr          #+#    #+#             */
/*   Updated: 2025/05/25 21:28:33 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"

static void	validate_map_and_player_presence(int fd, t_game *gamevar,
		t_data *data)
{
	if (gamevar->map_height == 0)
	{
		write(2, "Error, map not found\n", 21);
		close(fd);
		ft_gc_free_all(&data->gc);
		exit(1);
	}
	else if (gamevar->has_player_dir == 0 || gamevar->player_dir == 0)
	{
		write(2, "Error, player not found\n", 24);
		close(fd);
		ft_gc_free_all(&data->gc);
		exit(1);
	}
}

void	parse_line(int fd, t_game *gamevar, t_data *data)
{
	char	*line;

	line = get_next_line(fd, data);
	while (line != NULL)
	{
		if (line_is_empty(line))
		{
			line = get_next_line(fd, data);
			continue ;
		}
		if (!has_directions(gamevar))
		{
			ft_gc_free_all(&data->gc);
			exit(1);
		}
		if (gamevar->state == PARSE_TEXTURES_AND_COLORS_STATE)
			handle_texture_and_color_input(line, gamevar, data);
		if (gamevar->state == PARSE_MAP_STATE)
			handle_map_row_input(fd, line, gamevar, data);
		line = get_next_line(fd, data);
	}
	validate_map_and_player_presence(fd, gamevar, data);
	close(fd);
}
