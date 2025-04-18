#include "so_long.h"

// Helper function to load one image with error checking
void	*load_single_image(t_map *map, char *path)
{
	void	*img_ptr;
	int		width;
	int		height;

	img_ptr = mlx_xpm_file_to_image(map->mlx, path, &width, &height);
	if (img_ptr == NULL)
	{
		// Construct error message including the path
		char *err_msg_start = "Failed to load image: ";
		char *full_msg = malloc(ft_strlen(err_msg_start) + ft_strlen(path) + 1);
		if (full_msg)
		{
			ft_strlcpy(full_msg, err_msg_start, ft_strlen(err_msg_start) + 1);
			ft_strlcat(full_msg, path, ft_strlen(err_msg_start) + ft_strlen(path) + 1);
			exit_with_error(map, full_msg); // exit_with_error handles cleanup
			free(full_msg); // Won't be reached, but good practice
		}
		else
			exit_with_error(map, "Failed to load image (and failed alloc msg)");
	}
	// Optional check: ensure image size matches expected tile size
/*	if (width != IMG_PXL || height != IMG_PXL)
	{
		// Handle mismatch - for 42 usually requires exit
		mlx_destroy_image(map->mlx, img_ptr); // Destroy the wrong-sized image
		exit_with_error(map, "Image size mismatch (required 32x32).");
	}*/
	return (img_ptr);
}

// Loads player-specific images
void	load_player_images(t_map *map)
{
	// Define paths - Adjust these to your actual asset locations
	map->img.player_right1 = load_single_image(map, "assets/right.xpm");
	map->img.player_right2 = load_single_image(map, "assets/right_2.xpm");
	map->img.player_left1 = load_single_image(map, "assets/left.xpm");
	map->img.player_left2 = load_single_image(map, "assets/left_2.xpm");
	map->img.player_up1 = load_single_image(map, "assets/front.xpm");
	map->img.player_up2 = load_single_image(map, "assets/front_2.xpm");
	map->img.player_down1 = load_single_image(map, "assets/back.xpm");
	map->img.player_down2 = load_single_image(map, "assets/back_2.xpm");
}

// Loads all necessary game images
void	load_game_images(t_map *map)
{
	// Define paths - Adjust these to your actual asset locations
	map->img.empty = load_single_image(map, "assets/grass.xpm");
	map->img.wall = load_single_image(map, "assets/wall.xpm");
	map->img.exit = load_single_image(map, "assets/exit.xpm");
	map->img.collectible = load_single_image(map, "assets/star.xpm");

	load_player_images(map); // Load all player sprites

	// Set the initial player sprite (e.g., facing down)
	map->img.current_player = map->img.player_down1;
}