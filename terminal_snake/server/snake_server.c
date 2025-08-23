
// 1. Own header first
#include "snake_server.h"

// 2. Project-wide defines, feature flags (if needed)
#include "interface.h"

// 3. Standard C headers
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

// 4. Third-party libraries

// 5. Other project headers
#include "logger.h"
#include "timer.h"

int	   server_fd;


void shutdown_server(int signum)
{
	(void)signum;
	printf("\nShutting down server...\n");
	close(server_fd);
	exit(0);
}

int main(void)
{

	t_server	s;
	signal(SIGINT, shutdown_server);
	memset(&s, 0, sizeof(s));
	s.current_time = elapsed_ms(1);

	struct sockaddr_in server_addr, client_addr;
	socklen_t		   addr_len = sizeof(client_addr);
	// int client_sockets[MAX_CLIENTS] = {0};
	// char buffer[BUFFER_SIZE];

	// Create socket
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		logger(ERROR, "socker error", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}
	logger(INFO, "socker created", __FILE__, __LINE__);
	int opt = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) <
		0)
	{
		logger(ERROR, "bind error", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}
	logger(NOTICE, "bind created", __FILE__, __LINE__);
	if (listen(server_fd, 5) < 0)
	{
		logger(ERROR, "listen error", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}
	logger(INFO, "listen passed", __FILE__, __LINE__);
	printf("Server listening on port %d...\n", PORT);

	int max_sd = server_fd;

	logger(SUCCESS, "Intialization passed", __FILE__, __LINE__);

	fd_set		read_fds;
	fd_set		write_fds;
	while (1)
	{
		s.current_time = elapsed_ms(1);
		FD_ZERO(&read_fds);
		FD_ZERO(&write_fds);

		FD_SET(server_fd, &read_fds); 
		max_sd = server_fd; 


		for (int i = 0; i < MAX_CLIENTS; ++i)
		{
			if (s.users[i].sd)
			{
				if (s.users[i].sd > max_sd)
					max_sd = s.users[i].sd;
				if (s.users[i].sending == 0)
				{
					FD_SET(s.users[i].sd , &read_fds);
				}
				else
				{
					FD_SET(s.users[i].sd , &write_fds);
				}
			}
		}


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


		int ret = select(max_sd + 1, &read_fds, &write_fds, NULL, &tv);


		if (ret < 0)
		{
			if (errno == EINTR)
				continue; // signal interrupted
			logger(ERROR, "select failure", __FILE__, __LINE__);
			exit(EXIT_FAILURE);
		}

		if (ret == 0)
			continue ;


		if (FD_ISSET(server_fd, &read_fds))
		{
			int new_sd = 0;
			if ((new_sd = accept(server_fd,  (struct sockaddr*)&client_addr, &addr_len)) < 0) 
			{
				logger(ERROR, "accept failure", __FILE__, __LINE__);
			}
			if (new_sd < MAX_CLIENTS)
			{
				s.users[new_sd].sd = new_sd;
				s.users[new_sd].port = ntohs(client_addr.sin_port);
				s.users[new_sd].ip = ntohl(client_addr.sin_addr.s_addr);
				s.users[new_sd].sending = 1;
				logger(INFO, "New client", __FILE__, __LINE__);
			}
			else
			{
				logger(ERROR, "too many clients", __FILE__, __LINE__);
			}
		}
		
		for (int i = 0; i < MAX_CLIENTS; ++i)
		{
			int sd = s.users[i].sd;
			if (FD_ISSET(sd, &read_fds))
			{
				logger(TRACE, "Reading", __FILE__, __LINE__);
				if (s.users[i].verification < VERIFIED)
				{
					ssize_t n = recv(sd , s.users[i].ping_pong, 4, MSG_NOSIGNAL);
					if (n <= 0) 
					{
						close(sd);
						s.users[i].sd = 0;
						logger(WARNING, "Client disconnected during recv", __FILE__, __LINE__);
					}
					else
					{
						if (n == 2)
						{
							if (s.users[i].verification == SERVER_PING)
							{
								if (s.users[i].ping_pong[0] == ((uint8_t)~SECRET))
								{
									s.users[i].sending = 1;
									s.users[i].verification += 1;
									logger(WARNING, "Client is trustworthy", __FILE__, __LINE__);
								}
								else
								{
									logger(WARNING, "Client is not trustworthy", __FILE__, __LINE__);
								}
							}
							else
							{
								logger(WARNING, "Should not be here", __FILE__, __LINE__);
							}
						}
						else if (n > 2)
						{
							logger(WARNING, "Client sending too much", __FILE__, __LINE__);
						}
						else
							logger(NOTICE, "Partial recv, retrying", __FILE__, __LINE__);
					}
				}
				else if (s.users[i].verification < REGISTERED)
				{
					char buffer[18];
					ssize_t n = recv(sd , buffer, 18, MSG_NOSIGNAL);
					if (n <= 0) 
					{
						close(sd);
						s.users[i].sd = 0;
						logger(WARNING, "Client disconnected during recv", __FILE__, __LINE__);
					}
					else
					{
						if (n == 16)
						{
							memcpy (s.users[i].name, buffer, 9);
							memcpy (s.users[i].host, &(buffer[9]), 7);
							write(1, buffer, 16);
							write(1, "\n", 1);
							printf("%s %s\n", s.users[i].name, s.users[i].host);
							logger(INFO, "Client sent name and host", __FILE__, __LINE__);
							s.users[i].verification = REGISTERED;
						}
						else if (n > 16)
						{
							logger(WARNING, "Client sending too much", __FILE__, __LINE__);
						}
						else
							logger(NOTICE, "Partial recv, retrying", __FILE__, __LINE__);
					}
				}
				else
				{
					logger(NOTICE, "Client verified, nothing to read??", __FILE__, __LINE__);
					ssize_t n = recv(sd , s.users[i].ping_pong, 3, MSG_NOSIGNAL);
					s.users[i].ping_pong[3] = 0;
					(void)n;
					// printf("%zi: %s\n", n, s.users[i].ping_pong);
				}
			}
			else if (FD_ISSET(sd, &write_fds))
			{
				logger(TRACE, "Sending", __FILE__, __LINE__);
				if (s.users[i].verification < VERIFIED)
				{
					ssize_t n;
					if (s.users[i].verification == NOT_VERIFIED)
					{
						s.users[i].ping_pong[0] = 0;
						s.users[i].ping_pong[1] = SECRET;
						logger(TRACE, "NV: Sending secret", __FILE__, __LINE__);
					}
					else if (s.users[i].verification == CLIENT_SENT_SECRET)
					{
						s.users[i].ping_pong[0] = ~s.users[i].ping_pong[1];
						s.users[i].ping_pong[1] = SECRET;
						logger(TRACE, "CSS: Sending secret back", __FILE__, __LINE__);
					}
					else
					{
						logger(WARNING, "Should not be here either", __FILE__, __LINE__);
					}
					if ((n = send(sd , s.users[i].ping_pong, 2, MSG_NOSIGNAL)) <= 0) 
					{
						close(sd);
						s.users[i].sd = 0;
						logger(WARNING, "Client disconnected during send", __FILE__, __LINE__);
					}
					else
					{
						if (n == 2)
						{
							s.users[i].sending = 0;
							s.users[i].verification += 1;
							if (s.users[i].verification == SERVER_PONG)
								s.users[i].verification = VERIFIED;
						}
						else
							logger(NOTICE, "Partial send, retrying", __FILE__, __LINE__);
					}
				}
				else
				{
					logger(NOTICE, "Client verified, nothing to send", __FILE__, __LINE__);
				}
			}
		}
	}

	return 0;
}

// User ID = username + hostname
// 10 minutes practice and registeration
// 10 minute qualification to get best score
// 10 minutes best 16 1v1 (8, 4)
// 10 minutes top 8 (4, 2)

//read fds
//write fds

//see if I can send message every 0.1 second or less

//have a 5 minute game to get maximum xp
//show results at the end

// add naboo and alderaan info

//final between best player of naboo and alderaan