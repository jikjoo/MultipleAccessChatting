#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void c_socket(){
    printf("open!! client\n");
}

void c_connect(){
    printf("Success to connect to server!\n");

}

void c_read(){
    printf("Server : welcome to chatting server!!\n");
}

void c_write(){
    //printf(" Chatting On...\n");
    char msg[2000];
    scanf("%s",msg);
}

void c_close(){

}

int main(void){
    int ip[4] = {127,0,0,1};
    int port = 1234;
    printf("Input Server IP Address : %d.%d.%d.%d",ip[0],ip[1],ip[2],ip[3]);
    printf("Input Server Port Number : %d",port);
    c_socket();
    c_connect();
    c_read();
    c_write();
    return 0;
}