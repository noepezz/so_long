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
		trim_newline(line);
		map->array[current_y] = ft_strdup(line);
		map->copy[current_y] = ft_strdup(line);
		if (!map->array[current_y] || !map->copy[current_y])
		{
			free(line);
			map->array[current_y] = NULL;
			map->copy[current_y] = NULL;
			close(fd);
			free_map_arrays(map);
			exit_with_error(map, "Memory allocation failed duplicating map line.");
		}
		free(line);
		current_y++;
	}
	map->array[current_y] = NULL;
	map->copy[current_y] = NULL;
	close(fd);
	if (current_y != map->y)
	{
		free_map_arrays(map);
		exit_with_error(map, "Map read error/mismatch on second pass.");
	}
}

// Recursive flood-fill for path checking (operates on map->copy)
void	flood_fill_path(int x, int y, t_map *map)
{

	if (y < 0 || y >= map->y || x < 0 || x >= map->x
		|| map->copy[y][x] == WALL || map->copy[y][x] == VISITED)
		return ;
	if (map->copy[y][x] == COLLECTIBLE)
		map->c_check -= 1;
	else if (map->copy[y][x] == EXIT)
		map->e_check -= 1;
	map->copy[y][x] = VISITED;
	flood_fill_path(x + 1, y, map);
	flood_fill_path(x - 1, y, map);
	flood_fill_path(x, y + 1, map);
	flood_fill_path(x, y - 1, map);
}

// Orchestrates all map validation checks
void	validate_map(t_map *map)
{
	check_filename_ext(map);
	read_map_file(map);
	check_map_dimensions(map);
	check_map_params(map);
	check_map_walls(map);
	check_map_path(map);
}