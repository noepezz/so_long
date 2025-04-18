#include "so_long.h"

int	main(int ac, char **av)
{
	t_map	map; // Allocate map struct on the stack

	// Argument Check
	if (ac != 2)
	{
		write(2, "Usage: ./so_long <map_file.ber>\n", 32);
		return (EXIT_FAILURE);
	}

	// Initialization
	init_map_struct(&map, av);

	// Map Validation (includes reading, checks, pathfinding)
	validate_map(&map); // Exits internally on any error

	// Initialize MinilibX Connection
	map.mlx = mlx_init();
	if (!map.mlx)
		exit_with_error(&map, "Failed to initialize MinilibX connection.");

	// Load Game Images (Must happen AFTER mlx_init)
	load_game_images(&map); // Exits internally on loading failure

	// Create Game Window
	map.wnd = mlx_new_window(map.mlx, map.x * IMG_PXL, map.y * IMG_PXL, WND_NAME);
	if (!map.wnd)
		exit_with_error(&map, "Failed to create MinilibX window.");

	// Initial Draw and Output
	draw_map_to_window(&map);
	print_moves_terminal(&map); // Show initial moves (0)

	// Setup Event Hooks
	// Key press hook
	mlx_key_hook(map.wnd, handle_key_press, &map);
	// Window close button hook (destroy notify event)
	// Event 17, Mask 0 is standard for window close on Linux/macOS MLX
	mlx_hook(map.wnd, 17, 0, handle_close_event, &map);
	// Optional: Loop hook for animations independent of key presses
	// mlx_loop_hook(map.mlx, animation_update_function, &map);

	// Start Event Loop (this function blocks until exit)
	mlx_loop(map.mlx);

	// Cleanup is handled by handle_close_event or exit_with_error
	// Code here is normally not reached if mlx_loop runs correctly.
	return (EXIT_SUCCESS);
}