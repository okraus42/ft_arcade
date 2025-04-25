#include "../include/defines.h"
#include <stdio.h>

int main(void)
{
	t_game game;
	bool running = true;
	init_game(&game);
	uint32_t current_time = SDL_GetTicks();
	while (running)
	{
		handle_input(&game, &running);
		current_time = SDL_GetTicks();
		(void)current_time;
		if (game.tetris[PLAYER_1].running && game.frame >= game.tetris[PLAYER_1].next_move)
			update_game_1(&game); // Call your function
		if (game.tetris[PLAYER_2].running && game.frame >= game.tetris[PLAYER_2].next_move)
			update_game_2(&game); // Call your function
		update_render(&game); // Call your function
		render(&game);
		if (running)
			running = game.tetris[PLAYER_1].running || game.tetris[PLAYER_2].running;
		SDL_Delay(16); // ~60 FPS
		game.frame += 1;
	}
	printf("Player 1: %8u %3u %4u\nPlayer 2: %8u %3u %4u\n", game.tetris[PLAYER_1].score, game.tetris[PLAYER_1].level, game.tetris[PLAYER_1].lines, game.tetris[PLAYER_2].score, game.tetris[PLAYER_2].level, game.tetris[PLAYER_2].lines);
	cleanup(&game);
	return 0;
}
