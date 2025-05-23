/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2d_map.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamir <mamir@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:46:02 by mamir             #+#    #+#             */
/*   Updated: 2025/05/23 11:47:35 by mamir            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
void draw_map_2d(t_data *data, char **map, int rows, int cols)
{
    int i, j, x, y;
    int tile_x, tile_y;
    int color;

    i = 0;
    while (i < rows)
    {
        j = 0;
        while (j < cols)
        {
            tile_x = j * TILE_SIZE;
            tile_y = i * TILE_SIZE;
            if (map[i][j] == '0')
                color = 0xFFFFFF; // White for empty spaces
            else if (map[i][j] == '1')
                color = 0x000000; // Black for walls
            y = 0;
            while (y < TILE_SIZE)
            {
                x = 0;
                while (x < TILE_SIZE)
                {
                    // Draw the grid lines
                    if (x == 0 || y == 0)
                        my_mlx_pixel_put(data, tile_x + x, tile_y + y, 0x808080);
                    else
                        my_mlx_pixel_put(data, tile_x + x, tile_y + y, color);

                    x++;
                }
                y++;
            }
            j++;
        }
        i++;
    }
}

void draw_line(t_data *data, int x1, int y1, int x2, int y2, int color)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1)
    {
        my_mlx_pixel_put(data, x1, y1, color);
        if (x1 == x2 && y1 == y2)
            break;
        int e2 = err * 2;
        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

void draw_player_facing_line(t_data *data)
{
    double angle = data->player->rot_angle;
    int line_length = 50;
    int line_end_x = data->player->player_x 
                    * TILE_SIZE + cos(angle) * line_length;
    int line_end_y = data->player->player_y 
                    * TILE_SIZE + sin(angle) * line_length;
    int x1 = data->player->player_x * TILE_SIZE;
    int y1 = data->player->player_y * TILE_SIZE;    
    draw_line(data, x1, y1, line_end_x, line_end_y, 0xFF0000);  
}

void draw_player(t_data *data)
{
    if (!data || !data->player || !data->map)
    {
        printf("Error: Null pointer in draw_player function.\n");
        return;
    }
    int player_x = data->player->player_x * TILE_SIZE;
    int player_y = data->player->player_y * TILE_SIZE;
    int square_size = 10;

    int py = -square_size / 2;
    while (py <= square_size / 2)
    {
        int px = -square_size / 2;
        while (px <= square_size / 2)
        {
            my_mlx_pixel_put(data, player_x + px, player_y + py, 0xFF0000);
            px++;
        }
        py++;
    }
}
**/