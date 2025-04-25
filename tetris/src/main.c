#include "../include/defines.h"
#include <stdio.h>

int main(void)
{
	t_game game;
	uint32_t last_move_time = 0U;
	uint32_t last_render_time = 0U;
	bool running = true;
	init_game(&game);
	uint32_t current_time = SDL_GetTicks();
	while (running)
	{
		handle_input(&game, &running);
		current_time = SDL_GetTicks();
		if (current_time - last_move_time >= STARTING_SPEED)  // 1000 ms = 1 second
		{
			if (game.tetris[PLAYER_1].running)
				update_game_1(&game); // Call your function
			if (game.tetris[PLAYER_2].running)
				update_game_2(&game); // Call your function
			last_move_time = current_time;
		}
		if (current_time - last_render_time >= INPUT_SPEED)  // 1000 ms = 1 second
		{
			// if (game.running[PLAYER_1])
			update_render(&game); // Call your function
			last_render_time = current_time;
		}
		render(&game);
		if (running)
			running = game.tetris[PLAYER_1].running || game.tetris[PLAYER_2].running;
		SDL_Delay(16); // ~60 FPS
	}
	printf("Player 1: %8u %3u %4u\nPlayer 2: %8u %3u %4u\n", game.tetris[PLAYER_1].score, game.tetris[PLAYER_1].level, game.tetris[PLAYER_1].lines, game.tetris[PLAYER_2].score, game.tetris[PLAYER_2].level, game.tetris[PLAYER_2].lines);
	cleanup(&game);
	return 0;
}
