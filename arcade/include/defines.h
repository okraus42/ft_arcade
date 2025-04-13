#ifndef DEFINES_H
#define DEFINES_H

#include <SDL3/SDL.h>

#define SQUARE_SIZE 30U	//cellsize of snake will be one smaller, leaving one pixel gap
#define STARTING_SPEED 125U

#define UP 1U
#define DOWN 2U
#define VERTICAL 3U
#define LEFT 4U
#define RIGHT 8U
#define HORIZONTAL 12U

#define BOARD_WIDTH 32U
#define BOARD_HEIGHT 32U

#define PLAYER_1 0U
#define PLAYER_2 1U
#define PLAYERS 2U

#define FOODS 800U

#define SNAKE_1 1U
#define SNAKE_2 2U
#define FOOD 4U

typedef struct s_food {
	bool		is_placed;
	uint32_t	x;
	uint32_t	y;
} t_food;

typedef struct s_segment {
	uint32_t	x;
	uint32_t	y;
} t_segment;

typedef struct s_snake {
	t_segment	segment[BOARD_HEIGHT * BOARD_WIDTH];
	uint32_t	last_key;
	uint32_t	len;
	uint32_t	score;
	uint32_t	speed;	//maybe according to len or time elapsed
	uint32_t	colour;
    uint8_t		dir;
	uint8_t		lives;
	bool		has_eaten;
} t_snake;

typedef struct s_game {
	uint8_t	board[BOARD_HEIGHT][BOARD_WIDTH];
    SDL_Window *window;
    SDL_Renderer *renderer;
	SDL_Texture* texture;
    uint32_t	screen_w;
    uint32_t	screen_h;
	uint32_t	offset_y;
	uint32_t	offset_x;
	uint32_t	*screen;
	t_snake		snake[PLAYERS];
	t_food		food[FOODS];
} t_game;

void init_game(t_game *game);
void handle_input(t_game *game, int *running);
void render(t_game *game);
void move_snake(t_game *game);
void cleanup(t_game *game);

#endif
