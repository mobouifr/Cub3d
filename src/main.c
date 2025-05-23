/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:52:35 by mamir             #+#    #+#             */
/*   Updated: 2025/05/23 15:53:37 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <math.h>

int get_texture_index(double ray_dir_x, double ray_dir_y, int side_hit)
{
    if (side_hit == 0)
    {
        if (ray_dir_x > 0)
            return 2; // East
        else
            return 3; // West
    }
    else
    {
        if (ray_dir_y > 0)
            return 1; // South
        else
            return 0; // North
    }
}

unsigned int get_tex_color(t_texture *tex, int x, int y)
{
    char *dst = tex->addr + (y * tex->line_len + x * (tex->bpp / 8));
    return *(unsigned int *)dst;
}

t_texture *load_texture(t_data *data, char *path)
{
    t_texture *tex = ft_gc_malloc(&data->gc, sizeof(t_texture));
    // t_texture *tex = malloc(sizeof(t_texture));
    if (!tex)
    {
        printf("Memory allocation failed for texture\n");
        ft_gc_free_all(&data->gc);
        exit(1);
    }
    tex->img = mlx_xpm_file_to_image(data->mlx->mlx, path, &tex->width, &tex->height);
    printf ("{%s} - %d - %d", path, tex->width, tex->height);
    if (!tex->img)
    {
        printf("Failed to load texture %s\n", path);
        ft_gc_free_all(&data->gc);
        exit(1);
    }
    tex->addr = mlx_get_data_addr(tex->img, &tex->bpp, &tex->line_len, &tex->endian);
    return tex;
}

int game_loop(t_data *data)
{
    if (data->player->walk_dir != 0)
        move_player(data);
    if (data->player->strafe_dir != 0)
        strafe_player(data, data->player->strafe_dir);
    if (data->player->turn_dir != 0)
        rotate_player(data, data->player->turn_dir);
    draw(data);
    return (0);
}

void start_game(t_data *data)
{
    mlx_hook(data->mlx->win, 2, 1L << 0, handle_keypress, data);
    mlx_hook(data->mlx->win, 3, 1L << 1, key_released, data);
    mlx_hook(data->mlx->win, 17, 0, close_window, data);     
    mlx_loop_hook(data->mlx->mlx, game_loop, data); 
    mlx_loop(data->mlx->mlx);  
}


double dir_to_angle(char c)
{
    if (c == 'N') 
        return (3 * M_PI / 2); // 270° → facing up
    if (c == 'S') 
        return (M_PI / 2);     // 90° → facing down
    if (c == 'E') 
        return (0);            // 0° → facing right
    if (c == 'W') 
        return (M_PI);         // 180° → facing left
    return 0;
}

int main(int argc, char **argv)
{
    t_data data;
    t_game *parsed;

    parsed = parser(argc, argv, &data);
    initialize_data(&data);
    print_gamevar(parsed);
    
    data.map->map = parsed->map;
    data.map->rows = parsed->map_height;
    data.map->cols = parsed->map_width;

    data.player->player_x = parsed->player_x + 0.5;
    data.player->player_y = parsed->player_y + 0.5;
    data.player->rot_angle = dir_to_angle(parsed->player_dir);
    
    data.colors->ceiling_color_hex = parsed->ceiling_color_hex;
    data.colors->floor_color_hex = parsed->floor_color_hex;
    mlx_start(&data);
    data.textures[0] = load_texture(&data, parsed->no_path);
    data.textures[1] = load_texture(&data, parsed->so_path);
    data.textures[2] = load_texture(&data, parsed->ea_path);
    data.textures[3] = load_texture(&data, parsed->we_path);
    start_game(&data);
    ft_gc_free_all(&data.gc);
    return 0;
}

