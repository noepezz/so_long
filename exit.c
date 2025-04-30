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
	ft_memset(&map->img, 0, sizeof(t_img));
}

// Handles clean exit (ESC or window close button)
int	handle_close_event(t_map *map)
{
	if (!map)
		exit(EXIT_SUCCESS);
	write(1, "\nClosing game...\n", 17);
	free_map_arrays(map);
	if (map->mlx)
	{
		destroy_images(map);
		if (map->wnd)
			mlx_destroy_window(map->mlx, map->wnd);
	}
	exit(EXIT_SUCCESS);
	return (0);
}

// Handles win condition logic and display
void	handle_win_condition(t_map *map)
{
	if (!map || !map->mlx || !map->wnd)
		return;
	map->exit_flag = 1;
	mlx_clear_window(map->mlx, map->wnd);
	mlx_string_put(map->mlx, map->wnd, (map->x * IMG_PXL / 2) - 40,
	 (map->y * IMG_PXL / 2), 0x00FF00, "YOU WON!");
	write(1, "\n--------------------\n", 22);
	write(1, "    CONGRATULATIONS! \n", 22);
	write(1, "      Moves: ", 13);
	print_moves_terminal(map);
	write(1, "\n--------------------\n", 22);
	fflush(stdout);
}