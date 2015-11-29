#include "stdio.h"
#include  "sys/socket.h"
#include "netinet/in.h"
#include "string.h"
#include "aux/list.h"
#include "aux/calc.h"
#include "unistd.h"
#include "pthread.h"
#include "graphics/paint.h"
#include "aux/tcp_connection.h"

#define PORT 3334

typedef struct {
    pthread_t id;
    pthread_mutex_t m;
} Chickens;

Chickens * chicken_list;
char buffer[500];

int socketfd,water_poisson,food_poisson,egg_poisson,chicken_length,max_range_water,min_range_water,max_range_food,min_range_food;
list_t water_distribution;
list_t food_distribution;
list_t egg_distribution;

pthread_mutex_t	food_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t	water_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t	egg_mutex = PTHREAD_MUTEX_INITIALIZER;

void calcDistr(list_t * lista, double poisson_value){
    long double a=0;
    int i=1;
    while(a<0.99){
        double next = poisson(i,poisson_value);
        if(a+next>1.0)
            break;
        //f(a+next < a+0.000001)
        //    break; // un aproximado para dejar de sacar probas pequenas a+=next; add(&egg_distribution,a); i++; } }
        a+=next;
        add(lista,a);
        i++;
    }
}

int next_food_wait(){
    double random = get_random();
    int i;
    for(i = 0; i < food_distribution.size; i++){
        if(random<get(&food_distribution,i)) return i+1;
    }
    return food_distribution.size;

}

int next_water_wait(){
    double random = get_random();
    int i;
    for(i = 0; i < water_distribution.size; i++){
        if(random<get(&water_distribution,i)) return i+1;
    }
    return water_distribution.size;

}
int next_egg_wait(){
    double random = get_random();
    int i;
    for(i = 0; i < egg_distribution.size; i++){
        if(random<get(&egg_distribution,i)) return i+1;
    }
    return egg_distribution.size;

}
int listen_conections() {
    int listenfd, socketfd;
    socklen_t length = NULL;
    static struct sockaddr_in cli_addr;
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return 0;

    static struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT);

    if (bind(listenfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("Error con el bind\n");
        exit(0);
    }

    if (listen(listenfd, 1) < 0) {
        printf("Error con el listen\n");
        return 0;
    }

    length = sizeof(cli_addr);

    if ((socketfd = accept(listenfd, (struct sockaddr *) &cli_addr, &length)) < 0) {
        printf("Error escuchando\n");
        return 0;
    }

    return socketfd;
}

