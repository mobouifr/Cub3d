/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_helper_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamir <mamir@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 14:21:15 by mamir             #+#    #+#             */
/*   Updated: 2025/05/25 14:21:16 by mamir            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_draw_vars(t_draw_vars *vars, int win_h, double dist)
{
	vars->line_height = (int)(win_h / dist);
	vars->unclipped_draw_start = -vars->line_height / 2 + win_h / 2;
	vars->draw_start = vars->unclipped_draw_start;
	if (vars->draw_start < 0)
		vars->draw_start = 0;
	vars->draw_end = vars->line_height / 2 + win_h / 2;
	if (vars->draw_end >= win_h)
		vars->draw_end = win_h - 1;
}

void	init_ray_vars(double ray_angle, t_data *data, t_dda *dda)
{
	dda->dir_x = cos(ray_angle);
	dda->dir_y = sin(ray_angle);
	dda->map_x = (int)data->player->player_x;
	dda->map_y = (int)data->player->player_y;
}

void	init_step_dist_x(t_data *data, t_dda *dda)
{
	if (dda->dir_x < 0)
	{
		dda->step_x = -1;
		dda->side_x = (data->player->player_x - dda->map_x) * dda->delta_x;
	}
	else
	{
		dda->step_x = 1;
		dda->side_x = (dda->map_x + 1.0 - data->player->player_x)
			* dda->delta_x;
	}
}

void	init_step_dist_y(t_data *data, t_dda *dda)
{
	if (dda->dir_y < 0)
	{
		dda->step_y = -1;
		dda->side_y = (data->player->player_y - dda->map_y) * dda->delta_y;
	}
	else
	{
		dda->step_y = 1;
		dda->side_y = (dda->map_y + 1.0 - data->player->player_y)
			* dda->delta_y;
	}
}

int	dda_loop(t_data *data, t_dda *dda)
{
	while (1)
	{
		if (dda->side_x < dda->side_y)
		{
			dda->side_x += dda->delta_x;
			dda->map_x += dda->step_x;
			dda->side_hit = 0;
		}
		else
		{
			dda->side_y += dda->delta_y;
			dda->map_y += dda->step_y;
			dda->side_hit = 1;
		}
		if (dda->map_x < 0 || dda->map_x >= data->map->cols || dda->map_y < 0
			|| dda->map_y >= data->map->rows)
			return (0);
		if (data->map->map[dda->map_y][dda->map_x] == '1')
			return (1);
	}
}
