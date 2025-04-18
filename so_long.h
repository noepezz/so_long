#ifndef SO_LONG_H
# define SO_LONG_H

// Standard Libraries
# include <unistd.h>    // write, close, read
# include <fcntl.h>     // open
# include <stdlib.h>    // exit, malloc, free
# include <stdio.h>     // perror, fflush
# include <string.h>    // strerror (for perror) - include if libft doesn't have it
# include <errno.h>     // errno (for perror)

// Custom Libraries
# include "libft/libft.h"        // Assumes libft provides ft_strlen, ft_strdup, ft_memset etc.
# include "libft/get_next_line.h"// Use your GNL header
# include "mlx/mlx.h"            // MinilibX graphics library

// --- Constants ---

// Key Codes (Linux X11 - Verify with tests if needed)
# define KEY_ESC        65307
# define KEY_W          119
# define KEY_A          97
# define KEY_S          115
# define KEY_D          100
# define KEY_UP         65362
# define KEY_DOWN       65364
# define KEY_LEFT       65361
# define KEY_RIGHT      65363

// Game Configuration
# define IMG_PXL      	50	// Pixel size of each tile
# define WND_NAME       "so_long" // Window Title

// Map Characters
# define WALL           '1'
# define EMPTY          '0'
# define COLLECTIBLE    'C'
# define PLAYER         'P'
# define EXIT           'E'
# define VISITED        'V' // For pathfinding flood fill

// --- Structures ---

// Structure for coordinates
typedef struct s_sprite
{
	int x; // Map grid column
	int y; // Map grid row
}               t_sprite;

// Structure to hold pointers to loaded images
typedef struct s_img
{
	void    *empty;
	void    *collectible;
	void    *wall;
	void    *exit;
	void    *player_left1;
	void    *player_right1;
	void    *player_up1;
	void    *player_down1;
	void    *player_left2;
	void    *player_right2;
	void    *player_up2;
	void    *player_down2;
	void    *current_player; // Pointer to the currently active player sprite
}               t_img;

// Main game state structure
typedef struct s_map
{
	// Map Data
	char        **array;     // The game map grid
	char        **copy;      // Copy used for pathfinding check
	char        *filename;   // Name of the map file
	int         y;           // Map height (rows)
	int         x;           // Map width (columns)

	// Game Elements Counts & State
	int         e;           // Initial exit count (should be 1)
	int         c;           // Current collectible count
	int         p;           // Initial player count (should be 1)
	int         c_check;     // Collectibles found during path check
	int         e_check;     // Exits reachable during path check
	int         exit_flag;   // Flag: 0 = playing, 1 = won
	int         moves;       // Player move counter

	// Graphics (MinilibX)
	void        *mlx;        // MLX connection instance
	void        *wnd;        // MLX window instance
	t_img       img;         // Struct holding image pointers

	// Player Info
	t_sprite    sprite;      // Player's current position (grid coords)

}               t_map;

// --- Function Prototypes ---

// File: check.c
void    check_filename_ext(t_map *map);
void    check_map_params(t_map *map);
void    check_map_walls(t_map *map);
void    check_map_dimensions(t_map *map);
void    check_map_path(t_map *map);

// File: errors.c
void    exit_with_error(t_map *map, char *message);
void    exit_perror(char *context_message);
void    free_map_arrays(t_map *map);
int     free_str_array(char **array, int height); // Moved prototype here

// File: exit.c
int     handle_close_event(t_map *map);
void    handle_win_condition(t_map *map);
void    destroy_images(t_map *map);

// File: img.c
void    load_game_images(t_map *map);
void    load_player_images(t_map *map);
void    *load_single_image(t_map *map, char *path);

// File: keys.c
void    find_player_start(t_map *map);
int     handle_key_press(int keycode, t_map *map);

// File: map.c
void    init_map_struct(t_map *map, char **av);
void    validate_map(t_map *map);
void    flood_fill_path(int x, int y, t_map *map);
void    read_map_file(t_map *map);

// File: moves.c
int     resolve_move_target(t_map *map, int nx, int ny);
void    update_player_graphics(t_map *map, int old_x, int old_y, int new_x, int new_y);
void    perform_move(t_map *map, int dx, int dy);

// File: print.c
void    print_moves_terminal(t_map *map);
void    draw_map_to_window(t_map *map);
void    draw_tile(t_map *map, int px_x, int px_y);

#endif // SO_LONG_H