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
	shuffle_bag(game->tetris[0].bag, game->seed, game->tetris[0].bag_number);
	game->tetris[0].termino.type = game->tetris[0].bag[0];
	game->tetris[0].termino.x = 3;
	game->tetris[0].termino.y = 0;
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

void handle_input(t_game* game, int* running)
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_EVENT_QUIT)
		{
			*running = 0;
		}
		else if (e.type == SDL_EVENT_KEY_DOWN)
		{
			switch (e.key.key)
			{
				case SDLK_ESCAPE:
					*running = 0;
					break;
				case SDLK_UP:
					game->tetris[PLAYER_1].key[KEY_ROTATE] = true;
					break;
				case SDLK_DOWN:
					game->tetris[PLAYER_1].key[KEY_DOWN] = true;
					break;
				case SDLK_LEFT:
					game->tetris[PLAYER_1].key[KEY_LEFT] = true;
					break;
				case SDLK_RIGHT:
					game->tetris[PLAYER_1].key[KEY_RIGHT] = true;
					break;
				case SDLK_SPACE:
					game->tetris[PLAYER_1].key[KEY_DROP] = true;
					break;
				case SDLK_W:
					game->tetris[PLAYER_2].key[KEY_ROTATE] = true;
					break;
				case SDLK_S:
					game->tetris[PLAYER_2].key[KEY_DOWN] = true;
					break;
				case SDLK_A:
					game->tetris[PLAYER_2].key[KEY_LEFT] = true;
					break;
				case SDLK_D:
					game->tetris[PLAYER_2].key[KEY_RIGHT] = true;
					break;
				case SDLK_LCTRL:
					game->tetris[PLAYER_2].key[KEY_DROP] = true;
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
					game->tetris[PLAYER_1].key[KEY_ROTATE] = false;
					break;
				case SDLK_DOWN:
					game->tetris[PLAYER_1].key[KEY_DOWN] = false;
					break;
				case SDLK_LEFT:
					game->tetris[PLAYER_1].key[KEY_LEFT] = false;
					break;
				case SDLK_RIGHT:
					game->tetris[PLAYER_1].key[KEY_RIGHT] = false;
					break;
				case SDLK_SPACE:
					game->tetris[PLAYER_1].key[KEY_DROP] = false;
					break;
				case SDLK_W:
					game->tetris[PLAYER_2].key[KEY_ROTATE] = false;
					break;
				case SDLK_S:
					game->tetris[PLAYER_2].key[KEY_DOWN] = false;
					break;
				case SDLK_A:
					game->tetris[PLAYER_2].key[KEY_LEFT] = false;
					break;
				case SDLK_D:
					game->tetris[PLAYER_2].key[KEY_RIGHT] = false;
					break;
				case SDLK_LCTRL:
					game->tetris[PLAYER_2].key[KEY_DROP] = false;
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
			game->screen[y * game->screen_w + x] = 0xFF11FF11U;
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
			if (game->tetris[0].board[y][x])
				colour = 0xFFFFFFFFU;
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
			if (game->tetris[1].board[y][x])
				colour = 0xFFFFFFFFU;
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
			uint32_t colour = 0xFFFF00FFU;
			if ((game->termino[game->tetris[0].termino.type][game->tetris[0].termino.rot] >> (y * 4 + x)) & 1)
			{
				// printf("here %u %u\n",y, x);
				for (uint32_t yy = 0U; yy < SQUARE_SIZE; yy++)
				{
					for (uint32_t xx = 0U; xx < SQUARE_SIZE; xx++)
					{
						if (yy == 0 || xx == 0)
							game->screen[((game->tetris[0].termino.y + y) * SQUARE_SIZE + game->offset_y0 + yy) * game->screen_w
								+ (game->tetris[0].termino.x + x) * SQUARE_SIZE + game->offset_x0 + xx] = 0xFF111111U;
							// printf("yx %u %u\n",((game->tetris[0].termino.y + y) * SQUARE_SIZE + game->offset_y0 + yy), (game->tetris[0].termino.x +x) * SQUARE_SIZE + game->offset_x0 + xx);
						else
							game->screen[((game->tetris[0].termino.y + y) * SQUARE_SIZE + game->offset_y0 + yy) * game->screen_w
								+ (game->tetris[0].termino.x + x) * SQUARE_SIZE + game->offset_x0 + xx] = colour;
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
				printf("%i %i\n", y +yy, x + xx);
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

// check if possible to move

void	check_input(t_game *game)
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

void update_game(t_game* game)
{
	/*update game logic here*/
	if (is_move_valid(game->termino[game->tetris[PLAYER_1].termino.type][game->tetris[PLAYER_1].termino.rot],
		game->tetris[PLAYER_1].board, game->tetris[PLAYER_1].termino.x, game->tetris[PLAYER_1].termino.y + 1))
		game->tetris[0].termino.y += 1;
}