void * water_proc(void* id_t){
    int id = *((int *)id_t);
    int next;
    int random;
    char  buffer[40];
    while(1){
        next = next_water_wait();
        //printf("La gallina %d se dormira %d \n",id+1,next);
        sleep(next);
        random = random_between(min_range_water,max_range_water);
        sprintf(buffer,"RESTAR %d DE AGUA\n",random);
        pthread_mutex_lock(&chicken_list[id].m);
        pthread_mutex_lock(&water_mutex);

        //posiciones iniciales en pantalla de la gallina que se movera
        int initial_x = get_chicken_x(id);
        int initial_y = get_chicken_y(id);
        // dibujar en pantalla como la gallina se mueve hacia el agua
        water_move(id);
        // enviar la senal de que se tomo agua
        printf("La gallina %d tomó %d de agua\n", id+1,random);
        sendTo(socketfd,buffer);

        //dibujar en pantalla como la gallina vuelve al centro
        water_move_back(id,initial_x,initial_y);

        pthread_mutex_unlock(&chicken_list[id].m);
        pthread_mutex_unlock(&water_mutex);
    }
    return NULL;
}
void * food_proc(void* id_t){
    int id = *((int *)id_t);
    int next;
    int random;
    char  buffer[40];
    while(1){
        next = next_food_wait();
        // printf("La gallina %d se dormira %d \n",id+1,next);
        sleep(next);
        random = random_between(min_range_food,max_range_food);
        sprintf(buffer,"RESTAR %d DE COMIDA\n",random);
        pthread_mutex_lock(&chicken_list[id].m);
        pthread_mutex_lock(&food_mutex);

        //posiciones iniciales en pantalla de la gallina que se movera
        int initial_x = get_chicken_x(id);
        int initial_y = get_chicken_y(id);
        // dibujar en pantalla como la gallina se mueve hacia el concentrado
        food_move(id);
        //enviar la senal
        printf("La gallina %d comió %d de concentrado\n", id+1,random);
        sendTo(socketfd,buffer);
        // dibujar en pantalla como la gallina se mueve hacia el grupo del centro
        food_move_back(id,initial_x,initial_y);

        pthread_mutex_unlock(&chicken_list[id].m);
        pthread_mutex_unlock(&food_mutex);
    }
    return NULL;
}
void* chicken_process(void* id_t){
    int id = *((int *)id_t);
    int next;
    pthread_t food_process =(pthread_t) malloc(sizeof(pthread_t));
    pthread_t water_process =(pthread_t) malloc(sizeof(pthread_t));

    pthread_create(&food_process,NULL,food_proc, (void *) &id); // hilo que maneja el consumo de concentrado.
    pthread_create(&water_process,NULL,water_proc, (void *) &id); // hilo que maneja el consumo de agua.

    // el proceso de los huevos se maneja aqui.
    char buffer[200];
    while(1){
        next = next_egg_wait();
        //printf("La gallina %d se dormira %d \n",id+1,next);
        sleep(next);
        sprintf(buffer,"RESTAR UN HUEVO\n");
        pthread_mutex_lock(&chicken_list[id].m);
        pthread_mutex_lock(&egg_mutex);

        //posiciones iniciales en pantalla de la gallina que se movera
        int initial_x = get_chicken_x(id);
        int initial_y = get_chicken_y(id);

        //dibujar en pantalla como la gallina se mueve hacia la canasta de huevos
        basket_move(id);
        //enviar la senal
        printf("La gallina %d puso un huevo\n", id + 1);
        sendTo(socketfd, buffer);
        //dibujar en pantalla como la gallina se mueve hacia el centro
        basket_move_back(id,initial_x,initial_y);

        pthread_mutex_unlock(&chicken_list[id].m);
        pthread_mutex_unlock(&egg_mutex);
    }

    return NULL;
}

void createChickens(){
    //printf("Se crearon %d gallinas\n",inputs[CHICKEN_LENGTH]);
    int *id;
    chicken_list = calloc(chicken_length,sizeof(Chickens));;
    int i;
    for (i = 0; i < chicken_length; i++) {  //crear los procesos para las gallinas con un id secuencial.
        id = malloc(sizeof(int));
        *id = i;
        pthread_mutex_init(&chicken_list[i].m, NULL);
        if (pthread_create(&chicken_list[i].id, NULL, &chicken_process, (void *) id)) {
            printf("Error creando la gallina con id: %d\n",i+1);
        }
    }
}



int main(){


    srand(time(NULL));
    socketfd = listen_conections();
    printf("Se creó una conexion \n");

    // recibir la infromacion de las gallinas
    recv(socketfd,buffer,200,0);

    sscanf(buffer,"POISSON AGUA %d\nPOISSON CONCENTRADO %d\nPOISSON HUEVOS %d\nCANTIDAD GALLINAS %d\nMIN AGUA %d\nMAX AGUA %d\nMIN CONCENTRADO %d\nMAX CONCENTRADO %d\n",&water_poisson,&food_poisson,&egg_poisson,&chicken_length,&min_range_water,&max_range_water,&min_range_food,&max_range_food);

    calcDistr(&water_distribution,water_poisson);
    calcDistr(&food_distribution,food_poisson);
    calcDistr(&egg_distribution,egg_poisson);

    printf(buffer,"POISSON AGUA %d\nPOISSON CONCENTRADO %d\nPOISSON HUEVOS %d\nCANTIDAD GALLINAS %d\nMIN AGUA %d\nMAX AGUA %d\nMIN CONCENTRADO %d\nMAX CONCENTRADO %d\n",water_poisson,food_poisson,egg_poisson,chicken_length,min_range_water,max_range_water,min_range_food,max_range_food);
    createChickens();

    sleep(0.5);

    pthread_t graphics_thread;
    pthread_create(&graphics_thread,NULL,&init_graphics,(void *) &chicken_length);

    char buffer [200];

    while(1){
        recv(socketfd,buffer,200,0);
        if(strstr(buffer,"QUITAR HUEVOS\n")){
            nivlem_wake_up();
            sendTo(socketfd,"SE QUITARON LOS HUEVOS\n");
            nivlem_come_back();
        }
    }
    return 0;

}