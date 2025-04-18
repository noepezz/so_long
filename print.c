#include "so_long.h"

// Draws a single tile to the window at specific PIXEL coordinates
void	draw_tile(t_map *map, int px_x, int px_y)
{
	int		tile_x;
	int		tile_y;
	char	type;
	void	*img_to_draw;

	tile_x = px_x / IMG_PXL;
	tile_y = px_y / IMG_PXL;
	// Bounds check for safety, although map_out should be correct
	if (tile_y < 0 || tile_y >= map->y || tile_x < 0 || tile_x >= map->x)
		return;
	type = map->array[tile_y][tile_x];
	// Determine the base image (always draw empty first)
	 // 1. Always draw the background (empty) tile first
	 img_to_draw = map->img.empty; // Assumes grass.xpm
	 mlx_put_image_to_window(map->mlx, map->wnd, img_to_draw, px_x, px_y);
 
	 // 2. Determine the foreground image based on the map character
	 img_to_draw = NULL;
	 if (type == WALL)         img_to_draw = map->img.wall;
	 else if (type == COLLECTIBLE) img_to_draw = map->img.collectible;
	 else if (type == EXIT)    img_to_draw = map->img.exit;
	 else if (type == PLAYER)  img_to_draw = map->img.current_player;
 
	 // 3. Draw the foreground image if it's not empty
	 if (img_to_draw != NULL)
		 mlx_put_image_to_window(map->mlx, map->wnd, img_to_draw, px_x, px_y);
}

// Draws the entire map to the window initially or redraws all
void	draw_map_to_window(t_map *map)
{
	int	y;
	int	x;

	if (!map || !map->mlx || !map->wnd)
		return; // Safety check

	// Optional: Clear window before drawing - can cause flicker
	// mlx_clear_window(map->mlx, map->wnd);
	y = 0;
	while (y < map->y)
	{
		x = 0;
		while (x < map->x)
		{
			// Call helper function for each tile, passing PIXEL coordinates
			draw_tile(map, x * IMG_PXL, y * IMG_PXL);
			x++;
		}
		y++;
	}
}

// Prints move count to the terminal, overwriting the previous line
void	print_moves_terminal(t_map *map)
{
	char	*move_str;

	move_str = ft_itoa(map->moves); // Use ft_itoa from libft
	if (!move_str)
		return; // Handle potential itoa failure

	write(1, "\rMoves: ", 8);         // \r moves cursor to beginning of line
	write(1, move_str, ft_strlen(move_str));
	// Overwrite potential leftover digits from previous longer numbers
	write(1, "        ", 8);
	// Don't add newline "\n" here - keeps output on one line

	free(move_str);
	fflush(stdout); // Ensure the output buffer is flushed immediately
}