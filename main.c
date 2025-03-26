#include "cub3d.h"

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

int is_walkable(t_data *data, int map_x, int map_y)
{
    if (map_x < 0 || map_x >= data->map->cols || map_y < 0 || map_y >= data->map->rows)
        return (0);

    return (data->map->map[map_y][map_x] == '0');
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
    data->player->player_x = 5; 
    data->player->player_y = 5;
    data->player->rot_angle = 0;
    data->player->turn_dir = 0;
    data->player->walk_dir = 0;
    data->player->move_speed = 0.01;
    data->player->rot_speed = 0.5 * (M_PI / 180);
}

int key_released(int keycode, t_data *data)
{
    if (keycode == 'w' || keycode == 's')
        data->player->walk_dir = 0;
    else if (keycode == 65361 || keycode == 65363)
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
    for (int py = -square_size / 2; py <= square_size / 2; py++)
    {
        for (int px = -square_size / 2; px <= square_size / 2; px++)
        {
            my_mlx_pixel_put(data, player_x + px, player_y + py, 0xFF0000); // Red color for the square
        }
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
    int num_rays = data->mlx->win_width;
    double fov = 60 * (M_PI / 180);     
    double angle_step = fov / num_rays;
    double start_angle = data->player->rot_angle - (fov / 2);
    for (int i = 0; i < num_rays; i++)
    {
        double ray_angle = start_angle + (i * angle_step);
        int side_hit;
        double perp_wall_dist = cast_ray(data, ray_angle, &side_hit);
        int line_height = (int)(data->mlx->win_height / perp_wall_dist);
        int draw_start = -line_height / 2 + data->mlx->win_height / 2;
        if (draw_start < 0)
            draw_start = 0;
        int draw_end = line_height / 2 + data->mlx->win_height / 2;
        if (draw_end >= data->mlx->win_height)
            draw_end = data->mlx->win_height - 1;
        int color = 0x00FF00; // Green color
        for (int y = draw_start; y < draw_end; y++)
        {
            my_mlx_pixel_put(data, i, y, color);
        }
    }
}

void clear_image(t_data *data)
{
    int width = data->mlx->win_width;
    int height = data->mlx->win_height;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            my_mlx_pixel_put(data, x, y, 0x000000); 
        }
    }
}

void draw(t_data *data)
{
    clear_image(data); 
    cast_rays(data);   
    mlx_put_image_to_window(data->mlx->mlx, data->mlx->win, data->mlx->img, 0, 0);
}


int handle_keypress(int keycode, t_data *data)
{
    if (keycode == 'w')
        data->player->walk_dir = 1;
    else if (keycode == 's') 
        data->player->walk_dir = -1;
    else if (keycode == 'a') 
        strafe_player(data, -1);
    else if (keycode == 'd') 
        strafe_player(data, 1);
    else if (keycode == 65361) 
        data->player->turn_dir = -1; 
    else if (keycode == 65363) 
        data->player->turn_dir = 1; 
    else if (keycode == 65307) 
        close_window(data);

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

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: ./program <map_file>\n");
        return 1;
    }
    t_data data;
    initialize_data(&data);
    data.map->map = read_map_from_file(argv[1], &data.map->rows, &data.map->cols);
    mlx_start(&data);
    start_game(&data);
    for (int i = 0; i < data.map->rows; i++)
    {
        free(data.map->map[i]);
    }
    free(data.map->map);
    return 0;
}

