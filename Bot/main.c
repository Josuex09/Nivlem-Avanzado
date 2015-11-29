#include "stdio.h"
#include  "sys/socket.h"
#include "netinet/in.h"
#include "string.h"
#include "stdlib.h"
#include "pthread.h"
#include "graphics/paint.h"

#define PORT 3333


int hours,minutes,seconds,food_cost,water_cost,actual_water,actual_food,water_refill_amount,food_refill_amount,water_min_allowed,food_min_allowed;
int actual_food_cost = 0;
int actual_water_cost = 0;
long total_water_used =0;
long total_food_used = 0;
int total_eggs_picked = 0;

int listen_conections(){
    int listenfd,socketfd;
    socklen_t length = NULL;
    static struct sockaddr_in cli_addr;
    if((listenfd = socket(AF_INET,SOCK_STREAM,0))< 0)
        return 0;

    static struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT);

    if(bind(listenfd,(struct sockaddr*) &serv_addr,sizeof(serv_addr))<0){
        printf("Error con el bind\n");
        exit(0);
    }

    if(listen(listenfd,1)<0){
        printf("Error con el listen\n");
        return 0;
    }

    length = sizeof(cli_addr);

    if((socketfd = accept(listenfd,(struct sockaddr *)&cli_addr,&length))<0){
        printf("Error escuchando\n");
        return 0;
    }

    return socketfd;
}


void sendTo(int socket,char * msg){
    send(socket,msg,strlen(msg),0);
}


int main(){

    int socketfd = listen_conections();
    printf("Se creó una conexion \n");
    char buffer[200];
    // RECIBIR LOS COSTOS Y EL MAXIMO.
    recv(socketfd,buffer,200,0);
    sscanf(buffer,"COSTO AGUA %d\nCOSTO CONCENTRADO %d\nMINIMO DE AGUA %d\nMINIMO DE CONCENTRADO %d\nRECARGA DE AGUA %d\nRECARGA DE CONCENTRADO %d",&water_cost,&food_cost,&water_min_allowed,&food_min_allowed,&water_refill_amount,&food_refill_amount);
    actual_food = food_refill_amount;
    actual_water = water_refill_amount;
    printf("COSTOS:\nAgua: %d\nConcentrado %d\n\n",water_cost,food_cost);
    memset(&buffer, 0, sizeof(buffer));





    pthread_t graphics_thread;
    pthread_create(&graphics_thread, NULL, &init_graphics, NULL);

    char msg[70];
    int value;
    while(1){
        recv(socketfd,msg,70,0);
        if(strstr(msg,"COMIDA")){
            sscanf(msg,"RESTAR %d DE COMIDA\n",&value);
            actual_food-= value;
            total_food_used+=value;
            if(actual_food<= food_min_allowed){
                //llamar al bot
                actual_food+= food_refill_amount;
                actual_food_cost += food_cost;
                sprintf(msg,"SE AUMENTO EL COSTO DEL CONCENTRADO A %d\n",actual_food_cost);
                sendTo(socketfd,msg);
                printf("SE ACABO EL CONCENTRADO, SE AUMENTO EL COSTO A %d\n",actual_food_cost);
            }
            printf("Queda %d de concentrado\n",actual_food);
        }
        else if(strstr(msg,"AGUA")){
            sscanf(msg,"RESTAR %d DE AGUA\n",&value);
            actual_water-=value;
            total_water_used += value;
            if(actual_water<= water_min_allowed){
                actual_water += water_refill_amount;
                actual_water_cost += water_cost;
                sprintf(msg,"SE AUMENTO EL COSTO DEL AGUA A %d\n",actual_water_cost);
                sendTo(socketfd,msg);
                printf("SE ACABO EL AGUA, SE AUMENTO EL COSTO A %d\n",actual_water_cost);
            }
            printf("Queda %d de agua\n",actual_water);
        }
        else if(strstr(msg,"ALARMA DENTRO DE")){
            sscanf(msg,"ALARMA DENTRO DE %d",&value);
            hours = value / 3600;
            minutes = (value % 3600)/60;
            seconds = (value % 3600)%60;
            //printf("%d : %d %d\n",hours,minutes,seconds);

        }
        else if(strstr(msg,"SE RECOGIERON")){
            sscanf(msg,"SE RECOGIERON %d HUEVOS",&value);
            total_eggs_picked+=value;
        }
        memset(&msg, 0, sizeof(msg));
    }


    return 0;

}
