#include "so_long.h"

int	main(int ac, char **av)
{
	t_map	map;

	if (ac != 2)
	{
		write(2, "Usage: ./so_long <map_file.ber>\n", 32);
		return (EXIT_FAILURE);
	}
	init_map_struct(&map, av);
	validate_map(&map);
	map.mlx = mlx_init();
	if (!map.mlx)
		exit_with_error(&map, "Failed to initialize MinilibX connection.");
	load_game_images(&map);
	map.wnd = mlx_new_window(map.mlx, map.x * IMG_PXL, map.y * IMG_PXL, WND_NAME);
	if (!map.wnd)
		exit_with_error(&map, "Failed to create MinilibX window.");
	draw_map_to_window(&map);
	print_moves_terminal(&map);
	mlx_key_hook(map.wnd, handle_key_press, &map);
	mlx_hook(map.wnd, 17, 0, handle_close_event, &map);
	mlx_loop(map.mlx);
	return (EXIT_SUCCESS);
}