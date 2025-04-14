#ifndef DEFINES_H
#define DEFINES_H

#include <SDL3/SDL.h>

#define SQUARE_SIZE 30U	//cellsize of snake will be one smaller, leaving one pixel gap
#define STARTING_SPEED 125U
#define INPUT_SPEED 125U

#define UP 1U
#define DOWN 2U
#define VERTICAL 3U
#define LEFT 4U
#define RIGHT 8U
#define HORIZONTAL 12U

#define BOARD_WIDTH 10U
#define BOARD_HEIGHT 20U
#define BOARD_PADDING 4U

#define PLAYER_1 0U
#define PLAYER_2 1U
#define PLAYERS 2U

#define FOODS 800U

#define SNAKE_1 1U
#define SNAKE_2 2U
#define FOOD 4U

#define KEY_ROTATE 0U
#define KEY_LEFT 1U
#define KEY_RIGHT 2U
#define KEY_DOWN 3U
#define KEY_INSTA 4U
#define KEYS 5U

typedef struct s_player {
	uint32_t	key[KEYS];
} t_player;


typedef struct s_game {
	uint8_t	board[PLAYERS][BOARD_HEIGHT][BOARD_WIDTH];
	t_player	player[PLAYERS];
    SDL_Window *window;
    SDL_Renderer *renderer;
	SDL_Texture* texture;
    uint32_t	screen_w;
    uint32_t	screen_h;
	uint32_t	offset_y;
	uint32_t	offset_x;
	uint32_t	*screen;
} t_game;

void init_game(t_game *game);
void handle_input(t_game *game, int *running);
void render(t_game *game);
void update_render(t_game *game);
void update_game(t_game *game);
void move_snake(t_game *game);
void cleanup(t_game *game);

#endif
