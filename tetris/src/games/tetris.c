#include "../include/defines.h"
#include <time.h> //time for seed
#include <stdio.h> //debug printf
#include <stdlib.h>
#include <strings.h>

void shuffle_bag(uint8_t bag[7], uint32_t base_seed, uint32_t bag_number) {
    // Create a deterministic seed for this specific bag
    uint32_t seed = base_seed + bag_number;
    srand(seed);  // Initialize the RNG with the combined seed

    // Fill bag with pieces 1 to 7 (corresponding to TERMINO_I to TERMINO_Z)
    for (uint8_t i = 0; i < 7; ++i) {
        bag[i] = i + 1;
    }

    // Fisher-Yates shuffle
    for (uint8_t i = 6; i > 0; --i) {
        uint8_t j = rand() % (i + 1);
        uint8_t temp = bag[i];
        bag[i] = bag[j];
        bag[j] = temp;
    }
}

void init_game(t_game* game)
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_DisplayID		   display = SDL_GetPrimaryDisplay();
	const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(display);
	bzero(game, sizeof(t_game));
	game->screen_w = mode->w;
	game->screen_h = mode->h;
	game->tetris[PLAYER_1].running = true;
	game->tetris[PLAYER_2].running = true;
	game->window = SDL_CreateWindow("snake", game->screen_w, game->screen_h, SDL_WINDOW_FULLSCREEN);
	game->renderer = SDL_CreateRenderer(game->window, NULL);

	game->screen = malloc(game->screen_w * game->screen_h * sizeof(uint32_t));
	game->texture = SDL_CreateTexture(game->renderer,
									  SDL_PIXELFORMAT_ARGB8888, //why?
									  SDL_TEXTUREACCESS_STREAMING, game->screen_w, game->screen_h);
	game->offset_y0 = (game->screen_h - BOARD_HEIGHT * SQUARE_SIZE) / 2;
	game->offset_x0 = (game->screen_w / 2 - 50 - BOARD_WIDTH * SQUARE_SIZE);
	game->offset_y1 = (game->screen_h - BOARD_HEIGHT * SQUARE_SIZE) / 2;
	game->offset_x1 = (game->screen_w / 2 + 50);

	game->seed = time(NULL);
	shuffle_bag(game->tetris[PLAYER_1].bag, game->seed, game->tetris[PLAYER_1].bag_number);
	game->tetris[PLAYER_1].termino.type = game->tetris[PLAYER_1].bag[0];
	game->tetris[PLAYER_1].termino.x = 3;
	game->tetris[PLAYER_1].termino.y = 0;
	shuffle_bag(game->tetris[PLAYER_2].bag, game->seed, game->tetris[PLAYER_2].bag_number);
	game->tetris[PLAYER_2].termino.type = game->tetris[PLAYER_2].bag[0];
	game->tetris[PLAYER_2].termino.x = 3;
	game->tetris[PLAYER_2].termino.y = 0;
	game->colours[0] = CLR_MAGENTA;
	game->colours[TERMINO_I] = CLR_CYAN;
	game->colours[TERMINO_O] = CLR_YELLOW;
	game->colours[TERMINO_T] = CLR_PURPLE;
	game->colours[TERMINO_S] = CLR_GREEN;
	game->colours[TERMINO_Z] = CLR_RED;
	game->colours[TERMINO_J] = CLR_BLUE;
	game->colours[TERMINO_L] = CLR_ORANGE;
	game->termino[TERMINO_I][0] = 0x2222U;
	game->termino[TERMINO_I][1] = 0x0F00U;
	game->termino[TERMINO_I][2] = 0x2222U;
	game->termino[TERMINO_I][3] = 0x0F00U;
	game->termino[TERMINO_O][0] = 0x0660U;
	game->termino[TERMINO_O][1] = 0x0660U;
	game->termino[TERMINO_O][2] = 0x0660U;
	game->termino[TERMINO_O][3] = 0x0660U;
	game->termino[TERMINO_T][0] = 0x4E00U;
	game->termino[TERMINO_T][1] = 0x4640U;
	game->termino[TERMINO_T][2] = 0x0E40U;
	game->termino[TERMINO_T][3] = 0x4C40U;
	game->termino[TERMINO_J][0] = 0x8E00U;
	game->termino[TERMINO_J][1] = 0x6440U;
	game->termino[TERMINO_J][2] = 0x0E20U;
	game->termino[TERMINO_J][3] = 0x44C0U;
	game->termino[TERMINO_L][0] = 0x2E00U;
	game->termino[TERMINO_L][1] = 0x4460U;
	game->termino[TERMINO_L][2] = 0x0E80U;
	game->termino[TERMINO_L][3] = 0xC440U;
	game->termino[TERMINO_S][0] = 0x06C0U;
	game->termino[TERMINO_S][1] = 0x4620U;
	game->termino[TERMINO_S][2] = 0x06C0U;
	game->termino[TERMINO_S][3] = 0x4620U;
	game->termino[TERMINO_Z][0] = 0x0C60U;
	game->termino[TERMINO_Z][1] = 0x2640U;
	game->termino[TERMINO_Z][2] = 0x0C60U;
	game->termino[TERMINO_Z][3] = 0x2640U;

}

