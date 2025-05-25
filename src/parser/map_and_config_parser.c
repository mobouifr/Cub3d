/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_and_config_parser.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 18:36:38 by mobouifr          #+#    #+#             */
/*   Updated: 2025/05/25 23:36:01 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"

static void	validate_parts_format(char **parts, t_data *data)
{
	if (!parts || !parts[0] || !parts[1] || parts[2])
	{
		write(2, "input error\n", 12);
		ft_gc_free_all(&data->gc);
		exit(1);
	}
}

static void	handle_texture_path(char **dest, char *path, int *flag,
		t_data *data)
{
	*dest = ft_strdup(path, data);
	check_file_exists(*dest, data);
	(*flag)++;
}

static void	handle_ceiling_floor_color_code(char **parts, t_game *gamevar,
		t_data *data)
{
	if (ft_strcmp(parts[0], "F") == 0)
	{
		gamevar->floor_color_hex = parse_rgb_color(gamevar->floor_color,
				parts[1], data);
		gamevar->has_floor++;
	}
	else if (ft_strcmp(parts[0], "C") == 0)
	{
		gamevar->ceiling_color_hex = parse_rgb_color(gamevar->ceiling_color,
				parts[1], data);
		gamevar->has_ceiling++;
	}
}

void	handle_texture_and_color_input(char *line, t_game *gamevar,
		t_data *data)
{
	char	**parts;

	parts = ft_split(line, ' ', data);
	validate_parts_format(parts, data);
	if (ft_strcmp(parts[0], "NO") == 0)
		handle_texture_path(&gamevar->no_path, parts[1], &gamevar->has_no,
			data);
	else if (ft_strcmp(parts[0], "SO") == 0)
		handle_texture_path(&gamevar->so_path, parts[1], &gamevar->has_so,
			data);
	else if (ft_strcmp(parts[0], "WE") == 0)
		handle_texture_path(&gamevar->we_path, parts[1], &gamevar->has_we,
			data);
	else if (ft_strcmp(parts[0], "EA") == 0)
		handle_texture_path(&gamevar->ea_path, parts[1], &gamevar->has_ea,
			data);
	else
		handle_ceiling_floor_color_code(parts, gamevar, data);
}

void	handle_map_row_input(int fd, char *line, t_game *gamevar, t_data *data)
{
	gamevar->map_height++;
	if (!map_row_is_valid(line, gamevar, data))
	{
		close(fd);
		ft_gc_free_all(&data->gc);
		exit(1);
	}
}
