#include "stdio.h"
#include  "sys/socket.h"
#include "netinet/in.h"
#include "string.h"
#include "pthread.h"
#include "graphics/paint.h"
#define PORT 3335


char buffer[200];
int max_eggs_allowed = 0;
int time_to_wait=0;
int socketfd=0;
int eggs_amount = 0;
int missing = 0;
extern int wake_up;
int water_actual_cost = 0;
int food_actual_cost = 0;

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
        exit(0);
    }

    length = sizeof(cli_addr);

    if((socketfd = accept(listenfd,(struct sockaddr *)&cli_addr,&length))<0){
        printf("Error escuchando\n");
        return 0;
    }

    return socketfd;
}



void* waitting_process(){
    while(1){
        sleep(1);
        if(wake_up==0) {
            missing--;
            char buffer[90];
            sprintf(buffer, "ALARMA DENTRO DE %d", missing);// enviar la alarma al bot
            send(socketfd, buffer, strlen(buffer), 0);
        }
        if(missing==0 && wake_up ==0){
            missing = time_to_wait;
            nivlem_wake_up();
            send(socketfd,"QUITAR HUEVOS\n",15,0);

        }
    }
    return NULL;
}

void * come_back_process(){
    char buffer[50];
    sprintf(buffer,"SE RECOGIERON %d HUEVOS",eggs_amount);
    send(socketfd,buffer,strlen(buffer),0);
    eggs_amount = 0;
    nivlem_come_back();
    return NULL;
}

void * wake_up_process(){
    nivlem_wake_up();
    send(socketfd,"QUITAR HUEVOS\n",15,0);
    return NULL;
}


int main() {

    socketfd = listen_conections();
    printf("Se creó una conexion \n");

    // RECIBIR LOS COSTOS Y EL MAXIMO.
    recv(socketfd, buffer, 200, 0);
    sscanf(buffer, "MAXIMO HUEVOS %d\nTIEMPO DE ESPERA %d\n", &max_eggs_allowed, &time_to_wait);

    //int max= max_eggs_allowed;
    printf("DATOS NIVLEM:\nMAXIMO DE HUEVOS: %d\nTIEMPO DE ESPERA %d\n\n", max_eggs_allowed, time_to_wait);


    memset(&buffer, 0, sizeof(buffer));

    pthread_t graphics_thread;
    pthread_create(&graphics_thread, NULL, &init_graphics, NULL);


    missing = time_to_wait;
    //PROCESO QUE ESPERA HASTA QUE PASE EL TIEMPO PREDEFINIDO
    pthread_t waitting_thread;
    if(pthread_create(&waitting_thread, NULL, waitting_process, NULL)) printf("ERROR CREADO EL HILO\n");
    // LEER SI SE LLEGA AL MAXIMO DE HUEVO

    pthread_t nivlem_wake_up_thread;

    memset(&buffer, 0, sizeof(buffer));
    while (1) {
        //max_eggs_allowed=max;
        recv(socketfd, buffer, 200, 0);
        if (strstr(buffer,"RESTAR UN HUEVO\n")) {
            eggs_amount++;
            if ((eggs_amount >= max_eggs_allowed) && (wake_up ==0)) {
               // printf("Se ha llegado al maximo de huevos, se recogeran\n");
                pthread_create(&nivlem_wake_up_thread,NULL,wake_up_process,NULL);
            }
        }
        else if(strstr(buffer,"SE QUITARON LOS HUEVOS\n")){
            pthread_t nivlem_come_back_thread;
            missing = time_to_wait;
            pthread_create(&nivlem_come_back_thread,NULL,come_back_process,NULL);
        }
        else if(strstr(buffer,"SE AUMENTO EL COSTO DEL AGUA")){
            sscanf(buffer,"SE AUMENTO EL COSTO DEL AGUA A %d\n",&water_actual_cost);
        }
        else if(strstr(buffer,"SE AUMENTO EL COSTO DEL CONCENTRADO")){
            sscanf(buffer,"SE AUMENTO EL COSTO DEL CONCENTRADO A %d\n",&food_actual_cost);
        }

        memset(&buffer, 0, sizeof(buffer));
    }
    return 0;
}