#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void s_socket(){

    printf("open!! server\n");

}

void s_bind(){

    printf("Chatting on\n");
}

void s_listen(int client_num){
}

void s_accept(int client_num){
    pritnf("connected to Client %d\n", client_num);

}

void read_write(int client_num, char *msg){
    printf("message from client %d : %s\n",client_num,msg);
}

void s_close(){

}

int main(void){
    int port = 30;
    printf("Server Port : %d\n",port);
    s_socket();
    s_bind();
    s_listen(1);
    s_accept(1);
    read_write(1,"hi");
    return 0;
}