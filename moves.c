#include "so_long.h"

// Checks the target tile type and updates game state if needed
// Returns 1 if the player won on this move.
// Returns -1 if the move is blocked (e.g., trying to exit with collectibles).
// Returns 0 otherwise (valid move onto EMPTY or COLLECTIBLE).
int	resolve_move_target(t_map *map, int nx, int ny)
{
	if (map->array[ny][nx] == EXIT)
	{
		if (map->c == 0)
			return (1); // Signal win condition met
		else
		{
			// Optional: Print only once or less frequently if annoying
			// write(1, "\rCollect all stars first!   \n", 30);
			// fflush(stdout);
			return (-1); // Signal move is blocked
		}
	}
	else if (map->array[ny][nx] == COLLECTIBLE)
	{
		map->array[ny][nx] = EMPTY; // Turn collectible tile into empty space
		map->c--;                   // Decrement collectible count
		return (0); // Signal normal move continuation
	}
	return (0); // Signal normal move continuation for EMPTY tile
}

// Redraws the necessary tiles after a player move
void	update_player_graphics(t_map *map, int old_x, int old_y, int new_x, int new_y)
{
	// Draw background/original tile at the old player position
	draw_tile(map, old_x * IMG_PXL, old_y * IMG_PXL);
	// Draw the player/updated tile at the new position
	draw_tile(map, new_x * IMG_PXL, new_y * IMG_PXL);
}

// Core logic for player movement
void	perform_move(t_map *map, int dx, int dy)
{
	int	old_x;
	int	old_y;
	int	new_x;
	int	new_y;
	int move_result;
	int	frame; // For animation

	if (map->exit_flag == 1) // Don't move if game already won
		return;
	old_x = map->sprite.x; old_y = map->sprite.y;
	new_x = old_x + dx; new_y = old_y + dy;
	// --- Pre-move Checks ---
	// Check Boundaries (redundant if map is properly walled)
	// if (new_y < 0 || new_y >= map->y || new_x < 0 || new_x >= map->x) return;
	// Check if target is a Wall
	if (map->array[new_y][new_x] == WALL)
		return;
	// --- Update Player Sprite ---
	// Simple 2-frame animation toggle: frame is 0 or 1
	frame = (map->moves % 4) / 2; // Changes every 2 moves (0,0,1,1,0,0...)
	if (dx > 0)
		map->img.current_player = (frame == 0) ? map->img.player_right1 : map->img.player_right2;
	else if (dx < 0)
		map->img.current_player = (frame == 0) ? map->img.player_left1 : map->img.player_left2;
	else if (dy > 0)
		map->img.current_player = (frame == 0) ? map->img.player_down1 : map->img.player_down2;
	else if (dy < 0)
		map->img.current_player = (frame == 0) ? map->img.player_up1 : map->img.player_up2;
	// --- Resolve Target Tile ---
	move_result = resolve_move_target(map, new_x, new_y);
	// --- Handle Blocked Move ---
	if (move_result == -1) // Move blocked (e.g., exit with C remaining)
	{
		draw_tile(map, old_x * IMG_PXL, old_y * IMG_PXL); // Redraw old tile (shows facing change)
		return;
	}
	// --- Execute Valid Move ---
	map->moves++; // Increment move counter *only* for valid moves/wins
	// Update Map Array
	map->array[old_y][old_x] = EMPTY; // Old position becomes empty
	if (move_result != 1) // Don't place 'P' on exit tile if winning move
		map->array[new_y][new_x] = PLAYER;
	// Update Player Coordinates
	map->sprite.x = new_x; map->sprite.y = new_y;
	// Update Terminal and Graphics
	print_moves_terminal(map); // Print moves first
	update_player_graphics(map, old_x, old_y, new_x, new_y); // Redraw tiles
	// Handle Win Condition
	if (move_result == 1)
		handle_win_condition(map);
}