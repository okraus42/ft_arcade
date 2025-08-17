#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT		12345
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

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
		perror("socket");
		exit(EXIT_FAILURE);
	}

	int opt = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 5) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	printf("Server listening on port %d...\n", PORT);

	FD_ZERO(&master_set);
	FD_SET(server_fd, &master_set);
	int max_sd = server_fd;

	while (1)
	{
		fd_set read_fds = master_set;

		if (select(max_sd + 1, &read_fds, NULL, NULL, NULL) < 0)
		{
			if (errno == EINTR)
				continue; // signal interrupted
			perror("select");
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i <= max_sd; i++)
		{
			if (FD_ISSET(i, &read_fds))
			{
				if (i == server_fd)
				{
					// New connection
					int new_socket = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
					if (new_socket < 0)
					{
						perror("accept");
						continue;
					}

					FD_SET(new_socket, &master_set);
					if (new_socket > max_sd)
						max_sd = new_socket;
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
		}
	}

	return 0;
}

// User ID = username + hostname
// 10 minutes practice and registeration
// 10 minute qualification to get best score
// 10 minutes best 16 1v1 (8, 4)
// 10 minutes top 8 (4, 2)