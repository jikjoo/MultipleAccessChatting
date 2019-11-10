#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 

#define PORT 1234
#define MLIM 128

void* c_write();
void* c_read();

pthread_mutex_t mutexsum = PTHREAD_MUTEX_INITIALIZER;  
int socket_num;

int main(int argc,char *argv[]){
    struct sockaddr_in server;
    char buffer[128];
    char *msg;
    int close = 0,check;
    socket_num = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_num <= 0){
        printf("fail!! to create socket");
        return -1;
    }
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    // convert IP addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &server.sin_addr)<=0){
        printf("fail!! Invalid IP address\n");
        return -1;
    }
    //server.sin_addr.s_addr = htonl(INADDR_ANY);
    printf("Input Server IP Address : %s\n","127.0.0.1");
    printf("Input Server Port Number : %d\n",PORT);
    printf("open!! client\n");

    if (connect(socket_num, (struct sockaddr *)&server, sizeof(server)) < 0) 
    { 
        printf("\nfail!! to connect Server\n"); 
        return -1; 
    } 
    printf("Success to connect to server!\n");
    /* while(1){
        
        bzero(buffer,MLIM);
        check = read(socket_num,buffer,MLIM);
        buffer[MLIM] = '\0';
        if(check <= 0){
            printf("fail to read %d\n", check);
        }
        printf("%s\n",buffer);

        bzero(buffer,MLIM);
        printf("Message to server: ");
        scanf("%s", buffer);
        printf("writing...%d\n",socket_num);
        check = write(socket_num,buffer,strlen(buffer));
        if(check <= 0){
            printf("fail to write %d\n",check);
        }   
    } */
    

    //setup threads for read and write
    pthread_t threads[2];
    pthread_create(&threads[0],NULL,c_write,NULL);
    pthread_create(&threads[1],NULL,c_read,NULL);
   
    pthread_join(threads[0],NULL);
    pthread_join(threads[1],NULL);
    printf("Close\n");
    return 0;
}

void *c_write(){
    char msg[MLIM];
    int s;
    while(1){
        bzero(msg,MLIM);
        scanf("%[^\n]%*c",msg);
        //printf("writing...%d\n",socket_num);
        if(s = send(socket_num,msg,strlen(msg),0)<0){
            printf("fail!! to send message\n");
            break;
        }
        else if(strcmp(msg,"Q") == 0){
            close(socket_num);
            break;
        }
        //printf("writing done\n");
        bzero(msg,MLIM);
    }
}

void *c_read(){
    char msg[MLIM];
    int r;
    while(1){
        //printf("reading...\n");
        bzero(msg,MLIM);
        if((r = recv(socket_num,msg,MLIM,0))<0){
            printf("fail!! to read message");
            close(socket_num);
            break;
        }
        else if(r==0){
            close(socket_num);
            break;
        }
        else if(strcmp(msg, "Server : Welcome to chatting server!!") == 0){
            printf("  Chatting On...\ninput 'Q' to exit\n");
        }
        printf("%s\n",msg);
    }
}