#include "cub3d.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int read_lines(int fd)
{
    int line_count;
    char *line;
    line_count = 0;
    while (get_next_line(fd) > 0)
    {
        line_count++;
        free(line);
    }
    return (line_count);
}

int get_map_width(char *line)
{
    int i;
    int width;
    i = 0;
    width = 0;
    while (line[i])
    {
        if (line[i] == '1')
            width++;
        i++;
    }
    return (width);
}

int get_map_height(char **file, int line_count)
{
    int i;
    int height;
    i = 0;
    height = 0;
    while (i < line_count)
    {
        if (file[i][0] == '1')
            height++;
        i++;
    }
    return (height);
}

void fill_map(t_data *data)
{
    int i;
    int j;
    int k;
    i = 0;
    j = 0;
    k = 0;
    data->map->map = (char **)malloc(sizeof(char *) * data->map->height);
    while (i < data->map->height)
    {
        data->map->map[i] = (char *)malloc(sizeof(char) * data->map->width);
        while (j < data->map->width)
        {
            data->map->map[i][j] = data->map->file[k][j];
            j++;
        }
        j = 0;
        k++;
        i++;
    }
}

void read_map_file(t_data *data, const char *filename)
{
    int i = 0;
    char *line;
    char buffer[1024];
    int bytes_read;
    int line_length;
    int buffer_index;
    int fd;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        perror("Error\nopen failed");
        exit(1);
    }

    data->map->file = (char **)malloc(sizeof(char *) * data->map->line_count);

    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[bytes_read] = '\0';
        buffer_index = 0;
        while (buffer[buffer_index])
        {
            line_length = 0;
            while (buffer[buffer_index + line_length] && buffer[buffer_index + line_length] != '\n')
                line_length++;
            line = (char *)malloc(sizeof(char) * (line_length + 1));
            for (int j = 0; j < line_length; j++)
                line[j] = buffer[buffer_index + j];
            line[line_length] = '\0';
            data->map->file[i] = line;
            i++;
            buffer_index += line_length + 1;
        }
    }
    data->map->file[i] = NULL;
    close(fd);
}

void init_data(t_data *data, const char *filename)
{
    data->map = (t_map *)malloc(sizeof(t_map));
    data->map->fd = open(filename, O_RDONLY);
    if (data->map->fd < 0)
    {
        perror("Error\nopen failed");
        exit(1);
    }
    data->map->line_count = read_lines(data->map->fd);
    close(data->map->fd);
    read_map_file(data, filename);
    data->map->width = get_map_width(data->map->file[0]);
    data->map->height = get_map_height(data->map->file, data->map->line_count);
    fill_map(data);

    data->player = (t_player *)malloc(sizeof(t_player));
    data->ray = (t_ray *)malloc(sizeof(t_ray));
    data->mlx = (t_mlx *)malloc(sizeof(t_mlx));

    data->player->player_x = 5;
    data->player->player_y = 4;
    data->mlx->mlx = NULL;
    data->mlx->win = NULL;
    data->mlx->img = NULL;
    data->win_height = SCREEN_HEIGHT;
    data->win_width = SCREEN_WIDTH;
}

void init_mlx(t_data *data)
{
    data->mlx->mlx = mlx_init();
    if (!data->mlx->mlx)
    {
        printf("Error\nmlx_init failed\n");
        exit(1);
    }
    data->mlx->win = mlx_new_window(data->mlx->mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "cub3d");
    if (!data->mlx->win)
    {
        printf("Error\nmlx_new_window failed\n");
        exit(1);
    }
    data->mlx->img = mlx_new_image(data->mlx->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!data->mlx->img)
    {
        printf("Error\nmlx_new_image failed\n");
        exit(1);
    }
    mlx_put_image_to_window(data->mlx->mlx, data->mlx->win, data->mlx->img, 0, 0);
    return;
}
bool can_move_to(t_map *map, int x, int y) {
    if (x < 0 || x >= map->width || y < 0 || y >= map->height) {
        return false;
    }
    return map->map[y][x] != '1';
}

