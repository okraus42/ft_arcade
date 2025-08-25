
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

#include <signal.h>
#include <sys/stat.h>
#include <termios.h>

#include "interface.h"
#include "logger.h"
#include "snake_client.h"
#include "timer.h"

#define SERVER_IP	"10.11.8.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

#define SECRET 84

static struct termios g_saved_termios; // saved terminal settings

void disableRawMode()
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &g_saved_termios);
	fflush(stdout);
	write(STDOUT_FILENO, EXIT_ALT_SCREEN, sizeof(EXIT_ALT_SCREEN) - 1);
	write(STDOUT_FILENO,
		  SHOW_CURSOR,
		  sizeof(SHOW_CURSOR) - 1); // <-- ensures cursor shown
}

void ft_exit(int num)
{
	disableRawMode();
	dprintf(2, "Server not available\n");
	exit(num);
}

void handleExit(int sig)
{
	disableRawMode();
	if (sig != 0)
	{
		write(STDERR_FILENO, "\n[Exited due to signal]\n", 25);
		exit(1);
	}
}

// Enhanced raw mode setup
void enableRawMode()
{
	struct termios raw;

	// Get and save current terminal settings
	tcgetattr(STDIN_FILENO, &g_saved_termios);
	raw = g_saved_termios;

	// Register cleanup handlers
	signal(SIGINT, handleExit);
	signal(SIGTERM, handleExit);
	signal(SIGQUIT, handleExit);
	signal(SIGHUP, handleExit);

	// Modify flags to enable true raw mode
	raw.c_lflag &= ~(ICANON | ECHO); // character-wise input, no echo
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
	fflush(stdout);
	write(STDOUT_FILENO, ENTER_ALT_SCREEN, sizeof(ENTER_ALT_SCREEN) - 1);

	    // Set stdin non-blocking
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}

// Read direction: 0=right, 1=down, 2=left, 3=up, keep old_dir if invalid
int read_direction(int old_dir)
{
	char c;
	int	 new_dir = old_dir;

	if (read(STDIN_FILENO, &c, 1) < 0)
		return old_dir;

	// Handle arrow keys (3-byte sequence: ESC [ A/B/C/D)
	if (c == '\033')
	{
		char seq[2];
		if (read(STDIN_FILENO, &seq[0], 1) < 0)
			return old_dir;
		if (read(STDIN_FILENO, &seq[1], 1) < 0)
			return old_dir;

		if (seq[0] == '[')
		{
			switch (seq[1])
			{
				case 'C':
					new_dir = 0;
					break; // right
				case 'B':
					new_dir = 1;
					break; // down
				case 'D':
					new_dir = 2;
					break; // left
				case 'A':
					new_dir = 3;
					break; // up
			}
		}
	}
	else
	{
		// Handle WASD
		switch (c)
		{
			case 'd':
			case 'D':
				new_dir = 0;
				break; // right
			case 's':
			case 'S':
				new_dir = 1;
				break; // down
			case 'a':
			case 'A':
				new_dir = 2;
				break; // left
			case 'w':
			case 'W':
				new_dir = 3;
				break; // up
		}
	}

	// Prevent reversing direction: if opposite, ignore input
	if (((new_dir + 2) & 0x3) == old_dir)
	{
		// Opposite direction, keep old one
		return old_dir;
	}

	return new_dir;
}

int check_dir(t_game* g, t_packet *packet)
{
	int old_dir = g->player[0].last_data.dir;
	int new_dir = read_direction(old_dir);
	if (new_dir != old_dir)
	{
		packet->dir = new_dir;
		return 1;
	}
	return 0;
}

