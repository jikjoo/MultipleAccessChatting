#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void socket(){
    printf("open!! client\n");
}

void connect(){
    printf("Success to connect to server!\n");

}

void read(){
    printf("Server : welcome to chatting server!!\n");
}

void write(){
    //printf(" Chatting On...\n");
    char msg[2000];
    scanf("%s",msg);
}

void close(){

}

int main(void){
    int ip[4] = {127,0,0,1};
    int port = 1234;
    printf("Input Server IP Address : %d.%d.%d.%d",ip[0],ip[1],ip[2],ip[3]);
    printf("Input Server Port Number : %d",port);
    socket();
    connect();
    read();
    write();
    return 0;
}