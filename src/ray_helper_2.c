/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_helper_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamir <mamir@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 14:21:13 by mamir             #+#    #+#             */
/*   Updated: 2025/05/25 14:21:14 by mamir            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

double	get_wall_x(t_data *data, double dist, double ray_angle,
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

int	get_tex_index_for_column(double ray_angle, int side_hit)
{
	return (get_texture_index(cos(ray_angle), sin(ray_angle), side_hit));
}

t_texture	*get_tex_for_column(t_data *data, int tex_index)
{
	return (data->textures[tex_index]);
}

int	get_tex_x_for_column(t_texture *tex, double wall_x, int side_hit,
		double ray_angle)
{
	int	tex_x;

	tex_x = (int)(wall_x * tex->width);
	if ((side_hit == 0 && cos(ray_angle) > 0) || (side_hit == 1
			&& sin(ray_angle) < 0))
		tex_x = tex->width - tex_x - 1;
	return (tex_x);
}
