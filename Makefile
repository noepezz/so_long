# Executable Name
NAME = so_long

# Compiler and Flags
CC = cc
# Add Include paths for headers (-I), Wall/Wextra/Werror for warnings
CFLAGS = -Wall -Wextra -Werror -I$(LIBFT_DIR) -I$(MLX_DIR) #-g # Uncomment -g for debugging

# Directories
MLX_DIR = ./mlx
LIBFT_DIR = ./libft

# Libraries (archive files)
MLX = $(MLX_DIR)/libmlx.a
LIBFT = $(LIBFT_DIR)/libft.a

# Source files (list all your .c files)
SRC = main.c check.c errors.c exit.c img.c keys.c map.c moves.c print.c

# Object files (derived from source files)
OBJ = $(SRC:.c=.o)

# Header file dependency
HDR = so_long.h

# Linker Flags (Linux X11)
# -L specifies library search paths, -l links specific libraries
# Order can matter: link dependencies (libft, mlx) before system libs
LDFLAGS = -L$(LIBFT_DIR) -lft -L$(MLX_DIR) -lmlx -lXext -lX11 -lm -lbsd

# --- Rules ---

# Default rule: build the executable
all: $(NAME)

# Rule to link the executable
# Depends on object files and the library archives
$(NAME): $(OBJ) $(LIBFT) $(MLX)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS)
	@echo "$(NAME) created successfully!"

# Rule to build libft (if it's not already built)
$(LIBFT):
	@echo "Making Libft..."
	@$(MAKE) -s -C $(LIBFT_DIR) # Use -s for silent make in libft

# Rule for MinilibX - Assumes libmlx.a exists or is handled externally
$(MLX):
	@# If MinilibX needs building via make: $(MAKE) -C $(MLX_DIR)
	@# Check if libmlx.a exists
	@test -f $(MLX) || (echo "Error: $(MLX) not found. Build MinilibX first." && exit 1)

# Rule to compile .c files into .o files
# $< is the source file (.c), $@ is the target file (.o)
# Depends on the source file and the main header file
%.o: %.c $(HDR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean object files and library intermediate files
clean:
	@echo "Cleaning object files..."
	@$(MAKE) -s -C $(LIBFT_DIR) clean
	@rm -f $(OBJ)

# Rule to clean everything (objects, libraries, executable)
fclean: clean
	@echo "Cleaning libraries and executable..."
	@$(MAKE) -s -C $(LIBFT_DIR) fclean
	@rm -f $(NAME)

# Rule to force rebuild (clean everything, then build all)
re: fclean all

# Declare rules that don't represent actual files
.PHONY: all clean fclean re $(LIBFT) $(MLX)