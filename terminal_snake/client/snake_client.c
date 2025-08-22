
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

#include "logger.h"
#include "snake_client.h"
#include "interface.h"

#define SERVER_IP	"127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

int main(void)
{
	int				   sock = 0;
	struct sockaddr_in server_addr;
	char			   read_buf[BUFFER_SIZE];
	char			   write_buf[BUFFER_SIZE];
	ssize_t			   write_len = 0;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		logger(ERROR, "socket() failed", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}
	fcntl(sock, F_SETFL, O_NONBLOCK);
	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);

	if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0)
	{
		logger(ERROR, "inet_pton() failed", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}

	if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		logger(ERROR, "connect() failed", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}
	logger(INFO, "Connected to server (or connecting)...", __FILE__, __LINE__);

	fd_set read_fds, write_fds;
	while (1)
	{
		//process all games (if something to send, send, receive otherwise)
		FD_ZERO(&read_fds);
		FD_ZERO(&write_fds);

		FD_SET(STDIN_FILENO, &read_fds);
		FD_SET(sock, &read_fds);
		if (write_len > 0)
			FD_SET(sock, &write_fds);

		int max_fd = sock > STDIN_FILENO ? sock : STDIN_FILENO;

		struct timeval tv;
		tv.tv_sec = 0;		 // seconds
		tv.tv_usec = 100000; // microseconds (0.1s)
		if ((elapsed_ms() / 100 % 1000) == 0)
			logger(DEBUG1, "Before select x 1000", __FILE__, __LINE__);
		else if ((elapsed_ms() / 100 % 100) == 0)
			logger(DEBUG2, "Before select x 100", __FILE__, __LINE__);
		else if ((elapsed_ms() / 100 % 10) == 0)
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

		// Handle stdin input
		if (FD_ISSET(STDIN_FILENO, &read_fds))
		{
			ssize_t r = read(STDIN_FILENO, write_buf, sizeof(write_buf));
			if (r > 0)
			{
				write_len = r;
				logger(DEBUG1, "Read from stdin", __FILE__, __LINE__);
			}
		}

		// Handle socket writability
		if (write_len > 0 && FD_ISSET(sock, &write_fds))
		{
			ssize_t s = send(sock, write_buf, write_len, 0);
			if (s > 0)
			{
				logger(DEBUG1, "Sent bytes to server", __FILE__, __LINE__);
				if (s < write_len)
					memmove(write_buf, write_buf + s, write_len - s);
				write_len -= s;
			}
		}

		// Handle socket readability
		if (FD_ISSET(sock, &read_fds))
		{
			ssize_t r = recv(sock, read_buf, sizeof(read_buf) - 1, 0);
			if (r > 0)
			{
				read_buf[r] = '\0';
				logger(INFO, "Received: %s", __FILE__, __LINE__);
				printf("%s", read_buf);
			}
			else if (r == 0)
			{
				logger(INFO, "Server closed connection", __FILE__, __LINE__);
				break;
			}
		}
	}

	close(sock);
	logger(INFO, "Client exiting", __FILE__, __LINE__);
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
