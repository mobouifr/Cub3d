#include "cub3d.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int read_lines(int fd)
void my_mlx_pixel_put(t_data *data, int x, int y, int color)
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
    char *dst;

    dst = data->mlx->addr + (y * data->mlx->line_length + x * (data->mlx->bpp / 8));
    *(unsigned int*) dst = color;
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
bool can_move_to(t_map *map, int x, int y) 
{
    if (x < 0 || x >= map->width || y < 0 || y >= map->height) 
    {
        return false;
    }
    return map->map[y][x] != '1';
}

void move_player(t_player *player, t_map *map, double dx, double dy) 
{
    double new_x = player->player_x + dx;
    double new_y = player->player_y + dy;
    int map_x = (int)new_x;
    int map_y = (int)new_y;
    if (can_move_to(map, map_x, map_y)) 
    {
        player->player_x = new_x;
        player->player_y = new_y;
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
        my_mlx_pixel_put(data, x1, y1, color);  // Draw pixel at (x1, y1)
        
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
    double angle = data->player->rot_angle;  // The angle the player is facing in radians
    int line_length = 50;  // Length of the line showing where the player faces (adjust as needed)
    
    // Calculate the end point of the line using trigonometry
    int line_end_x = data->player->player_x * TILE_SIZE + cos(angle) * line_length;
    int line_end_y = data->player->player_y * TILE_SIZE + sin(angle) * line_length;

    // Draw the line from player to the calculated endpoint
    int x1 = data->player->player_x * TILE_SIZE;  // Player's current x position (screen coordinates)
    int y1 = data->player->player_y * TILE_SIZE;  // Player's current y position (screen coordinates)
    
    // Draw a line from (x1, y1) to (line_end_x, line_end_y)
    draw_line(data, x1, y1, line_end_x, line_end_y, 0xFF0000);  // Red color for the line
}


void init_player_data(t_data *data)
{
    data->player->player_x = 5; 
    data->player->player_y = 5;
    data->player->rot_angle = 0;
    data->player->turn_dir = 0;
    data->player->walk_dir = 0;
    data->player->move_speed = 2;
    data->player->rot_speed = 2 * (M_PI / 180);
}

void mlx_start(t_data *data)
{
    // Dynamically calculate the screen dimensions
    int screen_width = data->map->cols * TILE_SIZE;
    int screen_height = data->map->rows * TILE_SIZE;

    data->mlx->mlx = mlx_init();
    data->mlx->win = mlx_new_window(data->mlx->mlx, screen_width, screen_height, "cub3d");
    data->mlx->img = mlx_new_image(data->mlx->mlx, screen_width, screen_height);
    data->mlx->addr = mlx_get_data_addr(data->mlx->img, &data->mlx->bpp, &data->mlx->line_length, &data->mlx->endian);
}



int close_window(t_data *data)
{
    mlx_destroy_window(data->mlx->mlx, data->mlx->win);
    printf("Exit Game!\n");
    exit(0);
    return (0);
}
int key_released(int keycode, t_data *data)
{
    if (keycode == 'w')
        data->player->walk_dir = 0;
    else if (keycode == 'a')
        data->player->turn_dir = 0;
    else if (keycode == 's')
        data->player->walk_dir = 0;
    else if (keycode == 'd')
        data->player->turn_dir = 0;
    return 0;
}
int key_pressed(int keycode, t_data *data)
{
    if (keycode == 65307)
        close_window(data);
    if (keycode == 'w')
        data->player->walk_dir = 1;
    else if (keycode == 'a')
        data->player->turn_dir = -1;
    else if (keycode == 's')
        data->player->walk_dir = -1;
    else if (keycode == 'd')
        data->player->turn_dir = 1;
    return 0;
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
    int bpp, size_line, endian;
    char *pixel = mlx_get_data_addr(data->mlx->img, &bpp, &size_line, &endian);
    memset(pixel, 0, SCREEN_WIDTH * SCREEN_HEIGHT * (bpp / 8));

    for (i = 0; i < data->map->height; i++)
    {
        for (j = 0; j < data->map->width; j++)
void draw_map(t_data *data, char **map, int rows, int cols)
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

            // Draw the grid lines and tiles
            y = 0;
            while (y < TILE_SIZE)
            {
                x = 0;
                while (x < TILE_SIZE)
                {
                    // Draw the grid lines
                    if (x == 0 || y == 0)
                        my_mlx_pixel_put(data, tile_x + x, tile_y + y, 0x808080); // Gray grid
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

void draw_player(t_data *data)
{
    // Get player position and center the screen
    int screen_width = data->map->cols * TILE_SIZE;
    int screen_height = data->map->rows * TILE_SIZE;

    int player_x = data->player->player_x * TILE_SIZE;
    int player_y = data->player->player_y * TILE_SIZE;

    // Debug: Print player position
    printf("Player Position (in tiles): (%f, %f)\n", data->player->player_x, data->player->player_y);

    // Calculate screen-centered player position
    int screen_center_x = screen_width / 2;
    int screen_center_y = screen_height / 2;

    int player_screen_x = screen_center_x - player_x;
    int player_screen_y = screen_center_y - player_y;

    // Debug: Print screen-centered player position
    printf("Player Screen Position: (%d, %d)\n", player_screen_x, player_screen_y);

    // Draw the player as a red circle (indicating the player's position)
    int radius = 10; // Radius of the circle
    int px, py;
    for (py = -radius; py <= radius; py++)
    {
        for (px = -radius; px <= radius; px++)
        {
            if (px * px + py * py <= radius * radius) // Check if the point lies within the circle
            {
                int screen_x = player_screen_x + px;
                int screen_y = player_screen_y + py;
                if (screen_x >= 0 && screen_x < screen_width && screen_y >= 0 && screen_y < screen_height)
                {
                    my_mlx_pixel_put(data, screen_x, screen_y, 0xFF0000); // Red color for the player
                }
            }
        }
    }

    // Debug: Check the angle (rotation) of the player
    printf("Player Rotation Angle: %d\n", data->player->rot_angle);

    // Draw a line representing the direction the player is facing
    double angle = data->player->rot_angle; // Player's rotation angle
    int line_length = 30;  // Length of the line representing the facing direction

    int line_end_x = player_screen_x + cos(angle) * line_length;
    int line_end_y = player_screen_y + sin(angle) * line_length;

    // Draw the line (debugging the line's coordinates)
    printf("Drawing line from (%d, %d) to (%d, %d)\n", player_screen_x, player_screen_y, line_end_x, line_end_y);

    // Draw the line
    my_mlx_pixel_put(data, player_screen_x, player_screen_y, 0x00FF00); // Green for the start of the line (player's position)
    my_mlx_pixel_put(data, line_end_x, line_end_y, 0x00FF00); // Green for the end of the line (direction the player is facing)
}


void draw(t_data *data)
{
    mlx_clear_window(data->mlx->mlx, data->mlx->win);

    draw_map(data, data->map->map, data->map->rows, data->map->cols);

    draw_player(data);

    mlx_put_image_to_window(data->mlx->mlx, data->mlx->win, data->mlx->img, 0, 0);
}

char **read_map_from_file(char *filename, int *rows, int *cols)
{
    int fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        printf("Error opening file\n");
        exit(1);
    }

    char **map = malloc(sizeof(char *) * 100);
    if (!map)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }

    char *line;
    int i = 0;
    *cols = 0;

    while ((line = get_next_line(fd)) != NULL)
    {
        // Trim trailing spaces
        char *trimmed_line = strdup(line);
        if (!trimmed_line)
        {
            x = j * square_size;
            y = i * square_size;
            if (data->map->map[i][j] == '1')
                draw_square(data, x, y, 0x00FF00, square_size); // Green for walls
            else if (data->map->map[i][j] == '0')
                draw_square(data, x, y, 0xFFFFFF, square_size); // White for empty space
        }
    }
    draw_square(data, (int)(data->player->player_x * square_size), (int)(data->player->player_y * square_size), 0xFF0000, square_size / 2);
    mlx_put_image_to_window(data->mlx->mlx, data->mlx->win, data->mlx->img, 0, 0);
}

int key_press(int keycode, t_data *data)
{
    double move_speed = 0.1; // Adjust the speed for smoother movement
    if (keycode == 65307) // Escape key
    {
        printf("Exited Game!\n");
        close_window(data);
    }
    else if (keycode == 'w') // Move up
    {
        move_player(data->player, data->map, 0, -move_speed);
    }
    else if (keycode == 'a') // Move left
    {
        move_player(data->player, data->map, -move_speed, 0);
    }
    else if (keycode == 's') // Move down
    {
        move_player(data->player, data->map, 0, move_speed);
    }
    else if (keycode == 'd') // Move right
    {
        move_player(data->player, data->map, move_speed, 0);
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
            printf("Memory allocation failed\n");
            exit(1);
        }
        
        // Remove trailing newline and spaces
        size_t len = strlen(trimmed_line);
        while (len > 0 && (trimmed_line[len - 1] == '\n' || trimmed_line[len - 1] == ' '))
        {
            trimmed_line[len - 1] = '\0';
            len--;
        }

        map[i] = malloc(len + 1);  // Allocate memory for the trimmed line
        if (!map[i])
        {
            printf("Memory allocation failed\n");
            exit(1);
        }

        strcpy(map[i], trimmed_line);
        
        if ((size_t)*cols < len)
            *cols = len;
        i++;
        free(line);
        free(trimmed_line);
    }

    *rows = i;

    close(fd);
    return map;
}   

int game_loop(t_data *data)
{
    // Update the player's rotation if there's input
    if (data->player->turn_dir != 0)
        data->player->rot_angle += data->player->turn_dir * data->player->rot_speed;

    // Normalize the rotation angle to keep it within 0 to 2*PI (0-360 degrees)
    if (data->player->rot_angle > 2 * M_PI)
        data->player->rot_angle -= 2 * M_PI;
    if (data->player->rot_angle < 0)
        data->player->rot_angle += 2 * M_PI;

    // Redraw the map and the player after rotation update
    draw(data);  // Clear and redraw the map and player

    return 0; // MLX expects a return value
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
    mlx_hook(data->mlx->win, 2, 1L << 0, key_pressed, data);  // Handle key presses
    mlx_hook(data->mlx->win, 3, 1L << 1, key_released, data); // Handle key releases
    mlx_hook(data->mlx->win, 17, 0, close_window, data);      // Handle window close
    mlx_loop_hook(data->mlx->mlx, game_loop, data); 

    mlx_loop(data->mlx->mlx);  // Start the MLX loop
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


int main(int ac, char **av)
{
    if (ac != 2)
    {
        printf("Usage: %s <map_file>\n", av[0]);
        return (1);
    }

    if (argc < 2)
    {
        printf("Usage: ./program <map_file>\n");
        return 1;
    }

    t_data data;
    init_data(&data, av[1]);
    initialize_data(&data);

    // Read map and assign to data.map->map
    data.map->map = read_map_from_file(argv[1], &data.map->rows, &data.map->cols);

    // Start MLX and game loop
    mlx_start(&data);
    start_game(&data);

    // Free the map memory after use
    for (int i = 0; i < data.map->rows; i++)
    {
        free(data.map->map[i]);
    }
    free(data.map->map);

    return 0;
}