void handle_input(t_game* game, bool* running)
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_EVENT_QUIT)
		{
			*running = false;
		}
		else if (e.type == SDL_EVENT_KEY_DOWN)
		{
			switch (e.key.key)
			{
				case SDLK_ESCAPE:
					*running = false;
					break;
				case SDLK_UP:
					game->tetris[PLAYER_2].key[KEY_ROTATE] = true;
					break;
				case SDLK_DOWN:
					game->tetris[PLAYER_2].key[KEY_DOWN] = true;
					break;
				case SDLK_LEFT:
					game->tetris[PLAYER_2].key[KEY_LEFT] = true;
					break;
				case SDLK_RIGHT:
					game->tetris[PLAYER_2].key[KEY_RIGHT] = true;
					break;
				case SDLK_SPACE:
					game->tetris[PLAYER_2].key[KEY_DROP] = true;
					break;
				case SDLK_W:
					game->tetris[PLAYER_1].key[KEY_ROTATE] = true;
					break;
				case SDLK_S:
					game->tetris[PLAYER_1].key[KEY_DOWN] = true;
					break;
				case SDLK_A:
					game->tetris[PLAYER_1].key[KEY_LEFT] = true;
					break;
				case SDLK_D:
					game->tetris[PLAYER_1].key[KEY_RIGHT] = true;
					break;
				case SDLK_LCTRL:
					game->tetris[PLAYER_1].key[KEY_DROP] = true;
					break;
			}
		}
		else if (e.type == SDL_EVENT_KEY_UP)
		{
			switch (e.key.key)
			{
				case SDLK_ESCAPE:
					*running = 0;
					break;
				case SDLK_UP:
					game->tetris[PLAYER_2].key[KEY_ROTATE] = false;
					break;
				case SDLK_DOWN:
					game->tetris[PLAYER_2].key[KEY_DOWN] = false;
					break;
				case SDLK_LEFT:
					game->tetris[PLAYER_2].key[KEY_LEFT] = false;
					break;
				case SDLK_RIGHT:
					game->tetris[PLAYER_2].key[KEY_RIGHT] = false;
					break;
				case SDLK_SPACE:
					game->tetris[PLAYER_2].key[KEY_DROP] = false;
					break;
				case SDLK_W:
					game->tetris[PLAYER_1].key[KEY_ROTATE] = false;
					break;
				case SDLK_S:
					game->tetris[PLAYER_1].key[KEY_DOWN] = false;
					break;
				case SDLK_A:
					game->tetris[PLAYER_1].key[KEY_LEFT] = false;
					break;
				case SDLK_D:
					game->tetris[PLAYER_1].key[KEY_RIGHT] = false;
					break;
				case SDLK_LCTRL:
					game->tetris[PLAYER_1].key[KEY_DROP] = false;
					break;
			}
		}
	}
}

//render head
//hide tail (optional)
//render and generate food
//make it nicer with one pixel gaps but connected later

