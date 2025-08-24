#ifndef INTERFACE_H
#define INTERFACE_H
#pragma once

#include <stdint.h>

#define SNAKE_MIN_LENGTH 3

#define GAME_WIDTH	16U
#define GAME_HEIGHT 16U

#define WAIT_TIME_IN_SECONDS 5U
#define MS_IN_SECOND		 1000U
#define GAME_TIME_IN_SECONDS 10U //285
#define GAME_TICK 500U 

#define FOODS 4U

typedef enum
{
	GM_SERVER_STARTED,
	GM_REGISTRATION,
	GM_WAIT1,
	GM_PRACTICE,
	GM_WAIT2,
	GM_QUALIFICATION,
	GM_WAIT3,
	GM_ONE_VS_ONE,
	GM_WAIT4,
	GM_FINALS,
	GM_WAIT5,
	GM_RESULTS,
	GM_SIZE
} t_game_mode;

typedef enum
{
	MAP_EMPTY = 0x00U,
	MAP_SNAKE1 = 0x01U,
	MAP_SNAKE2 = 0x02U,
	MAP_SNAKE3 = 0x04U,
	MAP_SNAKE4 = 0x08U,
	MAP_SNAKE5 = 0x10U,
	MAP_SNAKE6 = 0x20U,
	MAP_SNAKE7 = 0x40U,
	MAP_SNAKE8 = 0x80U,
	MAP_SNAKE_HEAD = 0xFF00U,  //ff01
	MAP_SNAKE_BODY = 0x0000U,  //0101
	// MAP_SNAKE_TAIL = 0x0100U,  //0101 snake food + snake body (for later iteration of game)
	MAP_SNAKE1_FOOD = 0x0100U,
	MAP_SNAKE2_FOOD = 0x0200U,
	MAP_SNAKE3_FOOD = 0x0400U,
	MAP_SNAKE4_FOOD = 0x0800U,
	MAP_SNAKE5_FOOD = 0x1000U,
	MAP_SNAKE6_FOOD = 0x2000U,
	MAP_SNAKE7_FOOD = 0x4000U,
	MAP_SNAKE8_FOOD = 0x8000U,
	MAP_FOOD = 0xFF00U,
} t_cells;

typedef union
{
	struct
	{
		uint8_t dir : 2;
		uint8_t connected_server : 1;
		uint8_t connected_client : 1;
		uint8_t speed : 2;	//boost, normal, slow
		uint8_t unused : 2; // respawn blinking
	};
	uint8_t data;
} t_packet;

typedef struct
{
	uint16_t sd;
	uint16_t port;
	uint32_t ip;
	uint8_t	 segments[GAME_WIDTH * GAME_HEIGHT];
	uint8_t	 head;
	uint8_t	 length;
	uint8_t	 next_poop;
	t_packet status;
	uint32_t score;
	char	 name[9];
	char	 host[7];
	uint8_t	 game_id : 7; //0 for spectators
	uint8_t	 sending : 1;
	t_packet last_data;
	uint8_t	 ping_pong[4];
	uint8_t	 verification;
	uint64_t last_server_activity;
	uint64_t last_client_activity;
} t_snake;

typedef struct
{
	uint16_t map[GAME_WIDTH * GAME_HEIGHT];
	t_snake	 player[8];
	uint8_t	 players;
	uint64_t start_time;   // maybe?
	uint64_t end_time;	   // maybe?
	uint64_t current_time; // maybe?
	uint64_t time_left;
	uint64_t tick;
	uint8_t	 game_mode;
	uint8_t	 game_speed;
	char	 broadcast[256];
} t_game;

typedef enum
{
	NOLOG,
	FATAL,
	ERROR,
	WARNING,
	NOTICE,
	INFO,
	SUCCESS,
	TRACE,
	DEBUG1,
	DEBUG2,
	DEBUG3,
	DEBUG4,
	LOG_LEVELS
} t_log_level;

typedef enum
{
	NOT_VERIFIED,		// just connected
	SERVER_PING,		//42
	CLIENT_SENT_SECRET, //~42 + random
	SERVER_PONG,		// ~random + 42
	VERIFIED,
	REGISTERED,
	VERIFICATION_LEVELS
} t_verification;

typedef enum
{
	RIGHT,
	DOWN,
	LEFT,
	UP
} t_direction;


//map 0 empty space
// dead snake -> food
// slow -> slows shortening (every 32 ticks)
// boost -> shortened snake converts tail to food often
// every 16 moves snake poops
// boost 4 moves snake poops
// collision 2 moves snake poops
// boost + collision snake poops every move

#endif /*INTERFACE_H*/
