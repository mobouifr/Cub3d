/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbook <macbook@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:48:37 by mamir             #+#    #+#             */
/*   Updated: 2025/05/24 13:58:33 by macbook          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	mlx_start(t_data *data)
{
	data->mlx->win_width = 1280;
	data->mlx->win_height = 600;
	data->mlx->mlx = mlx_init();
	data->mlx->win = mlx_new_window(data->mlx->mlx, data->mlx->win_width,
			data->mlx->win_height, "cub3D");
	data->mlx->img = mlx_new_image(data->mlx->mlx, data->mlx->win_width,
			data->mlx->win_height);
	data->mlx->addr = mlx_get_data_addr(data->mlx->img, &data->mlx->bpp,
			&data->mlx->line_length, &data->mlx->endian);
}

int	close_window(t_data *data)
{
	mlx_destroy_image(data->mlx->mlx, data->textures[0]->img);
	mlx_destroy_image(data->mlx->mlx, data->textures[1]->img);
	mlx_destroy_image(data->mlx->mlx, data->textures[2]->img);
	mlx_destroy_image(data->mlx->mlx, data->textures[3]->img);
	mlx_destroy_image(data->mlx->mlx, data->mlx->img);
	mlx_clear_window(data->mlx->mlx, data->mlx->win);
	mlx_destroy_window(data->mlx->mlx, data->mlx->win);
	mlx_destroy_display(data->mlx->mlx);
	if (data->mlx->mlx)
		free(data->mlx->mlx);
	printf("Exit Game!\n");
	ft_gc_free_all(&data->gc);
	exit(0);
}

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->mlx->addr + (y * data->mlx->line_length + x * (data->mlx->bpp
				/ 8));
	*(unsigned int *)dst = color;
}
