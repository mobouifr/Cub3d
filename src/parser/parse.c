/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 15:51:25 by mobouifr          #+#    #+#             */
/*   Updated: 2025/05/25 21:25:01 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"

t_game	*parser(int argc, char **argv, t_data *data)
{
	int		fd;
	t_game	*gamevar;

	if (argc != 2)
		exit_error("Wrong number of argument", data);
	gamevar = ft_gc_malloc(&data->gc, sizeof(t_game));
	if (!gamevar)
		exit_error("allocation Error", data);
	vars_init(gamevar);
	gamevar->mapfile_path = argv[1];
	valid_extention_check(gamevar, data);
	fd = open(gamevar->mapfile_path, O_RDONLY);
	if (fd == -1)
		exit_error("Fd error", data);
	parse_line(fd, gamevar, data);
	fill_map(gamevar, data);
	return (gamevar);
}
