#ifndef DEFINES_H
#define DEFINES_H

#include <SDL3/SDL.h>

# define DIGIT_HEIGHT	 64U
# define DIGIT_WIDTH	 32U
# define DIGIT_THICKNESS 4U

#define SQUARE_SIZE	   32U //cellsize of snake will be one smaller, leaving one pixel gap
#define STARTING_SPEED 500U
#define INPUT_SPEED	   16U

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
#define KEY_DROP   4U
#define KEYS	   5U

#define TERMINO_I	  0x01U
#define TERMINO_T	  0x02U
#define TERMINO_O	  0x03U
#define TERMINO_J	  0x04U
#define TERMINO_L	  0x05U
#define TERMINO_S	  0x06U
#define TERMINO_Z	  0x07U
#define TERMINO_BONUS 0x08U //fallen
#define TERMINO_ANY	  0xFFU

#define CLR_BLACK 0xFF000000U
#define CLR_WHITE 0xFFFFFFFFU

#define CLR_RED		  0xFFFF0000U
#define CLR_LIGHT_RED 0xFFFF6666U
#define CLR_DARK_RED  0xFF990000U

#define CLR_GREEN		0xFF00FF00U
#define CLR_LIGHT_GREEN 0xFF66FF66U
#define CLR_DARK_GREEN	0xFF009900U

#define CLR_BLUE	   0xFF0000FFU
#define CLR_LIGHT_BLUE 0xFF6666FFU
#define CLR_DARK_BLUE  0xFF000099U

#define CLR_YELLOW		 0xFFFFFF00U
#define CLR_LIGHT_YELLOW 0xFFFFFF66U
#define CLR_DARK_YELLOW	 0xFF999900U

#define CLR_CYAN	   0xFF00FFFFU
#define CLR_LIGHT_CYAN 0xFF66FFFFU
#define CLR_DARK_CYAN  0xFF009999U

#define CLR_MAGENTA		  0xFFFF00FFU
#define CLR_LIGHT_MAGENTA 0xFFFF66FFU
#define CLR_DARK_MAGENTA  0xFF990099U

#define CLR_ORANGE		 0xFFFFA500U
#define CLR_LIGHT_ORANGE 0xFFFFB347U
#define CLR_DARK_ORANGE	 0xFFCC8400U

#define CLR_PURPLE		 0xFF800080U
#define CLR_LIGHT_PURPLE 0xFFDDA0DDU
#define CLR_DARK_PURPLE	 0xFF4B0082U

#define CLR_BROWN		0xFF8B4513U
#define CLR_LIGHT_BROWN 0xFFCD853FU
#define CLR_DARK_BROWN	0xFF5C2E0FU

#define CLR_PINK	   0xFFFFC0CBU
#define CLR_LIGHT_PINK 0xFFFFB6C1U
#define CLR_DARK_PINK  0xFFC71585U

#define CLR_GRAY	   0xFF808080U
#define CLR_LIGHT_GRAY 0xFFD3D3D3U
#define CLR_DARK_GRAY  0xFF505050U

#define CLR_GRAY_1 0xFF222222U // Very Dark Gray
#define CLR_GRAY_2 0xFF444444U // Dark Gray
#define CLR_GRAY_3 0xFF666666U // Medium-Dark Gray
#define CLR_GRAY_4 0xFF888888U // Mid Gray
#define CLR_GRAY_5 0xFFAAAAAAU // Medium-Light Gray
#define CLR_GRAY_6 0xFFCCCCCCU // Light Gray
#define CLR_GRAY_7 0xFFEEEEEEU // Very Light Gray

#define CLR_GOLD   0xFFFFD700U
#define CLR_SILVER 0xFFC0C0C0U
#define CLR_BRONZE 0xFFCD7F32U

#define CLR_TEAL	   0xFF008080U
#define CLR_LIGHT_TEAL 0xFF66CCCCU
#define CLR_DARK_TEAL  0xFF004C4CU

#define CLR_NAVY	   0xFF000080U
#define CLR_LIGHT_NAVY 0xFF3366CCU
#define CLR_DARK_NAVY  0xFF00004CU

#define CLR_OLIVE		0xFF808000U
#define CLR_LIGHT_OLIVE 0xFFB5B35FU
#define CLR_DARK_OLIVE	0xFF5A5A00U

#define CLR_MAROON		 0xFF800000U
#define CLR_LIGHT_MAROON 0xFFB03060U
#define CLR_DARK_MAROON	 0xFF400000U

#define CLR_TURQUOISE		0xFF40E0D0U
#define CLR_LIGHT_TURQUOISE 0xFFAFEEEEU
#define CLR_DARK_TURQUOISE	0xFF008080U

#define CLR_TRANSPARENT 0x00000000U

typedef struct s_termino
{
	uint8_t type;
	uint8_t rot : 2; //0-3, bitfield
	int8_t	x;
	int8_t	y;
} t_termino;

typedef struct s_tetris
{
	t_termino termino;
	bool	running;
	bool	  key[KEYS];
	uint8_t	  queue[6]; // one falling and 6 in the queue
	uint8_t	  bag[7];	// 7 next pieces once the bag is empty, generate new one);
	uint32_t  bag_number;
	uint8_t	  next_termino_from_bag;
	uint8_t	  board[BOARD_HEIGHT][BOARD_WIDTH];
	uint32_t  next_move; //use in main loop
	uint32_t  next_tick; //use in main loop
	uint8_t  game_speed;
	uint32_t  score;
	uint32_t  level;
	uint32_t  lines;
}	t_tetris;

//char
typedef struct s_char
{
	uint8_t	 c;
	uint32_t width;
	uint32_t height;
	uint32_t pos_x;
	uint32_t pos_y;
	uint32_t colour;
	uint32_t background;
	//font
	//effect
} t_char;

typedef struct t_game
{
	uint32_t		fps;
	uint32_t	  seed;
	uint16_t	  termino[8][4];
	uint32_t	  colours[8];
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
	uint8_t		 digit[10][DIGIT_HEIGHT][DIGIT_WIDTH];
	uint32_t		frame;
} t_game;

void init_game(t_game* game);
void handle_input(t_game* game, bool* running);
void render(t_game* game);
void update_render(t_game* game);
void update_game_1(t_game* game);
void update_game_2(t_game* game);
void move_snake(t_game* game);
void cleanup(t_game* game);
void generateDigits(t_game *g);
void	printScore(t_game *g);

#endif