void clear_screen(t_game* game)
{
	for (uint32_t y = 0U; y < game->screen_h; y++)
	{
		for (uint32_t x = 0U; x < game->screen_w; x++)
		{
			game->screen[y * game->screen_w + x] = CLR_GRAY_2;
		}
	}
	
}

void render_boards(t_game* game)
{
	for (uint32_t y = 0U; y < BOARD_HEIGHT; y++)
	{
		for (uint32_t x = 0U; x < BOARD_WIDTH; x++)
		{
			uint32_t colour = 0xFF000000U;
			if (game->tetris[PLAYER_1].board[y][x])
				colour = game->colours[game->tetris[PLAYER_1].board[y][x]];
			for (uint32_t yy = 0U; yy < SQUARE_SIZE; yy++)
			{
				for (uint32_t xx = 0U; xx < SQUARE_SIZE; xx++)
				{
					game->screen[(y * SQUARE_SIZE + game->offset_y0 + yy) * game->screen_w
						+ x * SQUARE_SIZE + game->offset_x0 + xx] = colour;
				}
			}
		}
	}


	for (uint32_t y = 0U; y < BOARD_HEIGHT; y++)
	{
		for (uint32_t x = 0U; x < BOARD_WIDTH; x++)
		{
			uint32_t colour = 0xFF000000U;
			if (game->tetris[PLAYER_2].board[y][x])
			colour = game->colours[game->tetris[PLAYER_2].board[y][x]];
			for (uint32_t yy = 0U; yy < SQUARE_SIZE; yy++)
			{
				for (uint32_t xx = 0U; xx < SQUARE_SIZE; xx++)
				{
					game->screen[(y * SQUARE_SIZE + game->offset_y1 + yy) * game->screen_w
						+ x * SQUARE_SIZE + game->offset_x1 + xx] = colour;
				}
			}
		}
	}
}

void render_terminoes(t_game* game)
{
	for (uint32_t y = 0U; y < 4; y++)
	{
		for (uint32_t x = 0U; x < 4; x++)
		{
			uint32_t colour = game->colours[game->tetris[PLAYER_1].termino.type];
			if ((game->termino[game->tetris[PLAYER_1].termino.type][game->tetris[PLAYER_1].termino.rot] >> (y * 4 + x)) & 1)
			{
				// printf("here %u %u\n",y, x);
				for (uint32_t yy = 0U; yy < SQUARE_SIZE; yy++)
				{
					for (uint32_t xx = 0U; xx < SQUARE_SIZE; xx++)
					{
						if (yy == 0 || xx == 0)
							game->screen[((game->tetris[PLAYER_1].termino.y + y) * SQUARE_SIZE + game->offset_y0 + yy) * game->screen_w
								+ (game->tetris[PLAYER_1].termino.x + x) * SQUARE_SIZE + game->offset_x0 + xx] = 0xFF111111U;
							// printf("yx %u %u\n",((game->tetris[PLAYER_1].termino.y + y) * SQUARE_SIZE + game->offset_y0 + yy), (game->tetris[PLAYER_1].termino.x +x) * SQUARE_SIZE + game->offset_x0 + xx);
						else
							game->screen[((game->tetris[PLAYER_1].termino.y + y) * SQUARE_SIZE + game->offset_y0 + yy) * game->screen_w
								+ (game->tetris[PLAYER_1].termino.x + x) * SQUARE_SIZE + game->offset_x0 + xx] = colour;
					}
				}
			}
		}
	}
	for (uint32_t y = 0U; y < 4; y++)
	{
		for (uint32_t x = 0U; x < 4; x++)
		{
			uint32_t colour = game->colours[game->tetris[PLAYER_2].termino.type];
			if ((game->termino[game->tetris[PLAYER_2].termino.type][game->tetris[PLAYER_2].termino.rot] >> (y * 4 + x)) & 1)
			{
				// printf("here %u %u\n",y, x);
				for (uint32_t yy = 0U; yy < SQUARE_SIZE; yy++)
				{
					for (uint32_t xx = 0U; xx < SQUARE_SIZE; xx++)
					{
						if (yy == 0 || xx == 0)
							game->screen[((game->tetris[PLAYER_2].termino.y + y) * SQUARE_SIZE + game->offset_y1 + yy) * game->screen_w
								+ (game->tetris[PLAYER_2].termino.x + x) * SQUARE_SIZE + game->offset_x1 + xx] = 0xFF111111U;
							// printf("yx %u %u\n",((game->tetris[PLAYER_2].termino.y + y) * SQUARE_SIZE + game->offset_y0 + yy), (game->tetris[PLAYER_2].termino.x +x) * SQUARE_SIZE + game->offset_x0 + xx);
						else
							game->screen[((game->tetris[PLAYER_2].termino.y + y) * SQUARE_SIZE + game->offset_y1 + yy) * game->screen_w
								+ (game->tetris[PLAYER_2].termino.x + x) * SQUARE_SIZE + game->offset_x1 + xx] = colour;
					}
				}
			}
		}
	}
}

