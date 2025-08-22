#ifndef SNAKE_SERVER_H
# define SNAKE_SERVER_H
# pragma once


#include "interface.h"

#include <stdint.h>

#define PORT		12345
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define GAMES_COUNT 128


typedef struct
{
	uint16_t	sd;
	uint16_t	port;
	uint8_t		game_id:7;
	uint8_t		sending:1;
	t_packet	last_data;
} t_connection;

typedef struct
{
	uint64_t		game_modes[GM_SIZE];
	uint8_t			active_games[GAMES_COUNT];
	t_game			g[GAMES_COUNT];
	t_connection	users[MAX_CLIENTS];

} t_server;

#endif /*SNAKE_SERVER_H*/
