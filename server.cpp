#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <mutex>

#define BUFFER_SIZE 1024

typedef struct sockaddr_in	addr_in;
typedef struct sockaddr		addr;

char	buffer[BUFFER_SIZE] = {0};
std::mutex	ortak;

void*	handleClient(int s)
{
	int		read_data;

	while (true)
	{
		ortak.lock();
		memset(buffer, 0, BUFFER_SIZE);
		read_data = recv(s, buffer, BUFFER_SIZE, 0);
		if (read_data <= 0)
		{
			close(s);
			ortak.unlock();	
			return (NULL);
		}
		std::cout << buffer;
		ortak.unlock();
	}
	return (NULL);
}

int main(void)
{
    int		serverS, clientS;
	addr_in	serverA;
	char	buffer[BUFFER_SIZE];
	
	// create socket for server and check for errors
	if ((serverS = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Error while creating the socket");
		exit(1);
	}

	//set the struct data
	serverA.sin_family = AF_INET;
	serverA.sin_port = htons(3131);
	serverA.sin_addr.s_addr = INADDR_ANY;

	// bind the socket
	if (bind(serverS, (addr *)&serverA, sizeof(serverA)) < 0)
	{
		perror("Error while binding the socket");
		exit(1);
	}

	//listen on the socket
	if (listen(serverS, 10) < 0)
	{
		perror("Error while lisetining");
		exit(1);
	}
	std::cout << "Server listening on port 3131\n";

	//accept the connections
	std::vector <std::thread> threads;
	while (true)
	{
		addr_in		clientA;
		socklen_t	clientL = sizeof(clientA);
		int			clientS = accept(serverS, (addr *)&clientA, &clientL);
		
		std::thread clientThread(handleClient, clientS);
		threads.push_back(std::move(clientThread));
	}
	for (auto& thread : threads)
		thread.join();
	close(serverS);
	return (0);
}
