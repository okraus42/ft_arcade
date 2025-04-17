#ifndef DEFINES_H
#define DEFINES_H

#include <SDL3/SDL.h>

#define SQUARE_SIZE	   50U //cellsize of snake will be one smaller, leaving one pixel gap
#define STARTING_SPEED 500U
#define INPUT_SPEED	   125U

#define BOARD_WIDTH	  10U
#define BOARD_HEIGHT  20U
#define BOARD_PADDING 4U

#define PLAYER_1 0U
#define PLAYER_2 1U
#define PLAYERS	 2U

#define FOODS 800U

#define SNAKE_1 1U
#define SNAKE_2 2U
#define FOOD	4U

#define KEY_ROTATE 0U
#define KEY_LEFT   1U
#define KEY_RIGHT  2U
#define KEY_DOWN   3U
#define KEY_INSTA  4U
#define KEYS	   5U

#define	TERMINO_I		0x01U
#define	TERMINO_T		0x02U
#define	TERMINO_O		0x03U
#define	TERMINO_J		0x04U
#define	TERMINO_L		0x05U
#define	TERMINO_S		0x06U
#define	TERMINO_Z		0x07U
#define	TERMINO_BONUS	0x08U //fallen
#define	TERMINO_ANY		0xFFU


typedef struct s_termino
{
	uint8_t		type;
	uint8_t		rot:2;	//0-3, bitfield
	int8_t		x;
	int8_t		y;
} t_termino;

typedef struct s_tetris
{
	t_termino	termino;
	uint8_t		queue[6];	// one falling and 6 in the queue 
	uint8_t		bag[7];			// 7 next pieces once the bag is empty, generate new one);
	uint8_t		next_termino_from_bag;
	uint8_t		board[BOARD_HEIGHT][BOARD_WIDTH];
} t_tetris;

typedef struct s_game
{
	uint16_t		termino[8][4];
	uint32_t		colours[8];
	bool			key[KEYS];
	t_tetris	  tetris[PLAYERS];
	SDL_Window*	  window;
	SDL_Renderer* renderer;
	SDL_Texture*  texture;
	uint32_t	  screen_w;
	uint32_t	  screen_h;
	uint32_t	  offset_y0;
	uint32_t	  offset_x0;
	uint32_t	  offset_y1;
	uint32_t	  offset_x1;
	uint32_t*	  screen;
} t_game;

void init_game(t_game* game);
void handle_input(t_game* game, int* running);
void render(t_game* game);
void update_render(t_game* game);
void update_game(t_game* game);
void move_snake(t_game* game);
void cleanup(t_game* game);

#endif
