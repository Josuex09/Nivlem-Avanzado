//
// Created by josue on 11/7/15.
//

#include "tcp_conection.h"


int create_tcp_conection(char* host,int port){
    int socket_fd;
    struct sockaddr_in addr;
    socklen_t addr_size;
    socket_fd = socket(AF_INET,SOCK_STREAM,0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(host);

    memset(addr.sin_zero, '\0', sizeof addr.sin_zero);
    addr_size = sizeof addr;
    if(connect(socket_fd, (struct sockaddr *) &addr, addr_size)<0){
        printf("No se pudo crear la conexion\n");
        return -1;
    }

    return socket_fd;
}

void sendTo(int socket,char * msg){
    send(socket,msg,strlen(msg),0);
}