void place_snake(t_game* g, int y, int x)
{
	uint8_t pos = y * 16 + x;
	if (g->map[pos] == (MAP_SNAKE1 | MAP_SNAKE_HEAD))
		printf(SNAKE1_COLOR SNAKE_HEAD COLOR_RESET);
	else if (g->map[pos] == MAP_SNAKE1)
		printf(SNAKE1_COLOR SNAKE_BODY COLOR_RESET);
	else if (g->map[pos] == MAP_FOOD)
		printf(FOOD_COLOR FOOD COLOR_RESET);
	else
		printf(BACKGROUND_COLOR "  " COLOR_RESET);
}
// GM_SERVER_STARTED,
// GM_REGISTRATION,
// GM_WAIT1,
// GM_PRACTICE,
// GM_WAIT2,
// GM_QUALIFICATION,
// GM_WAIT3,
// GM_ONE_VS_ONE,
// GM_WAIT4,
void print_wait(t_game* g, char* mode)
{
	int y = 0;
	int x = 0;
	printf("╔");
	for (x = 1; x < 33; x++)
		printf("═");
	printf("╦");
	for (x = 34; x < SCREEN_WIDTH - 1; x++)
		printf("═");
	printf("╗"
		   "\n");
	y = 0;
	printf("║");
	for (x = 0; x < 16; x++)
		place_snake(g, y, x);
	printf("║");
	printf("      NAME |    SCORE | LENGTH | SPEED       ");
	printf("║"
		   "\n");
	++y; // 1
	printf("║");
	for (x = 0; x < 16; x++)
		place_snake(g, y, x);
	printf("║");
	printf("  %8.8s |%9i |%7i |%6i       ",
		   g->player[0].name,
		   g->player[0].score,
		   g->player[0].length,
		   g->player[0].last_data.speed);
	printf("║"
		   "\n");
	++y; // 2
	printf("║");
	for (x = 0; x < 16; x++)
		place_snake(g, y, x);
	printf("║");
	printf("2                                            ");
	printf("║"
		   "\n");
	++y; // 3
	printf("║");
	for (x = 0; x < 16; x++)
		place_snake(g, y, x);
	printf("║");
	printf("3                                            ");
	printf("║"
		   "\n");
	++y; // 4
	printf("║");
	for (x = 0; x < 16; x++)
		place_snake(g, y, x);
	printf("║");
	printf("4                                            ");
	printf("║"
		   "\n");
	++y; //5
	printf("║");
	for (x = 0; x < 16; x++)
		place_snake(g, y, x);
	printf("║");
	printf("5                                            ");
	printf("║"
		   "\n");
	++y; //6
	printf("║");
	for (x = 0; x < 16; x++)
		place_snake(g, y, x);
	printf("║");
	printf("6                                            ");
	printf("║"
		   "\n");
	++y; // 7
	printf("║");
	for (x = 0; x < 16; x++)
		place_snake(g, y, x);
	printf("║");
	printf("7                                            ");
	printf("║"
		   "\n");
	++y; //8
	printf("║");
	for (x = 0; x < 16; x++)
		place_snake(g, y, x);
	printf("║");
	printf("8                                            ");
	printf("║"
		   "\n");
	++y; //9
	printf("║");
	for (x = 0; x < 16; x++)
		place_snake(g, y, x);
	printf("╠");
	printf("═════════════════════════════════════════════");
	printf("╣"
		   "\n");
	++y; //10
	printf("║");
	for (x = 0; x < 16; x++)
		place_snake(g, y, x);
	printf("║");
	printf("1                                            ");
	printf("║"
		   "\n");
	++y; //11
	printf("║");
	for (x = 0; x < 16; x++)
		place_snake(g, y, x);
	printf("║");
	printf("2                                            ");
	printf("║"
		   "\n");
	++y; //12
	printf("║");
	for (x = 0; x < 16; x++)
		place_snake(g, y, x);
	printf("║");
	printf("3                                            ");
	printf("║"
		   "\n");
	++y; //13
	printf("║");
	for (x = 0; x < 16; x++)
		place_snake(g, y, x);
	printf("║");
	printf("4                                            ");
	printf("║"
		   "\n");
	++y; //14
	printf("║");
	for (x = 0; x < 16; x++)
		place_snake(g, y, x);
	printf("║");
	printf("5                                            ");
	printf("║"
		   "\n");
	++y; //15
	printf("║");
	for (x = 0; x < 16; x++)
		place_snake(g, y, x);
	printf("║");
	printf("6                                            ");
	printf("║"
		   "\n");
	printf("╠════════════════════════════════╣");
	printf("7                                            ");
	printf("║"
		   "\n");
	printf("║  %-30.30s║", mode);
	printf("8                                            ");
	printf("║"
		   "\n");
	printf("║  Time left:%5lis              ║", g->time_left / 1000U);
	printf("9                                            ");
	printf("║"
		   "\n");
	printf("║                                ║");
	printf("10                                           ");
	printf("║"
		   "\n");
	printf("║                                ║");
	printf("11                                           ");
	printf("║"
		   "\n");
	printf("╚");
	for (x = 1; x < 33; x++)
		printf("═");
	printf("╩");
	for (x = 34; x < SCREEN_WIDTH - 1; x++)
		printf("═");
	printf("╝"
		   "\n");
}

