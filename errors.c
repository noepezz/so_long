#include "so_long.h"

// Frees a 2D char array safely
// Returns 1 for consistency, though return value isn't critical here
int free_str_array(char **array, int height)
{
	int i;

	if (!array)
		return (1);
	i = 0;
	// Use height hint, but also check for NULL pointer in the row
	while (i < height && array[i])
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	// Free the array of pointers itself
	free(array);
	return (1);
}

// Frees the main map array and the copy array if they exist
void	free_map_arrays(t_map *map)
{
	if (!map)
		return;
	// Use free_str_array which should handle NULL pointers safely
	if (map->array)
	{
		free_str_array(map->array, map->y);
		map->array = NULL; // Set to NULL after freeing
	}
	if (map->copy)
	{
		free_str_array(map->copy, map->y);
		map->copy = NULL; // Set to NULL after freeing
	}
}

// Generic error handling function: prints message, cleans up, exits
void	exit_with_error(t_map *map, char *message)
{
	write(2, "Error\n", 6);
	if (message)
	{
		// Use ft_strlen if available, otherwise fallback or include <string.h>
		write(2, message, ft_strlen(message));
		write(2, "\n", 1);
	}
	else
		write(2, "An unspecified error occurred.\n", 30);

	// Cleanup resources if map struct exists
	if (map)
	{
		free_map_arrays(map); // Free map arrays first

		// Attempt MLX cleanup only if mlx connection was established
		if (map->mlx)
		{
			destroy_images(map); // Destroy images if loaded
			if (map->wnd)
				mlx_destroy_window(map->mlx, map->wnd); // Destroy window if created
			// Usually no need to destroy display on Linux/macOS
			// if running X11 backend: mlx_destroy_display(map->mlx);
		}
		// We don't free(map) itself unless it was allocated on the heap
	}
	exit(EXIT_FAILURE);
}

// Exits using perror for system call errors (like file open)
// Prepends the context message to the system error message
void	exit_perror(char *context_message)
{
	write(2, "Error\n", 6); // Keep the "Error\n" prefix consistent
	perror(context_message); // perror prints context: OS error string
	exit(EXIT_FAILURE);
}