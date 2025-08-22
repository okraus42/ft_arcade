
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
fd_set master_set;

void shutdown_server(int signum)
{
	(void)signum;
	printf("\nShutting down server...\n");
	close(server_fd);
	exit(0);
}

int main()
{
	signal(SIGINT, shutdown_server);

	struct sockaddr_in server_addr, client_addr;
	socklen_t		   addr_len = sizeof(client_addr);
	// int client_sockets[MAX_CLIENTS] = {0};
	char buffer[BUFFER_SIZE];

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

	FD_ZERO(&master_set);
	FD_SET(server_fd, &master_set);
	int max_sd = server_fd;

	logger(SUCCESS, "Intialization passed", __FILE__, __LINE__);
	while (1)
	{
		fd_set read_fds = master_set;
		fd_set write_fds;

		FD_ZERO(&write_fds);

		// Example: if you have data pending for a specific socket
		// you'd do something like:
		// if (have_data_to_send) {
		// 	FD_SET(client_socket, &write_fds);
		// }
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
		int ret = select(max_sd + 1, &read_fds, &write_fds, NULL, &tv);
		if (ret < 0)
		{
			if (errno == EINTR)
				continue; // signal interrupted
			logger(ERROR, "select failure", __FILE__, __LINE__);
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i <= max_sd; i++)
		{
			if (FD_ISSET(i, &read_fds))
			{
				if (i == server_fd)
				{
					// New connection
					int new_socket = accept(
						server_fd, (struct sockaddr*)&client_addr, &addr_len);
					if (new_socket < 0)
					{
						logger(ERROR, "accept failure", __FILE__, __LINE__);
						continue;
					}

					FD_SET(new_socket, &master_set);
					if (new_socket > max_sd)
						max_sd = new_socket;
					logger(INFO, "New client connected", __FILE__, __LINE__);
					printf("New client connected: %d\n", new_socket);

					// Send welcome message
					char* msg = "Server: Hello, client! Please respond.\n";
					send(new_socket, msg, strlen(msg), 0);
				}
				else
				{
					// Existing client sent message
					int valread = recv(i, buffer, BUFFER_SIZE - 1, 0);
					if (valread <= 0)
					{
						printf("Client %d disconnected.\n", i);
						close(i);
						FD_CLR(i, &master_set);
					}
					else
					{
						buffer[valread] = '\0';
						printf("Received from client %d: %s", i, buffer);
						// Server ignores unsolicited messages
						printf("Ignoring message unless prompted.\n");
					}
				}
			}
			if (FD_ISSET(i, &write_fds))
			{
				// Ready to send data
				// 	const char *msg = "Server: Periodic update!\n";
				// 	int sent = send(i, msg, strlen(msg), 0);
				// 	if (sent < 0) {
				// 		perror("send");
				// 		close(i);
				// 		FD_CLR(i, &master_set);
				// 	} else {
				// 		printf("Sent periodic message to client %d\n", i);
				// 	}
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