void print_game(t_game* g)
{
	printf(CLEAR_SCREEN);
	switch (g->game_mode)
	{
		case GM_REGISTRATION:
		{
			print_wait(g, "Registration");
			break;
		}
		case GM_WAIT1:
		{
			print_wait(g, "Practice starting soon");
			break;
		}
		case GM_PRACTICE:
		{
			print_wait(g, "Practice round");
			break;
		}
		case GM_WAIT2A:
		{
			print_wait(g, "Practice ended");
			break;
		}
		case GM_WAIT2B:
		{
			print_wait(g, "Game starting soon");
			break;
		}
		case GM_QUALIFICATION:
		{
			print_wait(g, "Game");
			break;
		}
		case GM_WAIT3A:
		{
			print_wait(g, "Thanks for playing");
			break;
		}
		default:
		{
			printf("Unknown mode %i\n", g->game_mode);
		}
	}
}

int main(void)
{
	printf(HIDE_CURSOR);
	enableRawMode();
	elapsed_ms(1);
	int				   sock = 0;
	uint8_t			   ping_pong[4];
	uint8_t			   verified = 0;
	struct sockaddr_in server_addr;
	ssize_t			   write_len = 0;
	char			   name[9] = {0};
	char			   host[7] = {0};
	t_game			   g;
	t_packet			packet;
	uint64_t			next_update = 0;

	char		hostname[256];
	const char* username = getenv("USER");
	if (username == NULL)
	{
		logger(ERROR, "No username", __FILE__, __LINE__);
		ft_exit(EXIT_FAILURE);
	}
	snprintf(name, 9, "%s", username);
	logger(INFO, name, __FILE__, __LINE__);
	if (gethostname(hostname, sizeof(hostname)) == 0)
	{
		snprintf(host, 7, "%s", hostname);
		logger(INFO, host, __FILE__, __LINE__);
	}
	else
	{
		logger(ERROR, "No hostname", __FILE__, __LINE__);
		ft_exit(EXIT_FAILURE);
	}
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		logger(ERROR, "socket() failed", __FILE__, __LINE__);
		ft_exit(EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);

	if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0)
	{
		logger(ERROR, "inet_pton() failed", __FILE__, __LINE__);
		ft_exit(EXIT_FAILURE);
	}

	if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		logger(ERROR, "connect() failed", __FILE__, __LINE__);
		ft_exit(EXIT_FAILURE);
	}
	logger(INFO, "Connected to server (or connecting)...", __FILE__, __LINE__);

	fd_set read_fds, write_fds;
	while (1)
	{
		elapsed_ms(1);
		if (g.game_mode == GM_PRACTICE || g.game_mode == GM_QUALIFICATION)
		{
			write_len = check_dir(&g, &packet);
			if (write_len && next_update < elapsed_ms(0))
			{
				write_len = sizeof(t_packet);
				next_update = elapsed_ms(0) + GAME_TICK / 5;
			}
			else
			{
				write_len = 0;
			}
		}
		//write new dir and stuff if game status is in practice
		elapsed_ms(1);
		FD_ZERO(&read_fds);
		FD_ZERO(&write_fds);

		FD_SET(STDIN_FILENO, &read_fds);
		if (write_len > 0)
			FD_SET(sock, &write_fds);
		else
			FD_SET(sock, &read_fds);

		int max_fd = sock > STDIN_FILENO ? sock : STDIN_FILENO;

		struct timeval tv;
		tv.tv_sec = 0;		 // seconds
		tv.tv_usec = GAME_TICK / 5; // microseconds (0.1s)
		if ((elapsed_ms(0) / 100 % 1000) == 0)
			logger(DEBUG1, "Before select x 1000", __FILE__, __LINE__);
		else if ((elapsed_ms(0) / 100 % 100) == 0)
			logger(DEBUG2, "Before select x 100", __FILE__, __LINE__);
		else if ((elapsed_ms(0) / 100 % 10) == 0)
			logger(DEBUG3, "Before select x 10", __FILE__, __LINE__);
		else
			logger(DEBUG4, "Before select", __FILE__, __LINE__);
		int ret = select(max_fd + 1, &read_fds, &write_fds, NULL, NULL);
		if (ret < 0)
		{
			if (errno == EINTR)
				continue;
			logger(ERROR, "select() failed", __FILE__, __LINE__);
			break;
		}
		// Handle socket writability
		if (write_len > 0 && FD_ISSET(sock, &write_fds))
		{
			logger(TRACE, "sending", __FILE__, __LINE__);
			if (verified < VERIFIED)
			{
				if (verified == SERVER_PING)
				{
					ping_pong[0] = ~ping_pong[1];
					ping_pong[1] = SECRET;
					logger(TRACE, "SP: Sending secret", __FILE__, __LINE__);
					ssize_t n = send(sock, ping_pong, 2, MSG_NOSIGNAL);
					if ((n) <= 0)
					{
						logger(WARNING,
							   "Client disconnected during send",
							   __FILE__,
							   __LINE__);
						close(sock);
						break;
					}
					else if (n == write_len)
					{
						write_len = 0;
						verified += 1;
						logger(SUCCESS, "Successful send", __FILE__, __LINE__);
					}
					else
						logger(NOTICE,
							   "Partial send, retrying",
							   __FILE__,
							   __LINE__);
				}
				else
				{
					logger(WARNING, "Should not be here", __FILE__, __LINE__);
				}
			}
			else if (verified < REGISTERED)
			{
				char buffer[16];
				memcpy(buffer, name, 9);
				memcpy(&(buffer[9]), host, 7);
				// snprintf(buffer, 16, "%s%c%s", name, 0, host);
				// write(1, buffer, 16);
				// write(1, "\n", 1);
				logger(TRACE,
					   "REGISTRATION: Sending name and host",
					   __FILE__,
					   __LINE__);
				ssize_t n = send(sock, buffer, 16, MSG_NOSIGNAL);
				if (n <= 0)
				{
					logger(WARNING,
						   "Client disconnected during send",
						   __FILE__,
						   __LINE__);
					close(sock);
					break;
				}
				else if (n == write_len)
				{
					write_len = 0;
					verified += 1;
					logger(SUCCESS, "Successful send", __FILE__, __LINE__);
				}
				else
					logger(
						NOTICE, "Partial send, retrying", __FILE__, __LINE__);
			}
			else
			{
				//registered handling game
				//countdown
				// game
				// score
				if (g.game_mode == GM_PRACTICE || g.game_mode == GM_QUALIFICATION)
				{
					ssize_t n = send(sock, &packet, sizeof(packet), MSG_NOSIGNAL);
					if (n <= 0)
					{
						logger(WARNING,
							"Client disconnected during send",
							__FILE__,
							__LINE__);
						close(sock);
						break;
					}
					else if (n == sizeof(packet))
					{
						write_len = 0;
						logger(SUCCESS, "Successful send", __FILE__, __LINE__);
					}
					else
						logger(
							NOTICE, "Partial send, retrying", __FILE__, __LINE__);
				}
				logger(INFO, "Not implemented", __FILE__, __LINE__);
				write_len = 0;
			}

			// ssize_t s = send(sock, write_buf, write_len, 0);
			// if (s > 0)
			// {
			// 	logger(DEBUG1, "Sent bytes to server", __FILE__, __LINE__);
			// 	if (s < write_len)
			// 		memmove(write_buf, write_buf + s, write_len - s);
			// 	write_len -= s;
			// }
		}

		// Handle socket readability
		else if (FD_ISSET(sock, &read_fds))
		{
			logger(TRACE, "Reading", __FILE__, __LINE__);
			if (verified < VERIFIED)
			{
				ssize_t n = recv(sock, ping_pong, sizeof(ping_pong), 0);
				if (n == 2)
				{
					if (verified == NOT_VERIFIED)
					{
						if (ping_pong[0] == 0)
						{
							write_len = 2;
							verified += 1;
							logger(
								INFO, "NV: Server is okay", __FILE__, __LINE__);
						}
						else
						{
							logger(WARNING,
								   "Server is not okay",
								   __FILE__,
								   __LINE__);
						}
					}
					else if (verified == CLIENT_SENT_SECRET)
					{
						if (ping_pong[0] == ((uint8_t)~SECRET))
						{
							write_len = 16;
							verified = VERIFIED;
							logger(INFO,
								   "CSS: Server is trustworthy",
								   __FILE__,
								   __LINE__);
						}
						else
						{
							logger(WARNING,
								   "Server is not trustworthy",
								   __FILE__,
								   __LINE__);
						}
					}
					else
					{
						logger(
							WARNING, "Should not be here", __FILE__, __LINE__);
					}
				}
				else if (n > 2)
				{
					logger(
						WARNING, "Server sending too much", __FILE__, __LINE__);
				}
				else if (n <= 0)
				{
					logger(NOTICE,
						   "Server closed connection during read",
						   __FILE__,
						   __LINE__);
					close(sock);
					break;
				}
				else
					logger(
						NOTICE, "Partial recv, retrying", __FILE__, __LINE__);
			}
			else if (verified == REGISTERED)
			{
				ssize_t n = recv(sock, &g, sizeof(g), 0);
				if (n == sizeof(g))
				{
					logger(INFO, "received struct", __FILE__, __LINE__);
					print_game(&g);
				}
				else if (n <= 0)
				{
					logger(NOTICE,
						   "Server closed connection during read",
						   __FILE__,
						   __LINE__);
					close(sock);
					break;
				}
				else
				{
					logger(
						NOTICE, "Partial recv, retrying", __FILE__, __LINE__);
				}
			}
			else
			{
				logger(INFO, "Not here", __FILE__, __LINE__);
			}
		}
	}

	close(sock);
	logger(INFO, "Client exiting", __FILE__, __LINE__);
	disableRawMode();
	printf("/******************************************************************************/\n");
	return 0;
}

