/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamir <mamir@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 18:46:12 by mamir             #+#    #+#             */
/*   Updated: 2025/05/24 18:56:48 by mamir            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	init_ray_vars(double ray_angle, t_data *data, t_dda *dda)
{
	dda->dir_x = cos(ray_angle);
	dda->dir_y = sin(ray_angle);
	dda->map_x = (int)data->player->player_x;
	dda->map_y = (int)data->player->player_y;
}

static void	init_step_dist_x(t_data *data, t_dda *dda)
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

static void	init_step_dist_y(t_data *data, t_dda *dda)
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

static int	dda_loop(t_data *data, t_dda *dda)
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

static void	draw_sky(t_data *data)
{
	int	y;
	int	x;
	int	color;

	color = data->colors->ceiling_color_hex;
	y = 0;
	while (y < data->mlx->win_height / 2)
	{
		x = 0;
		while (x < data->mlx->win_width)
		{
			my_mlx_pixel_put(data, x, y, color);
			x++;
		}
		y++;
	}
}

static void	draw_floor(t_data *data)
{
	int	y;
	int	x;
	int	color;

	color = data->colors->floor_color_hex;
	y = data->mlx->win_height / 2;
	while (y < data->mlx->win_height)
	{
		x = 0;
		while (x < data->mlx->win_width)
		{
			my_mlx_pixel_put(data, x, y, color);
			x++;
		}
		y++;
	}
}

void	clear_image(t_data *data)
{
	int	y;
	int	x;

	y = 0;
	while (y < data->mlx->win_height)
	{
		x = 0;
		while (x < data->mlx->win_width)
		{
			my_mlx_pixel_put(data, x, y, 0x000000);
			x++;
		}
		y++;
	}
}

static void	init_draw_vars(t_draw_vars *vars, int win_h, double dist)
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

static double	get_wall_x(t_data *data, double dist, double ray_angle,
		int side)
{
	double	wall_x;

	if (side == 0)
		wall_x = data->player->player_y + dist * sin(ray_angle);
	else
		wall_x = data->player->player_x + dist * cos(ray_angle);
	return (wall_x - floor(wall_x));
}

void	draw_wall_slice(t_data *data, t_texture *tex, int i, t_draw_vars *vars)
{
	int				y;
	int				tex_y;
	unsigned int	color;

	y = vars->draw_start;
	while (y < vars->draw_end)
	{
		tex_y = (int)(((double)(y - vars->unclipped_draw_start)
					/ vars->line_height) * tex->height);
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= tex->height)
			tex_y = tex->height - 1;
		color = get_tex_color(tex, vars->tex_x, tex_y);
		my_mlx_pixel_put(data, i, y, color);
		y++;
	}
}

static int	get_tex_index_for_column(double ray_angle, int side_hit)
{
	return (get_texture_index(cos(ray_angle), sin(ray_angle), side_hit));
}

static t_texture	*get_tex_for_column(t_data *data, int tex_index)
{
	return (data->textures[tex_index]);
}

static int	get_tex_x_for_column(t_texture *tex, double wall_x, int side_hit,
		double ray_angle)
{
	int	tex_x;

	tex_x = (int)(wall_x * tex->width);
	if ((side_hit == 0 && cos(ray_angle) > 0) || (side_hit == 1
			&& sin(ray_angle) < 0))
		tex_x = tex->width - tex_x - 1;
	return (tex_x);
}

static void	draw_column(t_data *data, int i, double ray_angle)
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

void	draw(t_data *data)
{
	clear_image(data);
	draw_sky(data);
	draw_floor(data);
	cast_rays(data);
	mlx_put_image_to_window(data->mlx->mlx, data->mlx->win, data->mlx->img, 0,
		0);
}
