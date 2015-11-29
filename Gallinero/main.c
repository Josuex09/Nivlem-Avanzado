#include "stdio.h"
#include "pthread.h"
#include "aux/tcp_conection.h"

#define INPUTS "input.txt"
#define HOSTS "hosts.txt"



#define CHICKEN_LENGTH 0
#define WATER_MIN_RANGE 1
#define WATER_MAX_RANGE 2
#define FOOD_MIN_RANGE 3
#define FOOD_MAX_RANGE 4
#define WATER_POISSON 5
#define FOOD_POISSON 6
#define EGG_POISSON 7
#define FOOD_MIN_ALLOWED 8
#define FOOD_REFILL_AMOUNT 9
#define WATER_MIN_ALLOWED 10
#define WATER_REFILL_AMOUNT 11
#define EGG_MAX_ALLOWED 12
#define WATER_COST 13
#define FOOD_COST 14

#define NIVLEM_WAIT 21600

int inputs[15];

char bot_host[10];
char chicken_host[10];
char nivlem_host[10];
int bot_port=0;
int chicken_port=0;
int nivlem_port=0;


int bot_fd,chickens_fd,nivlem_fd;


void readInput(){
    FILE* file,*file2;
    // leer los inputs con poisson, rangos, etc.
    file = fopen (INPUTS, "r");
    int i = 0;
    int j = 0;
    while (!feof (file)) {
        fscanf (file, "%d", &i);
        inputs[j] = i;
        j++;
    }
    fclose (file);
    //leer los inputs con hosts y puertos
    file2 = fopen(HOSTS,"r");
    fscanf (file2, "%s %d %*s",bot_host,&bot_port);
    fscanf (file2, "%s %d %*s",chicken_host,&chicken_port);
    fscanf (file2, "%s %d %*s",nivlem_host,&nivlem_port);
    fclose (file2);
}




void * recv_bot(){ // recibir del nivlem si se llego al maximo de huevos o el estado actual de la alarma
    char bot_msg[50];
    while(1) {
        recv(bot_fd, bot_msg, 50, 0);
        if (strstr(bot_msg, "AGUA") || strstr(bot_msg,"CONCENTRADO")) {
            sendTo(nivlem_fd,bot_msg);
        }
        memset(&bot_msg, 0, sizeof(bot_msg));
    }
}
void * recv_nivlem(){ // recibir del nivlem si se llego al maximo de huevos o el estado actual de la alarma
    char nivlem_msg[50];
    while(1) {
        recv(nivlem_fd, nivlem_msg, 50, 0);
        if (strstr(nivlem_msg, "QUITAR HUEVOS\n")) {
            sendTo(chickens_fd,nivlem_msg);
        }
        else if (strstr(nivlem_msg,"ALARMA DENTRO DE")){
            sendTo(bot_fd,nivlem_msg);
        }
        else if(strstr(nivlem_msg,"SE RECOGIERON")){
            sendTo(bot_fd,nivlem_msg);
        }
        memset(&nivlem_msg[0], 0, sizeof(nivlem_msg));
    }
}

void recv_chickens(){
// recibir los mensajes de las acciones de las gallinas
    char  chicken_msg[200];
    while(1){
        recv(chickens_fd,chicken_msg,50,0);
        if(strstr(chicken_msg,"COMIDA")){
            sendTo(bot_fd,chicken_msg);
        }
//        else if(strstr(chicken_msg,"SE QUITARON LOS HUEVOS\n")){
//            sendTo(nivlem_fd,chicken_msg);
//        }
        else if(strstr(chicken_msg,"HUEVO")){
            sendTo(nivlem_fd,chicken_msg);
        }

        else if(strstr(chicken_msg,"AGUA")){
            sendTo(bot_fd,chicken_msg);
        }
        memset(&chicken_msg, 0, sizeof(chicken_msg));


    }
}

int main(){
    readInput();  //leer el archivo de entrada, en el primer macro de este archivo se puede cambiar el nombre.

    //crear conexion con los demas procesos
    chickens_fd = create_tcp_conection(chicken_host,chicken_port);
    bot_fd = create_tcp_conection(bot_host,bot_port);
    nivlem_fd = create_tcp_conection(nivlem_host,nivlem_port);

    if(chickens_fd ==-1 && bot_fd == -1 && nivlem_fd == -1)
        return 0;


    // enviar los maximos y costos.
    char bot_info [150];
    sprintf(bot_info,"COSTO AGUA %d\nCOSTO CONCENTRADO %d\nMINIMO DE AGUA %d\nMINIMO DE CONCENTRADO %d\nRECARGA DE AGUA %d\nRECARGA DE CONCENTRADO %d",inputs[WATER_COST],inputs[FOOD_COST],inputs[WATER_MIN_ALLOWED],inputs[FOOD_MIN_ALLOWED],inputs[WATER_REFILL_AMOUNT],inputs[FOOD_REFILL_AMOUNT]);
    sendTo(bot_fd,bot_info);

    //enviar los poisson y info de para crear las gallinas
    char chickens_info [200];
    sprintf(chickens_info,"POISSON AGUA %d\nPOISSON CONCENTRADO %d\nPOISSON HUEVOS %d\nCANTIDAD GALLINAS %d\nMIN AGUA %d\nMAX AGUA %d\nMIN CONCENTRADO %d\nMAX CONCENTRADO %d\n",inputs[WATER_POISSON],inputs[FOOD_POISSON],inputs[EGG_POISSON],inputs[CHICKEN_LENGTH],inputs[WATER_MIN_RANGE],inputs[WATER_MAX_RANGE],inputs[FOOD_MIN_RANGE],inputs[FOOD_MAX_RANGE]);
    sendTo(chickens_fd,chickens_info);

    //enviar el tiempo q espera nivlem y el maximo de huevos permitido
    char nivlem_info [100];
    sprintf(nivlem_info,"MAXIMO HUEVOS %d\nTIEMPO DE ESPERA %d\n",inputs[EGG_MAX_ALLOWED],NIVLEM_WAIT);
    sendTo(nivlem_fd,nivlem_info);






   // escuchar al nivlem  
    pthread_t recv_nivlem_thread;
    pthread_create(&recv_nivlem_thread,NULL,&recv_nivlem,NULL);


    //escuchar al bot
    pthread_t recv_bot_thread;
    pthread_create(&recv_bot_thread,NULL,&recv_bot,NULL);
    // escuchar a las gallinas
    recv_chickens();

    return 0;

}

