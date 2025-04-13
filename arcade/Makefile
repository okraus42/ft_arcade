# Program name
NAME        := ft_arcade

# Compiler and flags
CC          := cc
CFLAGS      := -Wall -Wextra -Werror -g -gdwarf-4
RM          := rm -rf

# Directories
SRC_DIR     := src
OBJ_DIR     := obj
INCLUDE_DIR := include
SDL_DIR     := SDL
SDL_REPO    := https://github.com/libsdl-org/SDL.git
SDL_LIB     := $(SDL_DIR)/build/.libs/libSDL3.a

# Sources and objects
SRCS        :=	main.c \
				games/snake.c
OBJS        := $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
DEPS        := $(OBJS:.o=.d)

# SDL flags
SDL_CFLAGS  := -I$(SDL_DIR)/include -I$(INCLUDE_DIR)
SDL_LDFLAGS := -L$(SDL_DIR)/build -lSDL3

# Build rules
all: $(NAME)

$(NAME): $(SDL_LIB) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(SDL_LDFLAGS) -lm -o $(NAME)

$(SDL_LIB):
	@if [ ! -d "$(SDL_DIR)" ]; then \
		echo "Cloning SDL3..."; \
		git clone $(SDL_REPO) $(SDL_DIR); \
	fi
	@if [ ! -d "$(SDL_DIR)/build" ]; then \
		echo "Building SDL3..."; \
		mkdir -p $(SDL_DIR)/build; \
		cd $(SDL_DIR)/build && cmake .. -DSDL_STATIC=ON -DSDL_SHARED=OFF -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DCMAKE_BUILD_TYPE=Release; \
		cmake --build . --config Release; \
	fi

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -MMD -MP -c $< -o $@

clean:
	$(RM) $(OBJ_DIR)
	@make -C $(SDL_DIR) clean || true

fclean: clean
	$(RM) $(NAME)
	$(RM) -r $(SDL_DIR)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re
