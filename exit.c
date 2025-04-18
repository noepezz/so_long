#include "so_long.h"

// Frees all loaded MLX images
void	destroy_images(t_map *map)
{
	if (!map || !map->mlx) // No MLX connection, nothing to destroy
		return;
	// Destroy images safely, checking if pointer is non-NULL
	if (map->img.empty)
		mlx_destroy_image(map->mlx, map->img.empty);
	if (map->img.wall)
		mlx_destroy_image(map->mlx, map->img.wall);
	if (map->img.exit)
		mlx_destroy_image(map->mlx, map->img.exit);
	if (map->img.collectible)
		mlx_destroy_image(map->mlx, map->img.collectible);
	if (map->img.player_right1)
		mlx_destroy_image(map->mlx, map->img.player_right1);
	if (map->img.player_right2)
		mlx_destroy_image(map->mlx, map->img.player_right2);
	if (map->img.player_left1)
		mlx_destroy_image(map->mlx, map->img.player_left1);
	if (map->img.player_left2)
		mlx_destroy_image(map->mlx, map->img.player_left2);
	if (map->img.player_up1)
		mlx_destroy_image(map->mlx, map->img.player_up1);
	if (map->img.player_up2)
		mlx_destroy_image(map->mlx, map->img.player_up2);
	if (map->img.player_down1)
		mlx_destroy_image(map->mlx, map->img.player_down1);
	if (map->img.player_down2)
		mlx_destroy_image(map->mlx, map->img.player_down2);
	// Set pointers to NULL after destroying (optional but good practice)
	ft_memset(&map->img, 0, sizeof(t_img));
}

// Handles clean exit (ESC or window close button)
int	handle_close_event(t_map *map)
{
	if (!map)
		exit(EXIT_SUCCESS); // Exit cleanly if map is somehow NULL

	write(1, "\nClosing game...\n", 17);
	free_map_arrays(map); // Free map data
	if (map->mlx) // Check if MLX was initialized
	{
		destroy_images(map); // Destroy images
		if (map->wnd)
			mlx_destroy_window(map->mlx, map->wnd); // Destroy window
		// mlx_destroy_display is usually not needed/harmful on modern systems
	}
	exit(EXIT_SUCCESS); // Exit with success code
	return (0); // Keep compiler happy, though exit() prevents return
}

// Handles win condition logic and display
void	handle_win_condition(t_map *map)
{
	// Check for valid map, mlx, and wnd pointers
	if (!map || !map->mlx || !map->wnd)
		return; // Cannot display win message without graphics context

	map->exit_flag = 1; // Set flag to prevent further moves

	// Optional: Clear window before displaying win message
	// mlx_clear_window(map->mlx, map->wnd); // Can cause flicker

	// Display win message on the window (adjust coordinates as needed)
	mlx_string_put(map->mlx, map->wnd,
		(map->x * IMG_PXL / 2) - 40, // Approx center X
		(map->y * IMG_PXL / 2),      // Approx center Y
		0x00FF00,                   // Green color
		"YOU WON!");

	// Print final status to terminal
	write(1, "\n--------------------\n", 22);
	write(1, "    CONGRATULATIONS! \n", 22);
	write(1, "      Moves: ", 13);
	print_moves_terminal(map); // Print final count
	write(1, "\n--------------------\n", 22);
	fflush(stdout); // Ensure terminal output is displayed

	// Game loop continues, handle_key_press ignores moves now.
}