/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vars_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 15:34:22 by mobouifr          #+#    #+#             */
/*   Updated: 2025/05/25 15:35:37 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	init_floor_ceiling_vars(t_game *gamevar)
{
	int	i;

	i = 0;
	while (i < 3)
	{
		gamevar->floor_color[i] = -1;
		gamevar->ceiling_color[i] = -1;
		i++;
	}
	gamevar->floor_color_hex = 0;
	gamevar->ceiling_color_hex = 0;
	gamevar->has_floor = 0;
	gamevar->has_ceiling = 0;
}

static void	init_directions_vars(t_game *gamevar)
{
	gamevar->no_path = NULL;
	gamevar->so_path = NULL;
	gamevar->we_path = NULL;
	gamevar->ea_path = NULL;
	gamevar->has_no = 0;
	gamevar->has_so = 0;
	gamevar->has_we = 0;
	gamevar->has_ea = 0;
}

static void	init_player_vars(t_game *gamevar)
{
	gamevar->player_x = -1;
	gamevar->player_y = -1;
	gamevar->player_dir = '\0';
	gamevar->has_player_dir = 0;
}

static void	init_map_vars(t_game *gamevar)
{
	gamevar->map = NULL;
	gamevar->map_width = 0;
	gamevar->map_height = 0;
}

void	vars_init(t_game *gamevar)
{
	gamevar->mapfile_path = NULL;
	init_directions_vars(gamevar);
	init_floor_ceiling_vars(gamevar);
	init_map_vars(gamevar);
	init_player_vars(gamevar);
	gamevar->state = INITIAL;
}