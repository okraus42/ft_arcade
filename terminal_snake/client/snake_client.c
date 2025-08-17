// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>

// int main(void)
// {
// 	char* user = getenv("USER"); // get $USER from environment
// 	if (user == NULL)
// 	{
// 		fprintf(stderr, "Unknown user\n");
// 		return 1;
// 	}

// 	char hostname[64];
// 	char host[7];
// 	if (gethostname(hostname, sizeof(hostname)) == 0)
// 	{
// 		strncpy(host, hostname, 6);
// 		host[6] = '\0';
// 	}
// 	else
// 	{
// 		strncpy(host, "??????", 7);

// 	}

// 	printf("User: %s\n", user);
// 	printf("Hostname (first 6 chars): %s\n", host);

// 	return 0;
// }

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVER_IP	"127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

int main()
{
	int				   sock = 0;
	struct sockaddr_in server_addr;
	char			   buffer[BUFFER_SIZE];

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);

	if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0)
	{
		perror("inet_pton");
		exit(EXIT_FAILURE);
	}

	if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("connect");
		exit(EXIT_FAILURE);
	}

	printf("Connected to server.\n");

	while (1)
	{
		int valread = recv(sock, buffer, BUFFER_SIZE - 1, 0);
		if (valread <= 0)
		{
			printf("Server closed connection.\n");
			break;
		}
		buffer[valread] = '\0';
		printf("Server: %s", buffer);

		// Respond only when server asks
		char* response = "Client: Hello, server!\n";
		send(sock, response, strlen(response), 0);
	}

	close(sock);
	return 0;
}

//add magic handshake and verification