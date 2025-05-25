/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamir <mamir@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 18:46:12 by mamir             #+#    #+#             */
/*   Updated: 2025/05/25 14:18:55 by mamir            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_sky(t_data *data)
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

void	draw_floor(t_data *data)
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

void	draw(t_data *data)
{
	clear_image(data);
	draw_sky(data);
	draw_floor(data);
	cast_rays(data);
	mlx_put_image_to_window(data->mlx->mlx, data->mlx->win, data->mlx->img, 0,
		0);
}