void move_player(t_player *player, t_map *map, int dx, int dy) {
    int new_x = player->player_x + dx;
    int new_y = player->player_y + dy;
    if (can_move_to(map, new_x, new_y)) {
        player->player_x = new_x;
        player->player_y = new_y;
    }
}

int close_window(t_data *data)
{
    mlx_destroy_window(data->mlx->mlx, data->mlx->win);
    printf("Exited Game\n");
    exit(0);
    return (0);
}

void draw_square(t_data *data, int x, int y, int color, int square_size)
{
    int i, j;
    char *pixel;
    int bpp, size_line, endian;

    pixel = mlx_get_data_addr(data->mlx->img, &bpp, &size_line, &endian);
    for (i = 0; i < square_size; i++)
    {
        for (j = 0; j < square_size; j++)
        {
            int pixel_index = ((y + i) * size_line) + ((x + j) * (bpp / 8));
            if (i == 0 || j == 0 || i == square_size - 1 || j == square_size - 1)
                *(int *)(pixel + pixel_index) = 0x000000; // Draw border
            else
                *(int *)(pixel + pixel_index) = color;
        }
    }
}

void draw_map(t_data *data)
{
    int i, j;
    int x, y;
    int square_width = SCREEN_WIDTH / data->map->width;
    int square_height = SCREEN_HEIGHT / data->map->height;
    int square_size = (square_width < square_height) ? square_width : square_height;

    // Clear the image before drawing
    char *pixel = mlx_get_data_addr(data->mlx->img, &bpp, &size_line, &endian);
    memset(pixel, 0, SCREEN_WIDTH * SCREEN_HEIGHT * (bpp / 8));

    for (i = 0; i < data->map->height; i++)
    {
        for (j = 0; j < data->map->width; j++)
        {
            x = j * square_size;
            y = i * square_size;
            if (data->map->map[i][j] == '1')
                draw_square(data, x, y, 0x00FF00, square_size); // Green for walls
            else if (data->map->map[i][j] == '0')
                draw_square(data, x, y, 0xFFFFFF, square_size); // White for empty space
        }
    }
    draw_square(data, data->player->player_x * square_size, data->player->player_y * square_size, 0xFF0000, square_size / 2);
    mlx_put_image_to_window(data->mlx->mlx, data->mlx->win, data->mlx->img, 0, 0);
}

int key_press(int keycode, t_data *data)
{
    if (keycode == 65307) // Escape key
    {
        printf("Exited Game!\n");
        close_window(data);
    }
    else if (keycode == 'w') // Move up
    {
        move_player(data->player, data->map, 0, -1);
    }
    else if (keycode == 'a') // Move left
    {
        move_player(data->player, data->map, -1, 0);
    }
    else if (keycode == 's') // Move down
    {
        move_player(data->player, data->map, 0, 1);
    }
    else if (keycode == 'd') // Move right
    {
        move_player(data->player, data->map, 1, 0);
    }
    draw_map(data); // Redraw the map with the new player position
    return (0);
}

int key_release(int keycode, t_data *data)
{
    // Handle key release events if needed
    return (0);
}

int update(t_data *data)
{
    draw_map(data); // Continuously redraw the map
    return (0);
}

void start_game(t_data *data)
{
    init_mlx(data);
    draw_map(data); // Draw the map when the game starts
    mlx_hook(data->mlx->win, 2, 1L<<0, key_press, data); // Listen for key press events
    mlx_hook(data->mlx->win, 3, 1L<<1, key_release, data); // Listen for key release events
    mlx_loop_hook(data->mlx->mlx, update, data); // Continuously update the game
    mlx_hook(data->mlx->win, 17, 0, close_window, data); // Listen for window close events
    mlx_loop(data->mlx->mlx);
    return;
}

int main(int ac, char **av)
{
    if (ac != 2)
    {
        printf("Usage: %s <map_file>\n", av[0]);
        return (1);
    }

    t_data data;
    init_data(&data, av[1]);
    start_game(&data);
    return (0);
}
