#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void socket(){

    printf("open!! server\n");

}

void bind(){

    printf("Chatting on\n");
}

void listen(int client_num){
}

void accept(int client_num){
    pritnf("connected to Client %d\n", client_num);

}

void read_write(int client_num, char *msg){
    printf("message from client %d : %s\n",client_num,msg);
}

void close(){

}

int main(void){
    int port = 30;
    printf("Server Port : %d\n",port);
    socket();
    bind();
    listen(1);
    accept(1);
    read_write(1,"hi");
    return 0;
}