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

#include "interface.h"

#include "snake_client.h"

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
// countdown to start the game
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
