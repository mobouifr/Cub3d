#include <mlx.h>

int main()
{
    void    *mlx;
    void    *mlx_win;
    int     x, y;
    int     border_size = 10; // Size of the border
    int     square_size = 100; // Size of the square

    mlx = mlx_init();
    mlx_win = mlx_new_window(mlx, 1920, 1080, "Cub3D");

    // Draw the square with a border
    for (x = 100; x < 100 + square_size; x++) 
	{
        for (y = 100; y < 100 + square_size; y++) 
		{
            // Check if the pixel is within the border
            if (x < 100 + border_size || x >= 100 + square_size - border_size || 
                y < 100 + border_size || y >= 100 + square_size - border_size) 
			{
                mlx_pixel_put(mlx, mlx_win, x, y, 0xFF0000); // Red border
            }
			else 
			{
                mlx_pixel_put(mlx, mlx_win, x, y, 0x00FF00); // Green fill
            }
        }
    }
    mlx_loop(mlx);
    return 0;
}
