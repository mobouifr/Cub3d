/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:00:46 by mobouifr          #+#    #+#             */
/*   Updated: 2025/05/25 16:02:56 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"

void	valid_extention_check(t_game *game, t_data *data)
{
	int		i;
	int		j;
	char	*str;

	i = ft_strlen(game->mapfile_path);
	j = 4;
	str = ".cub";
	if (game->mapfile_path[i - 5] == '/' || i <= j)
	{
		write(2, "Error Wrong file extension\n", 27);
		ft_gc_free_all(&data->gc);
		exit(1);
	}
	while (j >= 0)
	{
		if (game->mapfile_path[i] != str[j])
		{
			write(2, "Error Wrong file extension\n", 27);
			ft_gc_free_all(&data->gc);
			exit(1);
		}
		i--;
		j--;
	}
}

int	parse_rgb_color(int *color_code, char *str, t_data *data)
{
	char	**rgb;
	int		i;

	i = 0;
	rgb = ft_split(str, ',', data);
	if (!rgb || !rgb[0] || !rgb[1] || !rgb[2])
	{
		write(2, "rgb error\n", 10);
		ft_gc_free_all(&data->gc);
		exit(1);
	}
	while (i < 3)
	{
		color_code[i] = ft_atoi(rgb[i]);
		if (color_code[i] < 0 || color_code[i] > 255)
		{
			write(2, "rgb range error\n", 16);
			ft_gc_free_all(&data->gc);
			exit(1);
		}
		i++;
	}
	return ((color_code[0] * 256 * 256) + (color_code[1] * 256)
		+ color_code[2]);
}

int	has_direction(t_game *gamevar)
{
	if (gamevar->has_no >= 2 || gamevar->has_so >= 2 || gamevar->has_we >= 2
		|| gamevar->has_ea >= 2 || gamevar->has_floor >= 2
		|| gamevar->has_ceiling >= 2)
	{
		write(2, "direction error\n", 16);
		return (0);
	}
	if (gamevar->has_no == 1 && gamevar->has_so == 1 && gamevar->has_we == 1
		&& gamevar->has_ea == 1 && gamevar->has_floor == 1
		&& gamevar->has_ceiling == 1)
		gamevar->state = PARSE_MAP_STATE;
	return (1);
}
