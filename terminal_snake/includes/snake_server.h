#ifndef SNAKE_SERVER_H
#define SNAKE_SERVER_H
#pragma once

#include "interface.h"

#include <stdint.h>

#define PORT		12345
#define MAX_CLIENTS 128
#define BUFFER_SIZE 1024
#define GAMES_COUNT 128

#define SECRET 42

typedef struct
{
	uint16_t sd;
	uint16_t port;
	uint32_t ip;
	char	 name[9];
	char	 host[7];
	uint8_t	 game_id : 7;
	uint8_t	 sending : 1;
	t_packet last_data;
	uint8_t	 ping_pong[4];
	uint8_t	 verification;
	uint32_t last_activity;
} t_connection;

typedef struct
{
	uint32_t	 current_time;
	uint64_t	 game_modes[GM_SIZE];
	uint8_t		 active_games[GAMES_COUNT];
	t_game		 g[GAMES_COUNT];
	t_connection users[MAX_CLIENTS];

} t_server;

#endif /*SNAKE_SERVER_H*/
