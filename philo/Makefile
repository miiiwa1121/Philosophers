# --- Variables ---

# The name of the executable
NAME = philo

# The compiler
CC = cc

# Compiler flags: -Wall, -Wextra, -Werror are required by the subject
# -Iinclude tells the compiler to look for header files in the 'include' directory
# -pthread is necessary for compiling and linking thread functions
CFLAGS = -Wall -Wextra -Werror -Iinclude -pthread

# Directories
SRCS_DIR = src
OBJS_DIR = obj

# List of source files
SRCS_LIST = main.c \
            init.c \
            simulation.c \
            actions.c \
            monitor.c \
            utils.c

# Prepend source directory path to each source file
SRCS = $(addprefix $(SRCS_DIR)/, $(SRCS_LIST))

# Replace .c extension with .o for object files and place them in OBJS_DIR
OBJS = $(addprefix $(OBJS_DIR)/, $(SRCS_LIST:.c=.o))

# Command to remove files
RM = rm -f

# --- Rules ---

# The default rule, which is the first one
all: $(NAME)

# Rule to link object files into the final executable
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

# Rule to compile .c source files into .o object files
# Creates the object directory if it doesn't exist
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to create the object directory
$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

# Rule to remove object files
clean:
	$(RM) -r $(OBJS_DIR)

# Rule to remove object files and the executable
fclean: clean
	$(RM) $(NAME)

# Rule to re-compile the project from scratch
re: fclean all

# Specify rules that don't create files with the same name
.PHONY: all clean fclean re