#ifndef SNAKE_SERVER_H
#define SNAKE_SERVER_H
#pragma once

#include "interface.h"

#include <stdint.h>

#define PORT		12345
#define MAX_CLIENTS 32
#define BUFFER_SIZE 1024
#define GAMES_COUNT 32

#define SECRET 42

typedef struct
{
	uint64_t game_modes[GM_SIZE];
	uint8_t	 active_games[GAMES_COUNT];
	t_game	 g[GAMES_COUNT];
	t_snake	 users[MAX_CLIENTS];
	uint64_t start_time;   // maybe?
	uint64_t end_time;	   // maybe?
	uint64_t current_time; // maybe?
	uint64_t time_left;
	uint64_t next_tick;
	uint64_t tick;
	uint8_t	 game_mode;

} t_server;

#endif /*SNAKE_SERVER_H*/
