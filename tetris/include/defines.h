#ifndef DEFINES_H
#define DEFINES_H

#include <SDL3/SDL.h>

# define DIGIT_HEIGHT	 64U
# define DIGIT_WIDTH	 32U
# define DIGIT_THICKNESS 4U

#define SQUARE_SIZE	   32U
#define STARTING_SPEED 500U
#define INPUT_SPEED	   16U

#define BOARD_WIDTH	  10U
#define BOARD_HEIGHT  20U

// Enum for Player IDs
enum Player {
    PLAYER_1 = 0U,
    PLAYER_2 = 1U,
    PLAYERS  = 2U
};

// Enum for Key IDs
enum Key {
    KEY_ROTATE = 0U,
    KEY_LEFT   = 1U,
    KEY_RIGHT  = 2U,
    KEY_DOWN   = 3U,
    KEY_DROP   = 4U,
    KEYS       = 5U
};

// Enum for Tetrimino types
enum Tetrimino {
    TERMINO_I    = 0x01U,
    TERMINO_T    = 0x02U,
    TERMINO_O    = 0x03U,
    TERMINO_J    = 0x04U,
    TERMINO_L    = 0x05U,
    TERMINO_S    = 0x06U,
    TERMINO_Z    = 0x07U,
    TERMINO_BONUS = 0x08U, // fallen
    TERMINO_ANY  = 0xFFU
};

