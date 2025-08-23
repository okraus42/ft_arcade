
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
#include "timer.h"

#define SERVER_IP	"127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

#define SECRET 84

int main(void)
{
	elapsed_ms(1);
	int				   sock = 0;
	uint8_t			   ping_pong[4];
	uint8_t				verified = 0;
	struct sockaddr_in server_addr;
	ssize_t			   write_len = 0;
	char				name[9] = {0};
	char				host[7] = {0};
	t_game				g;
	
	char hostname[256];
	const char *username = getenv("USER");
	if (username == NULL)
	{
		logger(ERROR, "No username", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}
	snprintf(name, 7, "%s", username);
	logger(INFO, name, __FILE__, __LINE__);
	if (gethostname(hostname, sizeof(hostname)) == 0)
	{
		snprintf(host, 7, "%s", hostname);
		logger(INFO, host, __FILE__, __LINE__);
	}
	else
	{
		logger(ERROR, "No hostname", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		logger(ERROR, "socket() failed", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}

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
		tv.tv_usec = 100000; // microseconds (0.1s)
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
					if ((n ) <= 0) 
					{
						logger(WARNING, "Client disconnected during send", __FILE__, __LINE__);
						close(sock);
						break ;
					}
					else if (n == write_len)
					{
						write_len = 0;
						verified += 1;
						logger(SUCCESS, "Successful send", __FILE__, __LINE__);
						
					}
					else
						logger(NOTICE, "Partial send, retrying", __FILE__, __LINE__);
				}
				else
				{
					logger(WARNING, "Should not be here", __FILE__, __LINE__);
				}
			}
			else if (verified < REGISTERED)
			{
				char buffer[16];
				memcpy (buffer, name, 9);
				memcpy (&(buffer[9]), host, 7);
				// snprintf(buffer, 16, "%s%c%s", name, 0, host);
				write(1, buffer, 16);
				write(1, "\n", 1);
				logger(TRACE, "REGISTRATION: Sending name and host", __FILE__, __LINE__);
				ssize_t n = send(sock, buffer, 16, MSG_NOSIGNAL);
				if (n <= 0) 
				{
					logger(WARNING, "Client disconnected during send", __FILE__, __LINE__);
					close(sock);
					break ;
				}
				else if (n == write_len)
				{
					write_len = 0;
					verified += 1;
					logger(SUCCESS, "Successful send", __FILE__, __LINE__);
					
				}
				else
					logger(NOTICE, "Partial send, retrying", __FILE__, __LINE__);
			}
			else
			{
				//registered handling game 
				//countdown
				// game
				// score
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
							logger(INFO, "NV: Server is okay", __FILE__, __LINE__);
						}
						else
						{
							logger(WARNING, "Server is not okay", __FILE__, __LINE__);
						}
					}
					else if (verified == CLIENT_SENT_SECRET)
					{
						if (ping_pong[0] == ((uint8_t)~SECRET))
						{
							write_len = 16;
							verified = VERIFIED;
							logger(INFO, "CSS: Server is trustworthy", __FILE__, __LINE__);
						}
						else
						{
							logger(WARNING, "Server is not trustworthy", __FILE__, __LINE__);
						}
					}
					else
					{
						logger(WARNING, "Should not be here", __FILE__, __LINE__);
					}
				}
				else if (n > 2)
				{
					logger(WARNING, "Server sending too much", __FILE__, __LINE__);
				}
				else if (n <= 0)
				{
					logger(NOTICE, "Server closed connection during read", __FILE__, __LINE__);
					close(sock);
					break ;
				}
				else
					logger(NOTICE, "Partial recv, retrying", __FILE__, __LINE__);
			}
			else if (verified == REGISTERED)
			{
				ssize_t n = recv(sock, &g, sizeof(g), 0);
				if (n == sizeof(g))
				{
					logger(INFO, "received struct", __FILE__, __LINE__);
					printf("time left: %li\n", g.time_left);
				}
				else if (n <= 0)
				{
					logger(NOTICE, "Server closed connection during read", __FILE__, __LINE__);
					close(sock);
					break ;
				}
				else
				{
					logger(NOTICE, "Partial recv, retrying", __FILE__, __LINE__);
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
