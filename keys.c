#include "so_long.h"

// Finds the initial player position 'P' in the map array
void	find_player_start(t_map *map)
{
	int	x;
	int	y;

	y = 0;
	while (y < map->y)
	{
		x = 0;
		while (x < map->x)
		{
			if (map->array[y][x] == PLAYER)
			{
				map->sprite.x = x;
				map->sprite.y = y;
				return; // Found player
			}
			x++;
		}
		y++;
	}
	// Should have been caught by check_map_params if P doesn't exist
	exit_with_error(map, "Player start 'P' not found (internal error).");
}

// Main key press event handler
int	handle_key_press(int keycode, t_map *map)
{
	if (keycode == KEY_ESC)
		return (handle_close_event(map)); // Handles cleanup and exit

	// Ignore movement keys if game is already won
	if (map->exit_flag == 1)
		return (0);

	// Call perform_move directly with appropriate dx, dy based on keycode
	if (keycode == KEY_W || keycode == KEY_UP)
		perform_move(map, 0, -1);  // Up
	else if (keycode == KEY_A || keycode == KEY_LEFT)
		perform_move(map, -1, 0); // Left
	else if (keycode == KEY_S || keycode == KEY_DOWN)
		perform_move(map, 0, 1);   // Down
	else if (keycode == KEY_D || keycode == KEY_RIGHT)
		perform_move(map, 1, 0);   // Right
	// Add other keys here if needed

	return (0); // Indicate event was handled
}