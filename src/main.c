/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamir <mamir@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:52:35 by mamir             #+#    #+#             */
/*   Updated: 2025/05/26 13:33:56 by mamir            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <math.h>

int	game_loop(t_data *data)
{
	if (data->player->walk_dir != 0)
		move_player(data);
	if (data->player->strafe_dir != 0)
		strafe_player(data, data->player->strafe_dir);
	if (data->player->turn_dir != 0)
		rotate_player(data, data->player->turn_dir);
	draw(data);
	return (0);
}

void	start_game(t_data *data)
{
	mlx_hook(data->mlx->win, 2, 1L << 0, handle_keypress, data);
	mlx_hook(data->mlx->win, 3, 1L << 1, key_released, data);
	mlx_hook(data->mlx->win, 17, 0, close_window, data);
	mlx_loop_hook(data->mlx->mlx, game_loop, data);
	mlx_loop(data->mlx->mlx);
}

double	dir_to_angle(char c)
{
	if (c == 'N')
		return (3 * M_PI / 2);
	if (c == 'S')
		return (M_PI / 2);
	if (c == 'E')
		return (0);
	if (c == 'W')
		return (M_PI);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;
	t_game	*parsed;

	initialize_data(&data);
	parsed = parser(argc, argv, &data);
	data.map->map = parsed->map;
	data.map->rows = parsed->map_height;
	data.map->cols = parsed->map_width;
	data.player->player_x = parsed->player_x + 0.5;
	data.player->player_y = parsed->player_y + 0.5;
	data.player->rot_angle = dir_to_angle(parsed->player_dir);
	data.colors->ceiling_color_hex = parsed->ceiling_color_hex;
	data.colors->floor_color_hex = parsed->floor_color_hex;
	mlx_start(&data);
	safe_load_texture(&data, &data.textures[0], parsed->no_path);
	safe_load_texture(&data, &data.textures[1], parsed->so_path);
	safe_load_texture(&data, &data.textures[2], parsed->ea_path);
	safe_load_texture(&data, &data.textures[3], parsed->we_path);
	start_game(&data);
	ft_gc_free_all(&data.gc);
	return (0);
}
