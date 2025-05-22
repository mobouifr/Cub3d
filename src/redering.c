#include "cub3d.h"

void draw(t_data *data)
{
    clear_image(data); // Clear the screen

    // Draw the sky
    int sky_color = data->colors->ceiling_color_hex/*0x2E3A3F*/;  // Muted smoky blue-gray
    int y = 0;
    while (y < data->mlx->win_height / 2) // Top half of the screen
    {
        int x = 0;
        while (x < data->mlx->win_width)
        {
            my_mlx_pixel_put(data, x, y, sky_color);
            x++;
        }
        y++;
    }
    //Draw the floor
    int floor_color = data->colors->floor_color_hex/*0x1A1A18*/;  // Industrial dark gray/black
    y = data->mlx->win_height / 2;
    while (y < data->mlx->win_height) // Bottom half of the screen
    {
        int x = 0;
        while (x < data->mlx->win_width)
        {
            my_mlx_pixel_put(data, x, y, floor_color);
            x++;
        }
        y++;
    }
    cast_rays(data); // Render the 3D walls
    // draw_map(data, data->map->map, data->map->rows, data->map->cols);
    // draw_player(data);
    mlx_put_image_to_window(data->mlx->mlx, data->mlx->win, data->mlx->img, 0, 0);
}

void clear_image(t_data *data)
{
    int width = data->mlx->win_width;
    int height = data->mlx->win_height;

    int y = 0;
    while (y < height)
    {
        int x = 0;
        while (x < width)
        {
            my_mlx_pixel_put(data, x, y, 0x000000); // Clear with black color
            x++;
        }
        y++;
    }
}

void cast_rays(t_data *data)
{
    int num_rays = data->mlx->win_width; // One ray per vertical screen pixel
    double fov = 60 * (M_PI / 180);      // Field of view in radians
    double angle_step = fov / num_rays; // Angle between each ray

    // Start casting rays from the leftmost angle of the FOV
    double start_angle = data->player->rot_angle - (fov / 2);

    int i = 0;
    while (i < num_rays)
    {
        double ray_angle = start_angle + (i * angle_step);
        int side_hit;
        double perp_wall_dist = cast_ray(data, ray_angle, &side_hit);

        // Correct the perpendicular wall distance to fix the fish-eye effect
        double corrected_dist = perp_wall_dist * cos(ray_angle - data->player->rot_angle);

        // Calculate the height of the wall slice
        int line_height = (int)(data->mlx->win_height / corrected_dist);

        // Calculate the start and end points of the wall slice
        int unclipped_draw_start = -line_height / 2 + data->mlx->win_height / 2; // added this for not letting the wall distoreted when close to them.
        int draw_start = unclipped_draw_start;
        if (draw_start < 0)
            draw_start = 0;
        int draw_end = line_height / 2 + data->mlx->win_height / 2;
        if (draw_end >= data->mlx->win_height)
            draw_end = data->mlx->win_height - 1;

        // Compute wall_x: the exact point of the wall hit (fractional part)
        double wall_x;
        if (side_hit == 0)
            wall_x = data->player->player_y + perp_wall_dist * sin(ray_angle);
        else
            wall_x = data->player->player_x + perp_wall_dist * cos(ray_angle);
        wall_x -= floor(wall_x);

        // Choose texture
        int tex_index = get_texture_index(cos(ray_angle), sin(ray_angle), side_hit);
        t_texture *tex = data->textures[tex_index];

        // Compute X coordinate in texture
        int tex_x = (int)(wall_x * tex->width);
        if ((side_hit == 0 && cos(ray_angle) > 0) || (side_hit == 1 && sin(ray_angle) < 0))
            tex_x = tex->width - tex_x - 1;

        // Draw vertical textured wall slice
        int y = draw_start;
        while (y < draw_end)
        {
            int tex_y = (int)(((double)(y - unclipped_draw_start) / line_height) * tex->height);
            if (tex_y < 0)
                tex_y = 0;
            if (tex_y >= tex->height)
                tex_y = tex->height - 1;
            unsigned int color = get_tex_color(tex, tex_x, tex_y);
            my_mlx_pixel_put(data, i, y, color);
            y++;
        }
        i++;
    }
}

double cast_ray(t_data *data, double ray_angle, int *side_hit)
{
    ray_angle = fmod(ray_angle, 2 * M_PI);
    if (ray_angle < 0)
        ray_angle += 2 * M_PI;
    double ray_dir_x = cos(ray_angle);
    double ray_dir_y = sin(ray_angle);
    double ray_x = data->player->player_x;
    double ray_y = data->player->player_y;
    int map_x = (int)ray_x;
    int map_y = (int)ray_y;
    double delta_dist_x = fabs(1 / ray_dir_x);
    double delta_dist_y = fabs(1 / ray_dir_y);
    int step_x, step_y;
    double side_dist_x, side_dist_y;
    if (ray_dir_x < 0)
    {
        step_x = -1;
        side_dist_x = (ray_x - map_x) * delta_dist_x;
    }
    else
    {
        step_x = 1;
        side_dist_x = (map_x + 1.0 - ray_x) * delta_dist_x;
    }

    if (ray_dir_y < 0)
    {
        step_y = -1;
        side_dist_y = (ray_y - map_y) * delta_dist_y;
    }
    else
    {
        step_y = 1;
        side_dist_y = (map_y + 1.0 - ray_y) * delta_dist_y;
    }

    int hit = 0;
    while (!hit)
    {
        if (side_dist_x < side_dist_y)
        {
            side_dist_x += delta_dist_x;
            map_x += step_x;
            *side_hit = 0;
        }
        else
        {
            side_dist_y += delta_dist_y;
            map_y += step_y;
            *side_hit = 1;
        }
        
        if (map_x < 0 || map_x >= data->map->cols || map_y < 0 || map_y >= data->map->rows)
            break;
        if (data->map->map[map_y][map_x] == '1')
            hit = 1;
    }
    double perp_wall_dist;
    if (*side_hit == 0)
        perp_wall_dist = (map_x - ray_x + (1 - step_x) / 2) / ray_dir_x;
    else
        perp_wall_dist = (map_y - ray_y + (1 - step_y) / 2) / ray_dir_y;
    return perp_wall_dist;
}

