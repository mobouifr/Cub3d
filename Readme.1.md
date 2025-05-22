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