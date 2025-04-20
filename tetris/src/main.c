#include "../include/defines.h"

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
			update_game(&game); // Call your function
			last_move_time = current_time;
		}
		if (current_time - last_render_time >= INPUT_SPEED)  // 1000 ms = 1 second
		{
			update_render(&game); // Call your function
			last_render_time = current_time;
		}
		render(&game);
		SDL_Delay(16); // ~60 FPS
		if (running)
			running = game.running[PLAYER_1];
	}

	cleanup(&game);
	return 0;
}
