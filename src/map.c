#include "cub3d.h"

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

