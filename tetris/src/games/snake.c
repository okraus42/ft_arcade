#include "../include/defines.h"
#include <stdlib.h>
#include <strings.h>
#include <stdio.h> //debug printf

void	place_foods(t_game *game)
{
	uint32_t free = 0U;

	for (uint32_t y = 0U; y < BOARD_HEIGHT; y++)
	{
		for (uint32_t x = 0U; x < BOARD_WIDTH; x++)
		{
			if (game->board[y][x] == 0U)
				free++;
		}
	}
	for (uint32_t f = 0; f < FOODS; f++)
	{
		if (game->food[f].is_placed == false && free != 0U)
		{
			uint32_t	p = 0U;
			uint32_t	r = rand() % free;
			for (uint32_t y = 0U; y < BOARD_HEIGHT; y++)
			{
				for (uint32_t x = 0U; x < BOARD_WIDTH; x++)
				{
					if (game->board[y][x] == 0U)
					{
						p++;
						if (p == r)
						{
							game->food[f].is_placed = true;
							game->food[f].y = y;
							game->food[f].x = x;
							game->board[y][x] |= FOOD;
						}
					}
				}
			}
		}
	}
}

void	fill_board(t_game *game)
{
	bzero(game->board, BOARD_HEIGHT * BOARD_WIDTH * sizeof(uint8_t));
	for (uint32_t p = 0; p < PLAYERS; p++)
	{
		for (uint32_t s = 0; s < game->snake[p].len; s++)
		{
			game->board[game->snake[p].segment[s].y][game->snake[p].segment[s].x] |= p + 1;
		}
	}
	for (uint32_t f = 0; f < FOODS; f++)
	{
		if (game->food[f].is_placed)
			game->board[game->food[f].y][game->food[f].x] |= FOOD;
	}
}

void init_game(t_game *game) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_DisplayID display = SDL_GetPrimaryDisplay();
    const SDL_DisplayMode *mode = SDL_GetCurrentDisplayMode(display);
	bzero(game, sizeof(t_game));
    game->screen_w = mode->w;
    game->screen_h = mode->h;

    game->window = SDL_CreateWindow("snake", game->screen_w, game->screen_h, SDL_WINDOW_FULLSCREEN);
    game->renderer = SDL_CreateRenderer(game->window, NULL);


	game->screen = malloc(game->screen_w * game->screen_h * sizeof(uint32_t));
	game->texture = SDL_CreateTexture(
		game->renderer,
		SDL_PIXELFORMAT_ARGB8888, //why?
		SDL_TEXTUREACCESS_STREAMING,
		game->screen_w,
		game->screen_h
	);
	game->offset_y = (game->screen_h - BOARD_HEIGHT * SQUARE_SIZE) / 2;
	game->offset_x = (game->screen_w - BOARD_WIDTH * SQUARE_SIZE) / 2;
	game->snake[0].dir = RIGHT;
	game->snake[0].len = 4U;
	game->snake[0].colour = 0xFF0000FFU;
	for (uint32_t i = 0U; i < game->snake[0].len; i++)
	{
		game->snake[0].segment[i].x = 9 - i;
		game->snake[0].segment[i].y = 16;
	}
	game->snake[1].dir = LEFT;
	game->snake[1].len = 4U;
	game->snake[1].colour = 0xFFFF0000U;
	for (uint32_t i = 0U; i < game->snake[1].len; i++)
	{
		game->snake[1].segment[i].x = BOARD_WIDTH - 10 + i;
		game->snake[1].segment[i].y = 16;
	}
	fill_board(game);
	place_foods(game);
}

void handle_input(t_game *game, int *running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) {
            *running = 0;
        } else if (e.type == SDL_EVENT_KEY_DOWN) {
            switch (e.key.key) {
                case SDLK_ESCAPE: *running = 0; break;
                case SDLK_UP: game->snake[0].last_key = UP; break;
                case SDLK_DOWN: game->snake[0].last_key = DOWN; break;
                case SDLK_LEFT: game->snake[0].last_key = LEFT; break;
                case SDLK_RIGHT: game->snake[0].last_key = RIGHT; break;
				case SDLK_W: game->snake[1].last_key = UP; break;
                case SDLK_S: game->snake[1].last_key = DOWN; break;
                case SDLK_A: game->snake[1].last_key = LEFT; break;
                case SDLK_D: game->snake[1].last_key = RIGHT; break;
            }
        }
    }
}