typedef enum {
    CLR_TRANSPARENT = 0x00000000U,
    CLR_BLACK       = 0xFF000000U,
    CLR_WHITE       = 0xFFFFFFFFU,

    // ==== GRAY ====
    CLR_GRAY        = 0xFF808080U,
    CLR_LIGHT_GRAY  = 0xFFD3D3D3U,
    CLR_DARK_GRAY   = 0xFF505050U,

    CLR_GRAY_1 = 0xFFEEEEEEU,
    CLR_GRAY_2 = 0xFFCCCCCCU,
    CLR_GRAY_3 = 0xFFAAAAAAU,
    CLR_GRAY_4 = 0xFF888888U,
    CLR_GRAY_5 = 0xFF666666U,
    CLR_GRAY_6 = 0xFF444444U,
    CLR_GRAY_7 = 0xFF222222U,

    // ==== RED ====
    CLR_RED        = 0xFFFF0000U,
    CLR_LIGHT_RED  = 0xFFFF6666U,
    CLR_DARK_RED   = 0xFF990000U,

    CLR_RED_1 = 0xFFFFCCCCU,
    CLR_RED_2 = 0xFFFF9999U,
    CLR_RED_3 = 0xFFFF6666U,
    CLR_RED_4 = 0xFFFF0000U,
    CLR_RED_5 = 0xFFCC0000U,
    CLR_RED_6 = 0xFF990000U,
    CLR_RED_7 = 0xFF660000U,

    // ==== GREEN ====
    CLR_GREEN        = 0xFF00FF00U,
    CLR_LIGHT_GREEN  = 0xFF66FF66U,
    CLR_DARK_GREEN   = 0xFF009900U,

    CLR_GREEN_1 = 0xFFCCFFCCU,
    CLR_GREEN_2 = 0xFF99FF99U,
    CLR_GREEN_3 = 0xFF66FF66U,
    CLR_GREEN_4 = 0xFF00FF00U,
    CLR_GREEN_5 = 0xFF00CC00U,
    CLR_GREEN_6 = 0xFF009900U,
    CLR_GREEN_7 = 0xFF006600U,

    // ==== BLUE ====
    CLR_BLUE        = 0xFF0000FFU,
    CLR_LIGHT_BLUE  = 0xFF6666FFU,
    CLR_DARK_BLUE   = 0xFF000099U,

    CLR_BLUE_1 = 0xFFCCCCFFU,
    CLR_BLUE_2 = 0xFF9999FFU,
    CLR_BLUE_3 = 0xFF6666FFU,
    CLR_BLUE_4 = 0xFF0000FFU,
    CLR_BLUE_5 = 0xFF0000CCU,
    CLR_BLUE_6 = 0xFF000099U,
    CLR_BLUE_7 = 0xFF000066U,

    // ==== YELLOW ====
    CLR_YELLOW        = 0xFFFFFF00U,
    CLR_LIGHT_YELLOW  = 0xFFFFFF66U,
    CLR_DARK_YELLOW   = 0xFF999900U,

    CLR_YELLOW_1 = 0xFFFFFFCCU,
    CLR_YELLOW_2 = 0xFFFFFF99U,
    CLR_YELLOW_3 = 0xFFFFFF66U,
    CLR_YELLOW_4 = 0xFFFFFF00U,
    CLR_YELLOW_5 = 0xFFCCCC00U,
    CLR_YELLOW_6 = 0xFF999900U,
    CLR_YELLOW_7 = 0xFF666600U,

    // ==== CYAN ====
    CLR_CYAN        = 0xFF00FFFFU,
    CLR_LIGHT_CYAN  = 0xFF66FFFFU,
    CLR_DARK_CYAN   = 0xFF009999U,

    CLR_CYAN_1 = 0xFFCCFFFFU,
    CLR_CYAN_2 = 0xFF99FFFFU,
    CLR_CYAN_3 = 0xFF66FFFFU,
    CLR_CYAN_4 = 0xFF00FFFFU,
    CLR_CYAN_5 = 0xFF00CCCCU,
    CLR_CYAN_6 = 0xFF009999U,
    CLR_CYAN_7 = 0xFF006666U,

    // ==== MAGENTA ====
    CLR_MAGENTA        = 0xFFFF00FFU,
    CLR_LIGHT_MAGENTA  = 0xFFFF66FFU,
    CLR_DARK_MAGENTA   = 0xFF990099U,

    CLR_MAGENTA_1 = 0xFFFFCCFFU,
    CLR_MAGENTA_2 = 0xFFFF99FFU,
    CLR_MAGENTA_3 = 0xFFFF66FFU,
    CLR_MAGENTA_4 = 0xFFFF00FFU,
    CLR_MAGENTA_5 = 0xFFCC00CCU,
    CLR_MAGENTA_6 = 0xFF990099U,
    CLR_MAGENTA_7 = 0xFF660066U,

    // ==== ORANGE ====
    CLR_ORANGE        = 0xFFFFA500U,
    CLR_LIGHT_ORANGE  = 0xFFFFB347U,
    CLR_DARK_ORANGE   = 0xFFCC8400U,

    CLR_ORANGE_1 = 0xFFFFE5CCU,
    CLR_ORANGE_2 = 0xFFFFCC99U,
    CLR_ORANGE_3 = 0xFFFFB347U,
    CLR_ORANGE_4 = 0xFFFFA500U,
    CLR_ORANGE_5 = 0xFFCC8400U,
    CLR_ORANGE_6 = 0xFF996300U,
    CLR_ORANGE_7 = 0xFF664200U,

    // ==== PURPLE ====
    CLR_PURPLE        = 0xFF800080U,
    CLR_LIGHT_PURPLE  = 0xFFDDA0DDU,
    CLR_DARK_PURPLE   = 0xFF4B0082U,

    CLR_PURPLE_1 = 0xFFE6CCFFU,
    CLR_PURPLE_2 = 0xFFD1A3FFU,
    CLR_PURPLE_3 = 0xFFDDA0DDU,
    CLR_PURPLE_4 = 0xFF800080U,
    CLR_PURPLE_5 = 0xFF660066U,
    CLR_PURPLE_6 = 0xFF4B0082U,
    CLR_PURPLE_7 = 0xFF2E0033U,

    // ==== BROWN ====
    CLR_BROWN        = 0xFF8B4513U,
    CLR_LIGHT_BROWN  = 0xFFCD853FU,
    CLR_DARK_BROWN   = 0xFF5C2E0FU,

    CLR_BROWN_1 = 0xFFFFE0CCU,
    CLR_BROWN_2 = 0xFFD2B48CU,
    CLR_BROWN_3 = 0xFFCD853FU,
    CLR_BROWN_4 = 0xFF8B4513U,
    CLR_BROWN_5 = 0xFF5C2E0FU,
    CLR_BROWN_6 = 0xFF3E1E0AU,
    CLR_BROWN_7 = 0xFF2B1506U,

    // ==== PINK ====
    CLR_PINK        = 0xFFFFC0CBU,
    CLR_LIGHT_PINK  = 0xFFFFB6C1U,
    CLR_DARK_PINK   = 0xFFC71585U,

    CLR_PINK_1 = 0xFFFFE6F0U,
    CLR_PINK_2 = 0xFFFFD6E8U,
    CLR_PINK_3 = 0xFFFFB6C1U,
    CLR_PINK_4 = 0xFFFFC0CBU,
    CLR_PINK_5 = 0xFFFF69B4U,
    CLR_PINK_6 = 0xFFF08080U,
    CLR_PINK_7 = 0xFFC71585U,

    // ==== OTHER COLORS ====
    CLR_GOLD   = 0xFFFFD700U,
    CLR_SILVER = 0xFFC0C0C0U,
    CLR_BRONZE = 0xFFCD7F32U,

    CLR_TEAL       = 0xFF008080U,
    CLR_LIGHT_TEAL = 0xFF66CCCCU,
    CLR_DARK_TEAL  = 0xFF004C4CU,

    CLR_NAVY       = 0xFF000080U,
    CLR_LIGHT_NAVY = 0xFF3366CCU,
    CLR_DARK_NAVY  = 0xFF00004CU,

    CLR_OLIVE       = 0xFF808000U,
    CLR_LIGHT_OLIVE = 0xFFB5B35FU,
    CLR_DARK_OLIVE  = 0xFF5A5A00U,

    CLR_MAROON       = 0xFF800000U,
    CLR_LIGHT_MAROON = 0xFFB03060U,
    CLR_DARK_MAROON  = 0xFF400000U

} ColorEnum;
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
