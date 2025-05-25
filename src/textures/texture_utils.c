/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 23:49:06 by mobouifr          #+#    #+#             */
/*   Updated: 2025/05/25 23:52:15 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <math.h>

int	get_texture_index(double ray_dir_x, double ray_dir_y, int side_hit)
{
	if (side_hit == 0)
	{
		if (ray_dir_x > 0)
			return (2);
		else
			return (3);
	}
	else
	{
		if (ray_dir_y > 0)
			return (1);
		else
			return (0);
	}
}

unsigned int	get_tex_color(t_texture *tex, int x, int y)
{
	char	*dst;

	dst = tex->addr + (y * tex->line_len + x * (tex->bpp / 8));
	return (*(unsigned int *)dst);
}

t_texture	*load_texture(t_data *data, char *path)
{
	t_texture	*tex;

	tex = ft_gc_malloc(&data->gc, sizeof(t_texture));
	if (!tex)
		return (NULL);
	tex->img = mlx_xpm_file_to_image(data->mlx->mlx, path, &tex->width,
			&tex->height);
	if (!tex->img)
		return (NULL);
	tex->addr = mlx_get_data_addr(tex->img, &tex->bpp, &tex->line_len,
			&tex->endian);
	return (tex);
}

void	safe_load_texture(t_data *data, t_texture **dest, char *path)
{
	*dest = load_texture(data, path);
	if (*dest == NULL)
	{
		printf("Failed to load texture %s\n", path);
		close_window(data);
		exit(1);
	}
}