//render head
//hide tail (optional)
//render and generate food
//make it nicer with one pixel gaps but connected later

void	render_food(t_game *game)
{
	for (uint32_t f = 0U; f < FOODS; f++)
	{
		if (game->food[f].is_placed == false)
			continue ;
		uint32_t	start_y = game->food[f].y * SQUARE_SIZE + 1;
		uint32_t	start_x = game->food[f].x * SQUARE_SIZE + 1;
		uint32_t	end_x = (game->food[f].x * SQUARE_SIZE) + SQUARE_SIZE - 1;
		uint32_t	end_y = (game->food[f].y * SQUARE_SIZE) + SQUARE_SIZE - 1;

		for (uint32_t y = start_y; y < end_y; y++)
		{
			for (uint32_t x = start_x; x < end_x; x++)
			{
				if ((y + x) & 1)
					game->screen[((y + game->offset_y) * game->screen_w + x + game->offset_x)] |= 0xFFFFFFFFU; //use food colour here
			}
		}
	}
}

void	render_snake_segment(t_game *game, uint32_t snake, uint32_t segment)
{
	// printf("%u, %u %u %u\n", snake, segment, game->snake[snake].segment[segment].x, game->snake[snake].segment[segment].y);
	uint32_t	start_y = game->snake[snake].segment[segment].y * SQUARE_SIZE + 1;
	uint32_t	start_x = game->snake[snake].segment[segment].x * SQUARE_SIZE + 1;
	uint32_t	end_x = (game->snake[snake].segment[segment].x * SQUARE_SIZE) + SQUARE_SIZE - 1;
	uint32_t	end_y = (game->snake[snake].segment[segment].y * SQUARE_SIZE) + SQUARE_SIZE - 1;

	if ((segment > 0U
			&& game->snake[snake].segment[segment - 1].x == game->snake[snake].segment[segment].x - 1)
		|| (segment < game->snake[snake].len - 1
			&& game->snake[snake].segment[segment + 1].x == game->snake[snake].segment[segment].x - 1))
		--start_x;
	if ((segment > 0U
			&& game->snake[snake].segment[segment - 1].x == game->snake[snake].segment[segment].x + 1)
		|| (segment < game->snake[snake].len - 1
				&& game->snake[snake].segment[segment + 1].x == game->snake[snake].segment[segment].x + 1))
		++end_x;
	if ((segment > 0U
			&& game->snake[snake].segment[segment - 1].y == game->snake[snake].segment[segment].y - 1)
		|| (segment < game->snake[snake].len - 1
			&& game->snake[snake].segment[segment + 1].y == game->snake[snake].segment[segment].y - 1))
		--start_y;
	if ((segment > 0U
			&& game->snake[snake].segment[segment - 1].y == game->snake[snake].segment[segment].y + 1)
		|| (segment < game->snake[snake].len - 1
				&& game->snake[snake].segment[segment + 1].y == game->snake[snake].segment[segment].y + 1))
		++end_y;
	for (uint32_t y = start_y; y < end_y; y++)
	{
		for (uint32_t x = start_x; x < end_x; x++)
		{
			game->screen[((y + game->offset_y) * game->screen_w + x + game->offset_x)] |= game->snake[snake].colour; //use snake colour here
		}
	}
}

void	clear_screen(t_game *game)
{
	for (uint32_t y = 0U; y < game->screen_h; y++)
	{
		for (uint32_t x = 0U; x < game->screen_w; x++)
		{
			game->screen[y * game->screen_w + x] = 0xFF111111U;
		}
	}
	for (uint32_t y = game->offset_y; y < game->screen_h - game->offset_y; y++)
	{
		for (uint32_t x = game->offset_x; x < game->screen_w - game->offset_x; x++)
		{
			game->screen[y * game->screen_w + x] = 0x0U;
		}
	}
}

void	render_snake(t_game *game, int snake)
{
	for (uint32_t i = 0U; i < game->snake[snake].len; i++)
	{
		render_snake_segment(game, snake, i);
	}
}

void	render_snake_head(t_game *game, int snake)
{
	render_snake_segment(game, snake, 0);
}

// void	hide_snake_tail(t_game *game, int snake)
// {
// 	if (game->snake[snake].has_eaten)
// 		game->snake[snake].has_eaten = false;
// 	else
// 	{
		
// 	}
// }

