/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:05:11 by mamir             #+#    #+#             */
/*   Updated: 2025/05/23 16:30:43 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	handle_keypress(int keycode, t_data *data)
{
	if (keycode == 'w')
		data->player->walk_dir = 1;
	else if (keycode == 's')
		data->player->walk_dir = -1;
	else if (keycode == 'a')
		data->player->strafe_dir = -1;
	else if (keycode == 'd')
		data->player->strafe_dir = 1;
	else if (keycode == 65361)
		data->player->turn_dir = -1;
	else if (keycode == 65363)
		data->player->turn_dir = 1;
	else if (keycode == 65307)
		close_window(data);
	return (0);
}

int	key_released(int keycode, t_data *data)
{
	if (keycode == 'w' || keycode == 's')
		data->player->walk_dir = 0;
	else if (keycode == 'a' || keycode == 'd')
		data->player->strafe_dir = 0;
	else if (keycode == 65361 || keycode == 65363)
		data->player->turn_dir = 0;
	return (0);
}
