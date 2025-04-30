#include "so_long.h"

// Frees a 2D char array safely
int free_str_array(char **array, int height)
{
	int i;

	if (!array)
		return (1);
	i = 0;
	while (i < height && array[i])
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
	return (1);
}

// Frees the main map array and the copy array if they exist
void	free_map_arrays(t_map *map)
{
	if (!map)
		return;
	if (map->array)
	{
		free_str_array(map->array, map->y);
		map->array = NULL;
	}
	if (map->copy)
	{
		free_str_array(map->copy, map->y);
		map->copy = NULL;
	}
}

// Generic error handling function: prints message, cleans up, exits
void	exit_with_error(t_map *map, char *message)
{
	write(2, "Error\n", 6);
	if (message)
	{
		write(2, message, ft_strlen(message));
		write(2, "\n", 1);
	}
	else
		write(2, "An unspecified error occurred.\n", 30);
	if (map)
	{
		free_map_arrays(map);
		if (map->mlx)
		{
			destroy_images(map);
			if (map->wnd)
				mlx_destroy_window(map->mlx, map->wnd); 
		}
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