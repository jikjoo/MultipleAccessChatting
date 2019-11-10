//gcc -c server.c && gcc -pthread -o server.o server.c && gcc -c client.c && gcc -pthread -o client.o client.c
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_CLIENT 5
#define PORT 1234
//message limit
#define MLIM 128 

void *read_write(void *);
int clientCount = 0;
// used for multithread
struct clientInfo {
	int index; // ascending number to distinguish client
	int client_num; // result after socket accept
};
struct clientInfo ClientInfo[MAX_CLIENT]; //shared variable

//for mutex
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[]) {
	//prepare server
	struct sockaddr_in server, client;
	int addrlen = sizeof(server);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(PORT);
	int socket_num, client_num;
    int Index = 0; // ascending number for distinguish clients

	//purchase phone
	printf("Server Port : %d\n", PORT);
	socket_num = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_num <= 0) {
		printf("fail!! to open server\n");
	}
	printf("open!! server\n");

	//assigned phone number
    // bind socket and server address
	int b = bind(socket_num, (struct sockaddr *)&server, sizeof(server));
	if (b < 0) {
		printf("fail!! to bind\n");
		return 1;
	}
	printf("Chatting on\n");
	//listening
	listen(socket_num, MAX_CLIENT);

	// pick up the phone
	pthread_t thread_id[MAX_CLIENT];
	int clilen = sizeof(client);
	while (client_num = accept(socket_num, (struct sockaddr *)&client, (socklen_t *)&clilen)) {
		int pos = 0; // find empty in ClientInfo
		if (clientCount < MAX_CLIENT) {
			pthread_mutex_lock(&mtx); 
            // critical zone to protect ClientInfo
			for (pos = 0; pos < MAX_CLIENT; pos++) {
				if (ClientInfo[pos].client_num == 0 && ClientInfo[pos].index == 0)
					break;
			}
			ClientInfo[pos].client_num = client_num;
			ClientInfo[pos].index = ++Index;
			pthread_mutex_unlock(&mtx);
		} else {
			printf("fail!! full client\n");
			continue;
		}
		// make thread for accepted Clients
		if (pthread_create(&thread_id[pos], NULL, read_write, (void *)&ClientInfo[pos])) {
			printf("fail!! to create thread\n");
			return 1;
		}
		clientCount++;
	}
	for (int i = 0; i < MAX_CLIENT; i++)
		pthread_join(thread_id[i], NULL);

	if (client_num < 0) {
		printf("fail!! to accept\n");
		return 1;
	}
	printf("Close");
	return 0;
}

// function executed in thread for read and write client messages
void *read_write(void *clientDetail) {
	// client socket
	pthread_mutex_lock(&mtx);
	// critical zone
	struct clientInfo *ClientDetail = (struct clientInfo *)clientDetail;
	int c_socket = ClientDetail->client_num; // current selected thread's socket number
	int index = ClientDetail->index;
	//int c_socket = *(int *)client_num;
	int read_size = 0;
	char buffer[MLIM];
	char msg[MLIM + 10];
    int closed = 0; // socket state 0 : opened, 1: closed 

	printf("connected to Client %d\n", index);
	// send connection message to clients
	for (int i = 0; i < MAX_CLIENT; i++) {
		if (ClientInfo[i].client_num == 0) // no accepted clients
			continue;
		else if (ClientInfo[i].client_num == c_socket) { // current selected client
			bzero(buffer, MLIM);
			strcpy(buffer, "Server : Welcome to chatting server!!");
		} else {
			bzero(buffer, MLIM); // other accepted clients
			sprintf(buffer, "Client %d has been connected!", index);
		}
		if (send(ClientInfo[i].client_num, buffer, strlen(buffer), 0) < 0) {
			printf("fail! to send message to %d\n", ClientInfo[i].index);
			clientCount--;
			ClientDetail->client_num = 0;
			ClientDetail->index = 0;
			close(c_socket);
			break;
		}
	}
	pthread_mutex_unlock(&mtx);

	// receive message from client
	bzero(buffer, MLIM);
	bzero(msg, MLIM + 10);
	while ((read_size = recv(c_socket, buffer, MLIM, 0) > 0)) {
		pthread_mutex_lock(&mtx);
		// critical zone
		buffer[MLIM] = '\0';
		printf("message from client %d : %s\n", index, buffer);
		// Close current client
		if (strcmp(buffer, "Q") == 0) {
            closed = 1;
            sprintf(msg, "Client %d disconnected",index);
		}
        // not close client, and add information of who said in message
		else sprintf(msg, "Client %d : %s", index, buffer);
		// echo to other clients
		for (int i = 0; i < MAX_CLIENT; i++) {
			if (ClientInfo[i].client_num == 0 || ClientInfo[i].client_num == c_socket) {
                // pass No accepted client or current client
				continue;
			}
            // send messages.  if fail, close current socket
			if (send(ClientInfo[i].client_num, msg, strlen(msg), 0) < 0) {
				printf("fail! to send message to %d\n", ClientInfo[i].index);
				clientCount--;
				ClientDetail->client_num = 0;
				ClientDetail->index = 0;
				close(c_socket);
				break;
			}
		}
        // empty array for buffer and message
		bzero(buffer, MLIM);
		bzero(msg, MLIM + 10);
        // if current client closed, change ClientInfos
        if(closed){
            ClientDetail->client_num = 0;
            ClientDetail->index = 0;
            clientCount--;
            printf("disconnected Client %d\n", index);
            close(c_socket);
            pthread_mutex_unlock(&mtx);
            break;
        }
		pthread_mutex_unlock(&mtx);
	}
	if (read_size < 0) {
		printf("fail!! to receive %d %d\n", read_size, c_socket);
	}
}