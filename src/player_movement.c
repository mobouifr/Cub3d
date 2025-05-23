/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamir <mamir@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:06:57 by mamir             #+#    #+#             */
/*   Updated: 2025/05/23 11:11:26 by mamir            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	move_player(t_data *data)
{
	double	move_step;
	double	cos_angle;
	double	sin_angle;
	double	new_x;
	double	new_y;

	move_step = data->player->walk_dir * data->player->move_speed;
	cos_angle = cos(data->player->rot_angle);
	sin_angle = sin(data->player->rot_angle);
	new_x = data->player->player_x + cos_angle * move_step;
	new_y = data->player->player_y + sin_angle * move_step;
	if (is_walkable(data, new_x, data->player->player_y))
		data->player->player_x = new_x;
	if (is_walkable(data, data->player->player_x, new_y))
		data->player->player_y = new_y;
}

void	strafe_player(t_data *data, int direction)
{
	double	move_step;
	double	cos_angle;
	double	sin_angle;
	double	new_x;
	double	new_y;

	move_step = direction * data->player->move_speed;
	cos_angle = cos(data->player->rot_angle + M_PI_2);
	sin_angle = sin(data->player->rot_angle + M_PI_2);
	new_x = data->player->player_x + cos_angle * move_step;
	new_y = data->player->player_y + sin_angle * move_step;
	if (is_walkable(data, new_x, data->player->player_y))
		data->player->player_x = new_x;
	if (is_walkable(data, data->player->player_x, new_y))
		data->player->player_y = new_y;
}

void	rotate_player(t_data *data, int direction)
{
	data->player->rot_angle += (direction * data->player->rot_speed);
	if (data->player->rot_angle < 0)
		data->player->rot_angle += 2 * M_PI;
	if (data->player->rot_angle > 2 * M_PI)
		data->player->rot_angle -= 2 * M_PI;
}

static int	is_wall(t_data *data, double x, double y)
{
	int	mx;
	int	my;

	mx = (int)x;
	my = (int)y;
	if (mx < 0 || my < 0 || mx >= data->map->cols || my >= data->map->rows)
		return (1);
	return (data->map->map[my][mx] == '1');
}

int	is_walkable(t_data *data, double new_x, double new_y)
{
	double	b;

	b = 0.2;
	if (is_wall(data, new_x - b, new_y - b))
		return (0);
	if (is_wall(data, new_x + b, new_y - b))
		return (0);
	if (is_wall(data, new_x - b, new_y + b))
		return (0);
	if (is_wall(data, new_x + b, new_y + b))
		return (0);
	return (1);
}
