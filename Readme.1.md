# cub3d

A simple raycasting 3D engine in C using MiniLibX, inspired by Wolfenstein 3D.  
This project renders a 3D view from a 2D map, allowing the player to move, strafe, and rotate, with textured walls and colored floor/ceiling.

---

## Table of Contents

- [Overview](#overview)
- [How It Works](#how-it-works)
- [Main Functions](#main-functions)
- [Formulas and Math](#formulas-and-math)
- [Controls](#controls)
- [File Structure](#file-structure)
- [Compilation & Running](#compilation--running)
- [Credits](#credits)

---

## Overview

This project implements a basic 3D raycasting engine:
- Reads a map file describing walls and player start.
- Loads wall textures and color settings.
- Uses raycasting to render a 3D view from the player's perspective.
- Supports smooth movement, strafing, and rotation.
- Handles collision detection and wall sliding.

---

## How It Works

1. **Parsing:**  
   The program parses a map file, extracting the map, player start position/direction, and texture/color paths.

2. **Initialization:**  
   Initializes all data structures, loads textures, and sets up the MiniLibX window.

3. **Game Loop:**  
   On each frame:
   - Handles input (movement, strafing, rotation).
   - Moves the player, checking for collisions.
   - Casts rays for each vertical screen column to determine wall distances and textures.
   - Draws the sky, floor, and textured walls.
   - Updates the window.

---

## Main Functions

### Parsing and Initialization

- **parser(argc, argv):**  
  Parses the map file and returns a `t_game` struct with all game variables.

- **initialize_data(t_data *data):**  
  Allocates and initializes all main data structures (`t_mlx`, `t_player`, `t_map`, `t_colors`).

- **init_player_data(t_data *data):**  
  Sets initial player movement/rotation states and speeds.

- **dir_to_angle(char c):**  
  Converts a direction character ('N', 'S', 'E', 'W') to a rotation angle in radians.

---

### Input Handling

- **handle_keypress(int keycode, t_data *data):**  
  Sets movement/rotation/strafe directions based on key pressed.

- **key_released(int keycode, t_data *data):**  
  Resets movement/rotation/strafe directions when keys are released.

---

### Player Movement

- **move_player(t_data *data):**  
  Moves the player forward/backward.  
  Checks X and Y axes separately for collision, allowing wall sliding.

- **strafe_player(t_data *data, int direction):**  
  Moves the player left/right (strafing).  
  Also checks X and Y axes separately for smooth gliding along walls.

- **rotate_player(t_data *data, int direction):**  
  Rotates the player's view left/right.

- **is_walkable(t_data *data, double new_x, double new_y):**  
  Checks if the given position is not colliding with a wall, using a small buffer for the player's size.

---

### Rendering

- **draw(t_data *data):**  
  Clears the screen, draws the sky and floor, casts rays to render walls, and updates the window.

- **clear_image(t_data *data):**  
  Fills the image buffer with black (clears the screen).

- **cast_rays(t_data *data):**  
  For each vertical column on the screen, casts a ray to find wall intersections and draws the corresponding textured wall slice.

- **cast_ray(t_data *data, double ray_angle, int *side_hit):**  
  Performs DDA (Digital Differential Analysis) to find the distance to the nearest wall for a given ray angle.

- **draw_line(t_data *data, int x1, int y1, int x2, int y2, int color):**  
  Draws a line using Bresenham's algorithm (used for debugging/minimap).

- **draw_player(t_data *data):**  
  Draws the player as a small square (for minimap/debug).

- **draw_player_facing_line(t_data *data):**  
  Draws a line showing the player's facing direction (for minimap/debug).

---

### Texture Handling

- **load_texture(t_data *data, char *path):**  
  Loads an XPM texture file and returns a `t_texture` struct.

- **get_texture_index(double ray_dir_x, double ray_dir_y, int side_hit):**  
  Determines which wall texture to use based on the ray direction and which side was hit.

- **get_tex_color(t_texture *tex, int x, int y):**  
  Gets the color of a pixel from a texture at (x, y).

---

### Utility

- **close_window(t_data *data):**  
  Cleans up and exits the program.

---

## Formulas and Math

### Raycasting (DDA Algorithm)

- **Ray Direction:**  
  ```c
  ray_dir_x = cos(ray_angle);
  ray_dir_y = sin(ray_angle);
  ```

- **Delta Distances:**  
  Distance the ray must travel to go from one x-side to the next, and y-side to the next:
  ```c
  delta_dist_x = fabs(1 / ray_dir_x);
  delta_dist_y = fabs(1 / ray_dir_y);
  ```

- **Step and Initial Side Distances:**  
  Determines the direction to step in the grid and the initial distance to the first side:
  ```c
  if (ray_dir_x < 0) { step_x = -1; side_dist_x = (ray_x - map_x) * delta_dist_x; }
  else { step_x = 1; side_dist_x = (map_x + 1.0 - ray_x) * delta_dist_x; }
  // Same for Y
  ```

- **DDA Loop:**  
  Steps through the map grid until a wall is hit.

- **Perpendicular Wall Distance:**  
  Corrects for the "fish-eye" effect:
  ```c
  if (side_hit == 0)
      perp_wall_dist = (map_x - ray_x + (1 - step_x) / 2) / ray_dir_x;
  else
      perp_wall_dist = (map_y - ray_y + (1 - step_y) / 2) / ray_dir_y;
  ```

- **Fish-eye Correction:**  
  ```c
  corrected_dist = perp_wall_dist * cos(ray_angle - player_angle);
  ```

- **Wall Height:**  
  ```c
  line_height = (int)(win_height / corrected_dist);
  ```

- **Texture X Coordinate:**  
  ```c
  wall_x = (side_hit == 0) ? player_y + perp_wall_dist * sin(ray_angle)
                           : player_x + perp_wall_dist * cos(ray_angle);
  wall_x -= floor(wall_x);
  tex_x = (int)(wall_x * tex->width);
  ```

---

### Player Movement

- **Forward/Backward:**  
  ```c
  new_x = player_x + cos(player_angle) * move_step;
  new_y = player_y + sin(player_angle) * move_step;
  ```

- **Strafing (Left/Right):**  
  ```c
  new_x = player_x + cos(player_angle + M_PI_2) * move_step;
  new_y = player_y + sin(player_angle + M_PI_2) * move_step;
  ```

- **Wall Sliding:**  
  X and Y are checked separately for collision, allowing the player to slide along walls.

---

### Collision Detection

- **Buffer:**  
  A small value (e.g., 0.2) is subtracted/added to the player's position to check all four corners of the player's "hitbox" for wall collisions.

---

## Controls

| Key         | Action                |
|-------------|-----------------------|
| W           | Move forward          |
| S           | Move backward         |
| A           | Strafe left           |
| D           | Strafe right          |
| ← / →       | Rotate left/right     |
| ESC         | Exit game             |

---

## File Structure

```
cub3d/
├── src/
│   ├── main.c         // Main logic and game loop
│   ├── ...            // Other source files
├── include/
│   ├── cub3d.h        // All struct and function declarations
├── textures/          // XPM texture files
├── maps/              // Map files
└── README.md
```

---

## Compilation & Running

1. **Compile:**  
   Make sure you have MiniLibX installed.  
   ```sh
   make
   ```

2. **Run:**  
   ```sh
   ./cub3d maps/map.cub
   ```

---

## Credits

- Raycasting algorithm based on Lode Vandevenne's [Raycasting Tutorial](https://lodev.org/cgtutor/raycasting.html).
- MiniLibX by 42 Network.

---

## Comments

All major functions are commented in the code to explain their purpose and logic.  
If you want to enable the minimap or player debug drawing, uncomment the relevant lines in `draw()`.

---

```c
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
    cast_rays(data);
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
```