bool	is_move_valid(uint16_t termino, uint8_t board[BOARD_HEIGHT][BOARD_WIDTH], int8_t x, int8_t y)
{
	for (int8_t yy = 0; yy < 4; yy++)
	{
		for (int8_t xx = 0; xx < 4; xx++)
		{
			if ((termino >> (yy * 4 + xx)) & 1)
			{
				// printf("%i %i\n", y +yy, x + xx);
				if (y + yy >= (int8_t)BOARD_HEIGHT)
					return (false);
				if (x + xx < 0 || x + xx >= (int8_t)BOARD_WIDTH)
					return (false);
				if (y + yy >= 0 && board[y + yy][x + xx])
					return (false);
			}
		}
	}
	return (true);
}

bool	predrop_termino(t_game *game, t_termino *termino, uint8_t board[BOARD_HEIGHT][BOARD_WIDTH])
{
	bool	has_dropped = false;
	while (!has_dropped)
	{
		termino->y += 1;
		has_dropped = true;
		for (int8_t y = 0; y < 4; y++)
		{
			for (int8_t x = 0; x < 4; x++)
			{
				if ((game->termino[termino->type][0] >> (y * 4 + x)) & 1)
				{
					if (termino->y + y < 0)
						has_dropped = false;
					if (termino->y + y >= 0 && board[termino->y + y][termino->x + x])
					{
						termino->y -= 1;
						return (false);
					}
				}
			}
		}
	}
	return (true);
}

// check if possible to move

