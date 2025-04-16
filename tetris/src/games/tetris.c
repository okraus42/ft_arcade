#include "../include/defines.h"
#include <stdio.h> //debug printf
#include <stdlib.h>
#include <strings.h>

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
					game->key[KEY_ROTATE] = true;
					break;
				case SDLK_DOWN:
					game->key[KEY_DOWN] = true;
					break;
				case SDLK_LEFT:
					game->key[KEY_LEFT] = true;
					break;
				case SDLK_RIGHT:
					game->key[KEY_RIGHT] = true;
					break;
				case SDLK_W:
					game->key[KEY_ROTATE] = true;
					break;
				case SDLK_S:
					game->key[KEY_DOWN] = true;
					break;
				case SDLK_A:
					game->key[KEY_LEFT] = true;
					break;
				case SDLK_D:
					game->key[KEY_RIGHT] = true;
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
					game->key[KEY_ROTATE] = false;
					break;
				case SDLK_DOWN:
					game->key[KEY_DOWN] = false;
					break;
				case SDLK_LEFT:
					game->key[KEY_LEFT] = false;
					break;
				case SDLK_RIGHT:
					game->key[KEY_RIGHT] = false;
					break;
				case SDLK_W:
					game->key[KEY_ROTATE] = false;
					break;
				case SDLK_S:
					game->key[KEY_DOWN] = false;
					break;
				case SDLK_A:
					game->key[KEY_LEFT] = false;
					break;
				case SDLK_D:
					game->key[KEY_RIGHT] = false;
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

void update_render(t_game* game)
{
	clear_screen(game);
	render_boards(game);
	//render boards
	//render terminoes
	//render ghost?
	//render menu?
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
	(void)game;
}