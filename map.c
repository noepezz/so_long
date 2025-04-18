#include "so_long.h"

// Initializes the t_map struct to default values
void	init_map_struct(t_map *map, char **av)
{
	if (!map)
		exit_with_error(NULL, "Failed to allocate map struct (internal error).");

	map->filename = av[1];
	map->moves = 0;
	map->e = 0;
	map->c = 0;
	map->p = 0;
	map->y = 0;
	map->x = 0;
	map->sprite.y = -1;
	map->sprite.x = -1;
	map->exit_flag = 0;
	map->c_check = 0;
	map->e_check = 0;
	map->array = NULL;
	map->copy = NULL;
	map->mlx = NULL;
	map->wnd = NULL;
	ft_memset(&map->img, 0, sizeof(t_img)); // Zero out image struct
}

#include "so_long.h"
#include <stdlib.h> // Ensure malloc/free are available

// Helper to trim trailing newline if present
static void trim_newline(char *line)
{
	int len;

	if (!line)
		return;
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
	{
		line[len - 1] = '\0'; // Replace newline with null terminator
	}
}

// Reads map file, removing newlines, into map->array and creates map->copy
void	read_map_file(t_map *map)
{
	int		fd;
	char	*line;
	int		current_y;
	//char	*temp_line; // Temporary storage for trimmed line

	// --- Stage 1: Determine map height (y) ---
	fd = open(map->filename, O_RDONLY);
	if (fd < 0)
		exit_perror("Failed to open map file (pass 1)");
	map->y = 0;
	while ((line = get_next_line(fd)) != NULL)
	{
		map->y++;
		free(line);
	}
	close(fd);
	if (map->y == 0)
		exit_with_error(map, "Map file is empty or could not be read.");

	// --- Stage 2: Allocate arrays and read content ---
	map->array = (char **)malloc(sizeof(char *) * (map->y + 1));
	map->copy = (char **)malloc(sizeof(char *) * (map->y + 1));
	if (!map->array || !map->copy)
	{
		free(map->array); free(map->copy);
		exit_with_error(map, "Memory allocation failed for map arrays.");
	}

	fd = open(map->filename, O_RDONLY);
	if (fd < 0)
	{
		free(map->array); free(map->copy);
		exit_perror("Failed to open map file (pass 2)");
	}

	current_y = 0;
	while ((line = get_next_line(fd)) != NULL)
	{
		if (current_y >= map->y) { free(line); break; } // Safety break

		// Trim the newline BEFORE duplicating
		trim_newline(line);

		// Now duplicate the line *without* the newline
		map->array[current_y] = ft_strdup(line);
		map->copy[current_y] = ft_strdup(line);

		// Check for allocation failure after strdup
		if (!map->array[current_y] || !map->copy[current_y])
		{
			free(line); // Free the original line from GNL
			map->array[current_y] = NULL; // Ensure null termination
			map->copy[current_y] = NULL;
			close(fd);
			free_map_arrays(map); // Use helper to free both arrays
			exit_with_error(map, "Memory allocation failed duplicating map line.");
		}
		free(line); // Free the original line from GNL *after* processing
		current_y++;
	}
	map->array[current_y] = NULL; // Null-terminate the arrays
	map->copy[current_y] = NULL;
	close(fd);

	// Final check: Did we read the expected number of lines?
	if (current_y != map->y)
	{
		free_map_arrays(map); // Free partially read arrays
		exit_with_error(map, "Map read error/mismatch on second pass.");
	}
}

// Recursive flood-fill for path checking (operates on map->copy)
void	flood_fill_path(int x, int y, t_map *map)
{
	// Base cases: Check boundaries and obstacles/visited cells in the copy
	if (y < 0 || y >= map->y || x < 0 || x >= map->x
		|| map->copy[y][x] == WALL || map->copy[y][x] == VISITED)
		return ;

	// Process current cell based on its type in the copy
	if (map->copy[y][x] == COLLECTIBLE)
		map->c_check -= 1; // Found a collectible
	else if (map->copy[y][x] == EXIT)
	{
		map->e_check -= 1; // Found the exit
		// Note: Don't return here, allow flood fill to continue
		// to check reachability of collectibles potentially "behind" exit
	}

	// Mark the current cell as visited in the copy
	map->copy[y][x] = VISITED;

	// Recursive calls for neighbors (4 directions)
	flood_fill_path(x + 1, y, map); // Right
	flood_fill_path(x - 1, y, map); // Left
	flood_fill_path(x, y + 1, map); // Down
	flood_fill_path(x, y - 1, map); // Up
}

// Orchestrates all map validation checks
void	validate_map(t_map *map)
{
	check_filename_ext(map);    // Check .ber extension

	read_map_file(map);         // Read into map->array and map->copy

	check_map_dimensions(map);  // Check rectangular, min size, set map->x
	check_map_params(map);      // Check chars, count P/E/C
	check_map_walls(map);       // Check surrounding walls
	check_map_path(map);        // Check path validity using flood fill
								// Frees map->copy if path is valid
}