void	check_input(t_game *game)
{
	if (game->tetris[PLAYER_1].running == true)
	{
		if (game->tetris[PLAYER_1].key[KEY_ROTATE])
		{
			if (is_move_valid(game->termino[game->tetris[PLAYER_1].termino.type][(game->tetris[PLAYER_1].termino.rot + 1) % 4],
				game->tetris[PLAYER_1].board, game->tetris[PLAYER_1].termino.x, game->tetris[PLAYER_1].termino.y))
			{
				game->tetris[PLAYER_1].termino.rot += 1U;
				game->tetris[PLAYER_1].key[KEY_ROTATE] = false;
			}
		}
		if (game->tetris[PLAYER_1].key[KEY_DOWN])
		{
			if (is_move_valid(game->termino[game->tetris[PLAYER_1].termino.type][game->tetris[PLAYER_1].termino.rot],
				game->tetris[PLAYER_1].board, game->tetris[PLAYER_1].termino.x, game->tetris[PLAYER_1].termino.y + 1))
			{
				// game->tetris[PLAYER_1].key[KEY_DOWN] = false;
				game->tetris[PLAYER_1].termino.y += 1;
			}
		}
		if (game->tetris[PLAYER_1].key[KEY_LEFT])
		{
			if (is_move_valid(game->termino[game->tetris[PLAYER_1].termino.type][game->tetris[PLAYER_1].termino.rot],
				game->tetris[PLAYER_1].board, game->tetris[PLAYER_1].termino.x - 1, game->tetris[PLAYER_1].termino.y))
			{
				// game->tetris[PLAYER_1].key[KEY_LEFT] = false;
				game->tetris[PLAYER_1].termino.x -= 1;
			}
		}
		if (game->tetris[PLAYER_1].key[KEY_RIGHT])
		{
			if (is_move_valid(game->termino[game->tetris[PLAYER_1].termino.type][game->tetris[PLAYER_1].termino.rot],
				game->tetris[PLAYER_1].board, game->tetris[PLAYER_1].termino.x + 1, game->tetris[PLAYER_1].termino.y))
			{
				// game->tetris[PLAYER_1].key[KEY_RIGHT] = false;
				game->tetris[PLAYER_1].termino.x += 1;
			}
		}
		if (game->tetris[PLAYER_1].key[KEY_DROP])
		{
			while (is_move_valid(game->termino[game->tetris[PLAYER_1].termino.type][game->tetris[PLAYER_1].termino.rot],
				game->tetris[PLAYER_1].board, game->tetris[PLAYER_1].termino.x, game->tetris[PLAYER_1].termino.y + 1))
			{
				game->tetris[PLAYER_1].termino.y += 1;
			}
			game->tetris[PLAYER_1].key[KEY_DROP] = false;
		}
	}
	if (game->tetris[PLAYER_2].running == true)
	{
		if (game->tetris[PLAYER_2].key[KEY_ROTATE])
		{
			if (is_move_valid(game->termino[game->tetris[PLAYER_2].termino.type][(game->tetris[PLAYER_2].termino.rot + 1) % 4],
				game->tetris[PLAYER_2].board, game->tetris[PLAYER_2].termino.x, game->tetris[PLAYER_2].termino.y))
			{
				game->tetris[PLAYER_2].termino.rot += 1U;
				game->tetris[PLAYER_2].key[KEY_ROTATE] = false;
			}
		}
		if (game->tetris[PLAYER_2].key[KEY_DOWN])
		{
			if (is_move_valid(game->termino[game->tetris[PLAYER_2].termino.type][game->tetris[PLAYER_2].termino.rot],
				game->tetris[PLAYER_2].board, game->tetris[PLAYER_2].termino.x, game->tetris[PLAYER_2].termino.y + 1))
			{
				// game->tetris[PLAYER_2].key[KEY_DOWN] = false;
				game->tetris[PLAYER_2].termino.y += 1;
			}
		}
		if (game->tetris[PLAYER_2].key[KEY_LEFT])
		{
			if (is_move_valid(game->termino[game->tetris[PLAYER_2].termino.type][game->tetris[PLAYER_2].termino.rot],
				game->tetris[PLAYER_2].board, game->tetris[PLAYER_2].termino.x - 1, game->tetris[PLAYER_2].termino.y))
			{
				// game->tetris[PLAYER_2].key[KEY_LEFT] = false;
				game->tetris[PLAYER_2].termino.x -= 1;
			}
		}
		if (game->tetris[PLAYER_2].key[KEY_RIGHT])
		{
			if (is_move_valid(game->termino[game->tetris[PLAYER_2].termino.type][game->tetris[PLAYER_2].termino.rot],
				game->tetris[PLAYER_2].board, game->tetris[PLAYER_2].termino.x + 1, game->tetris[PLAYER_2].termino.y))
			{
				// game->tetris[PLAYER_2].key[KEY_RIGHT] = false;
				game->tetris[PLAYER_2].termino.x += 1;
			}
		}
		if (game->tetris[PLAYER_2].key[KEY_DROP])
		{
			while (is_move_valid(game->termino[game->tetris[PLAYER_2].termino.type][game->tetris[PLAYER_2].termino.rot],
				game->tetris[PLAYER_2].board, game->tetris[PLAYER_2].termino.x, game->tetris[PLAYER_2].termino.y + 1))
			{
				game->tetris[PLAYER_2].termino.y += 1;
			}
			game->tetris[PLAYER_2].key[KEY_DROP] = false;
		}
	}
}

void update_render(t_game* game)
{
	clear_screen(game);
	render_boards(game);
	//render boards
	check_input(game);
	render_terminoes(game);
	//render ghost?
	//render menu?
	//handle input
}

void render(t_game* game)
{
	SDL_UpdateTexture(game->texture, NULL, game->screen, game->screen_w * sizeof(uint32_t));
	SDL_RenderClear(game->renderer);
	SDL_RenderTexture(game->renderer, game->texture, NULL, NULL);
	SDL_RenderPresent(game->renderer);
}