void render(t_game *game) {
    // SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
	clear_screen(game);
	render_snake(game, 0U);
	render_snake(game, 1U);
	render_food(game);
	SDL_UpdateTexture(game->texture, NULL, game->screen, game->screen_w * sizeof(uint32_t));
    SDL_RenderClear(game->renderer); // really needed

	SDL_RenderTexture(game->renderer, game->texture, NULL, NULL);
    // SDL_SetRenderDrawColor(game->renderer, 0, 0, 255, 255);
    // SDL_FRect frect = {
    //     .x = (float)game->square.x,
    //     .y = (float)game->square.y,
    //     .w = (float)game->square.w,
    //     .h = (float)game->square.h
    // };
    // SDL_RenderFillRect(game->renderer, &frect);

    SDL_RenderPresent(game->renderer);
}

void cleanup(t_game *game) {
    // Destroy renderer and window
    if (game->renderer) {
        SDL_DestroyRenderer(game->renderer);
    }
    if (game->window) {
        SDL_DestroyWindow(game->window);
    }
	if (game->texture) {
        SDL_DestroyTexture(game->texture);
    }
	free(game->screen);
    printf("Player 0 len: %u\nPlayer 1 len: %u\n", game->snake[0].len, game->snake[1].len);
    // Quit SDL subsystems
    SDL_Quit();
}

void move_snake(t_game *game)
{
	bool	is_coliding = false;
	// printf("len %u dir: %u\n", game->snake[0].len, game->snake[0].dir);
	for (uint32_t p = 0U; p < PLAYERS; p++)
	{
		is_coliding = false;
		uint8_t	previous_dir = game->snake[p].dir;
		uint32_t	y = game->snake[p].segment[0].y;
		uint32_t	x = game->snake[p].segment[0].x;
		switch (game->snake[p].last_key) {
			case UP:
				if (game->snake[p].dir & HORIZONTAL)
					game->snake[p].dir = UP;
				break;
			case DOWN:
				if (game->snake[p].dir & HORIZONTAL)
					game->snake[p].dir = DOWN;
				break;
			case LEFT:
				if (game->snake[p].dir & VERTICAL)
					game->snake[p].dir = LEFT;
				break;
			case RIGHT:
				if (game->snake[p].dir & VERTICAL)
					game->snake[p].dir = RIGHT;
				break;
			default:
				break;
		}
		switch (game->snake[p].dir) {
			case UP: y = (y - 1) & (BOARD_WIDTH - 1); break;
			case DOWN: y = (y + 1) & (BOARD_WIDTH - 1); break;
			case LEFT: x = (x - 1) & (BOARD_WIDTH - 1); break;
			case RIGHT: x = (x + 1) & (BOARD_WIDTH - 1); break;
			default: break;
		}
		//check new y and x and either move the snake or shorten it
		for (uint32_t i = game->snake[p].len; i > 0U; i--)
		{
			if (game->snake[p].segment[i].y == y && game->snake[p].segment[i].x == x)
			{
				game->snake[p].len -= 1;
				game->snake[p].dir = previous_dir;
				is_coliding = true;
			}
		}
		if (is_coliding)
			continue ;
		for (uint32_t i = game->snake[p].len; i > 0U; i--)
		{
			game->snake[p].segment[i].y = game->snake[p].segment[i - 1].y;
			game->snake[p].segment[i].x = game->snake[p].segment[i - 1].x;
		}
		game->snake[p].segment[0].y = y;
		game->snake[p].segment[0].x = x;
	}
	for (uint32_t f = 0U; f < FOODS; f++)
	{
		if (game->food[f].is_placed)
		{
			if (game->snake[0].segment[0].y == game->food[f].y && game->snake[0].segment[0].x == game->food[f].x)
			{
				game->snake[0].segment[game->snake[0].len].y = game->snake[0].segment[game->snake[0].len - 1].y;
				game->snake[0].segment[game->snake[0].len].x = game->snake[0].segment[game->snake[0].len - 1].x;
				game->snake[0].len += 1;
				game->food[f].is_placed = false;
			}
			if (game->snake[1].segment[0].y == game->food[f].y && game->snake[1].segment[0].x == game->food[f].x)
			{
				game->snake[1].segment[game->snake[1].len].y = game->snake[1].segment[game->snake[1].len - 1].y;
				game->snake[1].segment[game->snake[1].len].x = game->snake[1].segment[game->snake[1].len - 1].x;
				game->snake[1].len += 1;
				game->food[f].is_placed = false;
			}
		}
	}
	// printf("A: len %u dir: %u\n", game->snake[0].len, game->snake[0].dir);
	fill_board(game);
	place_foods(game);
}