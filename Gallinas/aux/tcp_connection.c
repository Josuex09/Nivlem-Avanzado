//
// Created by josue on 11/19/15.
//

#include "tcp_connection.h"

void sendTo(int socket, char * msg){
    send(socket,msg,strlen(msg),0);
}
