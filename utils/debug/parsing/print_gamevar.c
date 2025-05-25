/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_gamevar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 15:39:45 by mobouifr          #+#    #+#             */
/*   Updated: 2025/05/25 21:58:01 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	print_flags(const t_game *g)
{
	printf("has_player_dir        : [%d]\n", g->has_player_dir);
	printf("has_no                : [%d]\n", g->has_no);
	printf("has_so                : [%d]\n", g->has_so);
	printf("has_we                : [%d]\n", g->has_we);
	printf("has_ea                : [%d]\n", g->has_ea);
	printf("has_floor             : [%d]\n", g->has_floor);
	printf("has_ceiling           : [%d]\n", g->has_ceiling);
}

void	print_gamevar(const t_game *g)
{
	printf("=== t_game contents ===\n");
	printf("mapfile_path          : [%s]\n", g->mapfile_path);
	printf("no_path               : [%s]\n", g->no_path);
	printf("so_path               : [%s]\n", g->so_path);
	printf("we_path               : [%s]\n", g->we_path);
	printf("ea_path               : [%s]\n", g->ea_path);
	printf("floor_color           : [%d,%d,%d]\n", g->floor_color[0],
		g->floor_color[1], g->floor_color[2]);
	printf("ceiling_color         : [%d,%d,%d]\n", g->ceiling_color[0],
		g->ceiling_color[1], g->ceiling_color[2]);
	printf("floor_color_hex       : [%d]\n", g->floor_color_hex);
	printf("ceiling_color_hex     : [%d]\n", g->ceiling_color_hex);
	printf("map                   : [%p]\n", (void *)g->map);
	printf("map_width             : [%d]\n", g->map_width);
	printf("map_height            : [%d]\n", g->map_height);
	printf("player_x              : [%f]\n", g->player_x);
	printf("player_y              : [%f]\n", g->player_y);
	print_flags(g);
	printf("========================\n");
}
