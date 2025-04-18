#include "so_long.h"

// Check file extension
void	check_filename_ext(t_map *map)
{
	size_t	len;
	char	*extension;

	if (!map || !map->filename)
		exit_with_error(map, "Map structure or filename missing.");
	len = ft_strlen(map->filename);
	if (len < 4) // Minimum length for "a.ber"
		exit_with_error(map, "Invalid filename (too short).");
	extension = map->filename + len - 4;
	if (ft_strncmp(extension, ".ber", 5) != 0) // Use ft_strncmp
		exit_with_error(map, "Filename must end with .ber extension.");
}

// Check if map dimensions are valid (rectangular, >= 3x3) and set map->x
void	check_map_dimensions(t_map *map)
{
	int	y;
	int len_first_row;

	if (map->y <= 0 || !map->array || !map->array[0])
		exit_with_error(map, "Map array is invalid or empty after read.");
	len_first_row = ft_strlen(map->array[0]);
	if (len_first_row <= 0)
		exit_with_error(map, "Map row has zero length.");

	map->x = len_first_row; // Set map width based on the first row

	// Check minimum size (must be at least 3x3 to have content within walls)
	if (map->x < 3 || map->y < 3)
		exit_with_error(map, "Map is too small (must be at least 3x3).");

	y = 1;
	while (y < map->y)
	{
		if (!map->array[y] || (int)ft_strlen(map->array[y]) != map->x)
			exit_with_error(map, "Map must be rectangular.");
		y++;
	}
}

// In check.c

void    check_map_params(t_map *map)
{
	int		y;
	int		x;
	char	current_char; // Use a temporary variable for clarity

	map->c = 0;
	map->e = 0;
	map->p = 0; // Reset counts
	y = 0;
	while (y < map->y)
	{
		x = 0;
		while (x < map->x)
		{
			current_char = map->array[y][x]; // Get current char
			if (current_char == COLLECTIBLE)
				map->c++;
			else if (current_char == EXIT)
				map->e++;
			else if (current_char == PLAYER)
				map->p++;
			// ** CORRECTED CHECK **
			// Only check for invalid characters if it wasn't C, E, or P
			else if (current_char != EMPTY && current_char != WALL)
			{
				// If we reach here, it means the character was NOT C, E, P,
				// AND it's also not EMPTY or WALL. It's truly invalid.
				exit_with_error(map, "Invalid character found in map.");
			}
			x++;
		}
		y++;
	}
	// --- Check counts after iterating (This part is likely fine) ---
	if (map->c < 1)
		exit_with_error(map, "Map must contain at least one collectible ('C').");
	if (map->e != 1)
		exit_with_error(map, "Map must contain exactly one exit ('E').");
	if (map->p != 1)
		exit_with_error(map, "Map must contain exactly one player start ('P').");
}

// Check if the map is surrounded by walls
void	check_map_walls(t_map *map)
{
	int	x;
	int	y;

	// Check top wall (row 0) and bottom wall (row map->y - 1)
	x = 0;
	while (x < map->x)
	{
		if (map->array[0][x] != WALL || map->array[map->y - 1][x] != WALL)
			exit_with_error(map, "Map must be surrounded by walls ('1').");
		x++;
	}
	// Check side walls (col 0 and map->x - 1 for rows 1 to map->y - 2)
	y = 1;
	while (y < map->y - 1)
	{
		if (map->array[y][0] != WALL || map->array[y][map->x - 1] != WALL)
			exit_with_error(map, "Map must be surrounded by walls ('1').");
		y++;
	}
}

// Checks if there's a valid path from player start to all collectibles and exit
void	check_map_path(t_map *map)
{
	if (!map->copy) // Should have been created by read_map_file
		exit_with_error(map, "Map copy missing for path checking.");

	find_player_start(map); // Sets map->sprite.x and map->sprite.y

	// Initialize check counters with total counts found in check_map_params
	map->c_check = map->c;
	map->e_check = map->e; // Should be 1

	// Perform flood fill on the MAP COPY starting from player position
	flood_fill_path(map->sprite.x, map->sprite.y, map);

	// Verify if all collectibles and the exit were reached (counts decremented to 0)
	if (map->c_check != 0 || map->e_check != 0)
	{
		// Error: Not all required items are reachable
		// map->copy will be freed by exit_with_error called below
		exit_with_error(map, "No valid path to all collectibles and/or exit.");
	}

	// Path is valid. Free the copy map as it's no longer needed.
	free_str_array(map->copy, map->y);
	map->copy = NULL; // Set pointer to NULL
}