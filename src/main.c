#include "cub3d.h"
#include <math.h>

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

int is_walkable(t_data *data, double new_x, double new_y)
{
    // Define the player's collision buffer size (half the player's size)
    double buffer = 0.2; // Adjust this value as needed

    // Check all four corners of the player's collision box
    int top_left_x = (int)(new_x - buffer);
    int top_left_y = (int)(new_y - buffer);
    int top_right_x = (int)(new_x + buffer);
    int top_right_y = (int)(new_y - buffer);
    int bottom_left_x = (int)(new_x - buffer);
    int bottom_left_y = (int)(new_y + buffer);
    int bottom_right_x = (int)(new_x + buffer);
    int bottom_right_y = (int)(new_y + buffer);

    // Check if any corner is in a wall
    if (top_left_x < 0 || top_left_y < 0 || top_right_x >= data->map->cols || bottom_right_y >= data->map->rows)
        return 0; // Out of bounds

    if (data->map->map[top_left_y][top_left_x] == '1' ||
        data->map->map[top_right_y][top_right_x] == '1' ||
        data->map->map[bottom_left_y][bottom_left_x] == '1' ||
        data->map->map[bottom_right_y][bottom_right_x] == '1')
        return 0; // Collision with a wall

    return 1; // No collision
}

void move_player(t_data *data)
{
    double move_step = data->player->walk_dir * data->player->move_speed;
    double cos_angle = cos(data->player->rot_angle);
    double sin_angle = sin(data->player->rot_angle);

    double new_x = data->player->player_x + cos_angle * move_step;
    double new_y = data->player->player_y + sin_angle * move_step;

    if (is_walkable(data, new_x, new_y))
    {
        data->player->player_x = new_x;
        data->player->player_y = new_y;
    }
}

void strafe_player(t_data *data, int direction)
{
    double move_step = direction * data->player->move_speed * 20;
    double cos_angle = cos(data->player->rot_angle + M_PI_2);
    double sin_angle = sin(data->player->rot_angle + M_PI_2);
    double new_x = data->player->player_x + cos_angle * move_step;
    double new_y = data->player->player_y + sin_angle * move_step;

    if (is_walkable(data, new_x, new_y))
    {
        data->player->player_x = new_x;
        data->player->player_y = new_y;
    }
}

void rotate_player(t_data *data, int direction)
{
    data->player->rot_angle += (direction * data->player->rot_speed);
    if (data->player->rot_angle < 0)
        data->player->rot_angle += 2 * M_PI;
    if (data->player->rot_angle > 2 * M_PI)
        data->player->rot_angle -= 2 * M_PI;
}

void draw_player_facing_line(t_data *data)
{
    double angle = data->player->rot_angle;
    int line_length = 50;
    int line_end_x = data->player->player_x * TILE_SIZE + cos(angle) * line_length;
    int line_end_y = data->player->player_y * TILE_SIZE + sin(angle) * line_length;
    int x1 = data->player->player_x * TILE_SIZE;
    int y1 = data->player->player_y * TILE_SIZE;    
    draw_line(data, x1, y1, line_end_x, line_end_y, 0xFF0000);  
}


void init_player_data(t_data *data)
{
    //data->player->player_x = 5; 
    //data->player->player_y = 5;
    //data->player->rot_angle = 0;
    data->player->turn_dir = 0;
    data->player->walk_dir = 0;
    data->player->move_speed = 0.02;
    data->player->rot_speed = 0.5 * (M_PI / 180);
}

int key_released(int keycode, t_data *data)
{
    if (keycode == 'w' || keycode == 's') // Stop forward/backward movement
        data->player->walk_dir = 0;
    else if (keycode == 'a' || keycode == 'd' || keycode == 65361 || keycode == 65363) // Stop rotation
        data->player->turn_dir = 0;

    return 0;
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
            my_mlx_pixel_put(data, player_x + px, player_y + py, 0xFF0000); // Red color for the square
            px++;
        }
        py++;
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
        int draw_start = -line_height / 2 + data->mlx->win_height / 2;
        if (draw_start < 0)
            draw_start = 0;
        int draw_end = line_height / 2 + data->mlx->win_height / 2;
        if (draw_end >= data->mlx->win_height)
            draw_end = data->mlx->win_height - 1;

        // Use green for the walls
        int color = 0x00FF00; // Green color

        // Draw the vertical line
        int y = draw_start;
        while (y < draw_end)
        {
            my_mlx_pixel_put(data, i, y, color);
            y++;
        }
        i++;
    }
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

void draw(t_data *data)
{
    clear_image(data); // Clear the screen

    // Draw the sky
    int sky_color = 0x87CEEB; // Light blue color for the sky
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

    // Draw the floor
    int floor_color = 0x8B4513; // Brown color for the floor
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
    draw_map(data, data->map->map, data->map->rows, data->map->cols);
    draw_player(data);
    draw_player_facing_line(data);
    cast_rays(data); // Render the 3D walls
    mlx_put_image_to_window(data->mlx->mlx, data->mlx->win, data->mlx->img, 0, 0);
}


int handle_keypress(int keycode, t_data *data)
{
    if (keycode == 'w') // Move forward
        data->player->walk_dir = 1;
    else if (keycode == 's') // Move backward
        data->player->walk_dir = -1;
    else if (keycode == 'a') // Rotate left (A key)
        data->player->turn_dir = -1; // Counter-clockwise
    else if (keycode == 'd') // Rotate right (D key)
        data->player->turn_dir = 1; // Clockwise
    else if (keycode == 65361) // Rotate left (Left Arrow)
        data->player->turn_dir = -1; // Counter-clockwise
    else if (keycode == 65363) // Rotate right (Right Arrow)
        data->player->turn_dir = 1; // Clockwise
    else if (keycode == 65307) // ESC key (exit game)
        close_window(data); // Exit game

    return 0;
}


int game_loop(t_data *data)
{
    if (data->player->walk_dir != 0)
        move_player(data);
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


void initialize_data(t_data *data)
{
    data->mlx = malloc(sizeof(t_mlx));
    data->player = malloc(sizeof(t_player));
    data->map = malloc(sizeof(t_map));
    
    if (!data->mlx || !data->player || !data->map)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
    init_player_data(data);

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
   
    parsed = parser(argc, argv);
    initialize_data(&data);
    
    data.map->map = parsed->map;
    data.map->rows = parsed->map_height;
    data.map->cols = parsed->map_width;
    
    data.player->player_x = parsed->player_x;
    data.player->player_y = parsed->player_y;
    data.player->rot_angle = dir_to_angle(parsed->player_dir);
    
    mlx_start(&data);
    start_game(&data);

    int i = 0;
    while (i < data.map->rows)
    {
        free(data.map->map[i]);
        i++;
    }
    free(data.map->map);
    return 0;
}

