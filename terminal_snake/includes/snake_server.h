#ifndef SNAKE_SERVER_H
# define SNAKE_SERVER_H
# pragma once


#include "interface.h"

#include <stdint.h>

#define PORT		12345
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

typedef struct
{
	uint64_t	game_modes[GM_SIZE];
	t_game		g;
} t_server;

#endif /*SNAKE_SERVER_H*/