void cleanup(t_game* game)
{
	// Destroy renderer and window
	if (game->renderer)
	{
		SDL_DestroyRenderer(game->renderer);
	}
	if (game->window)
	{
		SDL_DestroyWindow(game->window);
	}
	if (game->texture)
	{
		SDL_DestroyTexture(game->texture);
	}
	free(game->screen);
	// Quit SDL subsystems
	SDL_Quit();
}

uint32_t update_board(t_game *game, t_tetris *tetris)
{
	uint8_t fills;
	uint32_t	lines = 0;
	static const uint32_t score[5] = {0, 100, 300, 800, 2000};

	for (int8_t y = 0; y < 4; y++)
	{
		for (int8_t x = 0; x < 4; x++)
		{
			if ((game->termino[tetris->termino.type][tetris->termino.rot] >> (y * 4 + x)) & 1)
			{
				tetris->board[y + tetris->termino.y][x + tetris->termino.x] = tetris->termino.type;
			}
		}
	}

	//clear lines
	for (int8_t y = 0; y < (int)BOARD_HEIGHT; y++)
	{
		fills = 0;
		for (int8_t x = 0; x < (int)BOARD_WIDTH; x++)
		{
			if (tetris->board[y][x])
				fills += 1;
			else
				break ;
		}
		if (fills == BOARD_WIDTH)
		{
			lines++;
			for (int8_t yy = y; yy > 0; yy--)
			{
				for (int8_t x = 0; x < (int)BOARD_WIDTH; x++)
					tetris->board[yy][x] = tetris->board[yy - 1][x];
			}
			for (int8_t x = 0; x < (int)BOARD_WIDTH; x++)
				tetris->board[0][x] = 0U;
		}
	}
	tetris->lines += lines;
	tetris->level = tetris->lines / 10;
	return (score[lines]);
}


//handle game over here
void	init_termino(t_game *game, int player)
{
	game->tetris[player].next_termino_from_bag += 1;
	if (game->tetris[player].next_termino_from_bag == 7)
	{
		game->tetris[player].bag_number += 1;
		shuffle_bag(game->tetris[player].bag, game->seed, game->tetris[player].bag_number);
		game->tetris[player].next_termino_from_bag = 0;
		// game->running[player] = false;
	}
	game->tetris[player].termino.type = game->tetris[player].bag[game->tetris[player].next_termino_from_bag];
	game->tetris[player].termino.x = 3;
	game->tetris[player].termino.y = -4;
	game->tetris[player].termino.rot = 0U;
	if (!predrop_termino(game, &(game->tetris[player].termino), game->tetris[player].board))
	{
		game->tetris[player].running = false;
		update_board(game, &(game->tetris[player]));
	}
	// printf("%i \n", game->tetris[player].termino.y);
}


void update_game_1(t_game* game)
{
	/*update game logic here*/
	if (is_move_valid(game->termino[game->tetris[PLAYER_1].termino.type][game->tetris[PLAYER_1].termino.rot],
		game->tetris[PLAYER_1].board, game->tetris[PLAYER_1].termino.x, game->tetris[PLAYER_1].termino.y + 1))
		game->tetris[PLAYER_1].termino.y += 1;
	else
	{
		//write in board
		game->tetris[PLAYER_1].score += update_board(game, &(game->tetris[PLAYER_1]));
		//init new termino
		init_termino(game, PLAYER_1);
	}
}

void update_game_2(t_game* game)
{
	/*update game logic here*/
	if (is_move_valid(game->termino[game->tetris[PLAYER_2].termino.type][game->tetris[PLAYER_2].termino.rot],
		game->tetris[PLAYER_2].board, game->tetris[PLAYER_2].termino.x, game->tetris[PLAYER_2].termino.y + 1))
		game->tetris[PLAYER_2].termino.y += 1;
	else
	{
		//write in board
		game->tetris[PLAYER_2].score += update_board(game, &(game->tetris[PLAYER_2]));
		//init new termino
		init_termino(game, PLAYER_2);
	}
}