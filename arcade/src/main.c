#include "../include/defines.h"

int main(void)
{
	t_game game;
	uint32_t last_move_time = 0U;
	int	   running = 1;

	init_game(&game);
	uint32_t current_time = SDL_GetTicks();
	uint32_t end_time = current_time + 1000U * 60U;
	while (running && current_time < end_time)
	{
		handle_input(&game, &running);
		current_time = SDL_GetTicks();
		if (current_time - last_move_time >= STARTING_SPEED)  // 1000 ms = 1 second
		{
			move_snake(&game); // Call your function
			last_move_time = current_time;
		}
		render(&game);
		SDL_Delay(16); // ~60 FPS
	}

	cleanup(&game);
	return 0;
}
