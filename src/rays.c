/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rays.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamir <mamir@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 14:21:11 by mamir             #+#    #+#             */
/*   Updated: 2025/05/25 14:21:12 by mamir            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

double	cast_ray(t_data *data, double ray_angle, int *side_hit)
{
	t_dda	dda;
	int		hit;
	double	dist;

	ray_angle = fmod(ray_angle, 2 * M_PI);
	if (ray_angle < 0)
		ray_angle += 2 * M_PI;
	init_ray_vars(ray_angle, data, &dda);
	dda.delta_x = fabs(1 / dda.dir_x);
	dda.delta_y = fabs(1 / dda.dir_y);
	init_step_dist_x(data, &dda);
	init_step_dist_y(data, &dda);
	hit = dda_loop(data, &dda);
	*side_hit = dda.side_hit;
	if (dda.side_hit == 0)
		dist = (dda.map_x - data->player->player_x + (1 - dda.step_x) / 2)
			/ dda.dir_x;
	else
		dist = (dda.map_y - data->player->player_y + (1 - dda.step_y) / 2)
			/ dda.dir_y;
	return (dist);
}

void	draw_column(t_data *data, int i, double ray_angle)
{
	int			side_hit;
	double		perp_wall_dist;
	double		corrected_dist;
	double		wall_x;
	t_draw_vars	vars;

	perp_wall_dist = cast_ray(data, ray_angle, &side_hit);
	corrected_dist = perp_wall_dist * cos(ray_angle - data->player->rot_angle);
	init_draw_vars(&vars, data->mlx->win_height, corrected_dist);
	wall_x = get_wall_x(data, perp_wall_dist, ray_angle, side_hit);
	vars.tex_x = get_tex_x_for_column(get_tex_for_column(data,
				get_tex_index_for_column(ray_angle, side_hit)), wall_x,
			side_hit, ray_angle);
	draw_wall_slice(data, get_tex_for_column(data,
			get_tex_index_for_column(ray_angle, side_hit)), i, &vars);
}

void	cast_rays(t_data *data)
{
	int		i;
	double	fov;
	double	angle_step;
	double	start_angle;
	double	ray_angle;

	fov = 60 * (M_PI / 180);
	angle_step = fov / data->mlx->win_width;
	start_angle = data->player->rot_angle - (fov / 2);
	i = 0;
	while (i < data->mlx->win_width)
	{
		ray_angle = start_angle + (i * angle_step);
		draw_column(data, i, ray_angle);
		i++;
	}
}
