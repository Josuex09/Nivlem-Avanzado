//
// Created by josue on 11/7/15.
//

#ifndef GALLINERO_TCP_CONECTION_H
#define GALLINERO_TCP_CONECTION_H

#include <sys/socket.h>
#include "netinet/in.h"
#include "string.h"
#include "netdb.h"
#include "arpa/inet.h"
#include "stdio.h"
int create_tcp_conection(char* host,int port);
void sendTo(int socket,char * msg);

#endif //GALLINERO_TCP_CONECTION_H