//add magic handshake and verification
// countdown to start the game (advice user to make terminal window bigger)
// when gamem mode is game display game
// show results at the end.

/******************************************************************************/
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/******************************************************************************/
//https://upload.wikimedia.org/wikipedia/commons/6/6b/Midnight_Commander_screenshot.png
//https://en.wikipedia.org/wiki/Box-drawing_characters#/media/File:Midnight_Commander_screenshot.png
/******************************************************************************/
/*01234567890123456789012345678901*  NAME  |  SCORE   | LENGTH | SPEED        */
/*11234567890123456789012345678901*  name1 | 13567890 |     32 |     2        */
/*21234567890123456789012345678901*  name2 |  9846546 |     16 |     4        */
/*31234567890123456789012345678901*  name3 |  9846546 |     16 |     4        */
/*41234567890123456789012345678901*      4 |  9846546 |     16 |     4        */
/*51234567890123456789012345678901*      5 |  9846546 |     16 |     4        */
/*61234567890123456789012345678901*      6 |  9846546 |     16 |     4        */
/*71234567890123456789012345678901*      7 |  9846546 |     16 |     4        */
/*81234567890123456789012345678901*      8 |  9846546 |     16 |     4        */
/*91234567890123456789012345678901*********************************************/
/*01234567890123456789012345678901*                                           */
/*11234567890123456789012345678901*     CHAT?                                 */
/*21234567890123456789012345678901*  name1: gg                                */
/*31234567890123456789012345678901*  *me: that was fun*                       */
/*41234567890123456789012345678901*  spectator: Go Naboo!                     */
/*51234567890123456789012345678901*                                           */
/**********************************                                           */
/* Time left: 356s                *                                           */
/* Length:    13                  *                                           */
/* Speed:     25                  *                                           */
/* Connected: yes                 *  >_                                       */
/******************************************************************************/
