//
// Created by josue on 11/9/15.
//

#include "paint.h"

ALLEGRO_DISPLAY *display;
ALLEGRO_BITMAP *background_image,*egg_image,*chicken_image1,*chicken_image2,*chicken_image3,*chancho_image;
ALLEGRO_BITMAP *chicken_image4,*chicken_image5,*chicken_image6,* chicken_image_back1,*chicken_image_back2,*chicken_image_back3;
ALLEGRO_BITMAP *nivlem_1,*nivlem_2,*nivlem_3,*nivlem_4,*nivlem_5,*nivlem_6,*chicken_image_front1,*chicken_image_front2,*chicken_image_front3;
ALLEGRO_MUTEX  *al_mutex;
ALLEGRO_SAMPLE *chickens_sound=NULL;

int chicken_length_g;
//chichen list
chicken_g * chicken_list_g = NULL;


nivlem_g * nivlem;
const int eggs_max =49;
egg_g * egg_list_g = NULL; // max permitido de huevos en vez de 10

//mover la posicion del nuevo huevo
int next_egg_x=0;
int next_egg_y=0;
int eggs_in_basket = 0;


int get_chicken_x(int id){
    return chicken_list_g[id].x;

}

int get_chicken_y(int id){
    return chicken_list_g[id].y;
}
void * init_graphics(void*  length){
    chicken_length_g = *((int *)length);
    nivlem = malloc(sizeof(nivlem_g));

    chicken_list_g = malloc(sizeof(chicken_g)*chicken_length_g);
    egg_list_g = malloc(sizeof(egg_g)*eggs_max);
    if(!al_init()) {
        printf("Error al_init\n");
        return NULL;
    }
    if(!al_init_image_addon()) {
        printf("Error init_image_addon\n");
        return NULL;
    }
    if(!al_install_audio()){
        fprintf(stderr, "failed to initialize audio!\n");
        return NULL;
    }

    if(!al_init_acodec_addon()){
        fprintf(stderr, "failed to initialize audio codecs!\n");
        return NULL;
    }

    if (!al_reserve_samples(1)){
        fprintf(stderr, "failed to reserve samples!\n");
        return NULL;
    }

    display = al_create_display(SCREEN_WIDTH,SCREEN_HEIGHT);
    if(!display) {
        printf("Error display\n");
        return NULL;
    }

    //Creacion de todas las pantallas.
    background_image = al_load_bitmap(BACKGROUND_IMAGE_PATH);
    chicken_image1 = al_load_bitmap(CHICKEN1_IMAGE_PATH);
    chicken_image2 = al_load_bitmap(CHICKEN2_IMAGE_PATH);
    chicken_image3 = al_load_bitmap(CHICKEN3_IMAGE_PATH);
    chicken_image4 = al_load_bitmap(CHICKEN4_IMAGE_PATH);
    chicken_image5 = al_load_bitmap(CHICKEN5_IMAGE_PATH);
    chicken_image6 = al_load_bitmap(CHICKEN6_IMAGE_PATH);

    chicken_image_back1 = al_load_bitmap(CHICKENBACK1_IMAGE_PATH);
    chicken_image_back2 = al_load_bitmap(CHICKENBACK2_IMAGE_PATH);
    chicken_image_back3 = al_load_bitmap(CHICKENBACK3_IMAGE_PATH);

    chicken_image_front1 = al_load_bitmap(CHICKENFRONT1_IMAGE_PATH);
    chicken_image_front2 = al_load_bitmap(CHICKENFRONT2_IMAGE_PATH);
    chicken_image_front3 = al_load_bitmap(CHICKENFRONT3_IMAGE_PATH);

    chancho_image = al_load_bitmap(CHANCHO_IMAGE_PATH);
    egg_image = al_load_bitmap(EGG_IMAGE_PATH);
    nivlem_1 = al_load_bitmap(NIVLEM1_PATH);
    nivlem_2 = al_load_bitmap(NIVLEM2_PATH);
    nivlem_3 = al_load_bitmap(NIVLEM3_PATH);
    nivlem_4 = al_load_bitmap(NIVLEM4_PATH);
    nivlem_5 = al_load_bitmap(NIVLEM5_PATH);
    nivlem_6 = al_load_bitmap(NIVLEM6_PATH);

    nivlem->img = nivlem_5;
    nivlem->x=NIVLEM_X;
    nivlem->y=NIVLEM_Y;

    //Creacion de todos los sonidos
    chickens_sound = al_load_sample(CHICKEN_AUDIO_PATH);


    //CREAR ARREGLO CON GALLINAS
    int i;
    int move_x,move_y =0;
    for (i = 0; i < chicken_length_g; i++) {
        chicken_list_g[i].id = i+1;
        chicken_list_g[i].img = chicken_image1;
        chicken_list_g[i].x = INITIAL_CHICKEN_X+move_x;
        chicken_list_g[i].y = INITIAL_CHICKEN_Y+move_y;
        move_x+=20;
        if(move_x>200){
            move_x=0;
            move_y+=30;
        }
    }
    al_mutex = al_create_mutex(); // no hace nada ya que solo bloquea aqui.

    al_play_sample(chickens_sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
    while(1) {
//        al_lock_mutex(al_mutex);
        al_draw_bitmap(background_image, 0, 0, 0);
        al_draw_bitmap(chancho_image,390,200,0);
        draw_eggs();
        draw_chickens();
        draw_nivlem();
        al_flip_display();
        al_clear_to_color(al_map_rgb(0, 0, 0));
  //      al_unlock_mutex(al_mutex);
    }
    return NULL;
}

void draw_chickens(){
    int i;
    for (i = 0; i < chicken_length_g; i++) {
        al_draw_bitmap(chicken_list_g[i].img,chicken_list_g[i].x,chicken_list_g[i].y,0);
      //  if(i==0)printf("posicion actual x: %d\n",chicken_list_g[i].x);
    }
}

void draw_eggs(){
    int i;
    for (i = 0; i < eggs_in_basket; i++) {
        al_draw_bitmap(egg_image,egg_list_g[i].x,egg_list_g[i].y,0);
    }
}


void food_move(int id){
    if(chicken_list_g[id].x<=FOOD_X){
        move_right(id,FOOD_X);
        move_up(id,WALL_Y);
    }
    else{
        move_left(id,FOOD_X);
        move_up(id,WALL_Y);

    }
}

void food_move_back(int id,int initial_x,int initial_y){
    if(initial_x<=FOOD_X){
        move_down(id,initial_y);
        move_left(id,initial_x);
    }
    else{
        move_down(id,initial_y);
        move_right(id,initial_x);
    }
}

void  water_move(int id){
    if(chicken_list_g[id].x<=WATER_X){
        move_right(id,WATER_X);
        move_up(id,WALL_Y);
    }
    else{
        move_left(id,WATER_X);
        move_up(id,WALL_Y);
    }
}

void water_move_back(int id,int initial_x,int initial_y){
    if(initial_x<=WATER_X){
        move_down(id,initial_y);
        move_left(id,initial_x);
    }
    else{
        move_down(id,initial_y);
        move_right(id,initial_x);
    }
}


void basket_move(int id){
    // el basket siempre estara a la izquierda de las gallinas entonces
    // no hay que validar para q lado moverse.
    move_left(id,BASKET_X);
    move_up(id,WALL_Y);
    new_egg(); // poner un huevo.
}

void basket_move_back(int id,int initial_x,int initial_y){
    move_down(id,initial_y);
    move_right(id,initial_x);
}

void new_egg(){
    if(eggs_in_basket<= eggs_max) {
        egg_list_g[eggs_in_basket].x = EGG_X + next_egg_x;
        egg_list_g[eggs_in_basket].y = EGG_Y + next_egg_y;
        eggs_in_basket++;
        next_egg_x += 20;
        if (next_egg_x >= 200) {
            next_egg_x = 0;
            next_egg_y -= 27;
        }
    }
}
void move_right(int id,int value){

    chicken_list_g[id].img = chicken_image1;
    int n = 0;
    while (chicken_list_g[id].x < value) { //moverse a la direccion del agua
        chicken_list_g[id].x++;
        n++;
        if(n == 5) // CAMBIAR DE IMAGEN PARA QUE SEA UNA SECUENCIA DE COMO CAMINA LA GALLINA.
            chicken_list_g[id].img = chicken_image2;
        else if(n== 10)
            chicken_list_g[id].img = chicken_image1;
        else if(n == 15) {
            chicken_list_g[id].img = chicken_image3;
            n = 0;
        }
        al_rest(0.01);
    }

}
void move_left(int id,int value){
    chicken_list_g[id].img = chicken_image4;
    int n =0;
    while (chicken_list_g[id].x > value) { //moverse a la direccion del agua
        chicken_list_g[id].x--;
        n++;
        if(n == 5) // CAMBIAR DE IMAGEN PARA QUE SEA UNA SECUENCIA DE COMO CAMINA LA GALLINA.
            chicken_list_g[id].img = chicken_image5;
        else if( n == 10)
            chicken_list_g[id].img = chicken_image4;
        else if( n == 15) {
            chicken_list_g[id].img = chicken_image6;
            n = 0;
        }
        al_rest(0.01);
    }
}
void move_up(int id,int value){
    chicken_list_g[id].img = chicken_image_back1;
    int n=0;
    while(chicken_list_g[id].y>=value){ // ir al frente del agua
        chicken_list_g[id].y--;
        n++;
        if(n == 5)
            chicken_list_g[id].img = chicken_image_back2;
        else if(n == 10)
            chicken_list_g[id].img = chicken_image_back1;
        else if(n == 15){
            chicken_list_g[id].img = chicken_image_back3;
            n=0;
        }
        al_rest(0.01);
    }

}void move_down(int id,int value){
    chicken_list_g[id].img = chicken_image_front1;
    int n=0;
    while(chicken_list_g[id].y<=value){ // volver al centro del gallinero
        chicken_list_g[id].y++;
        n++;
        if(n ==5)
            chicken_list_g[id].img = chicken_image_front2;
        else if(n == 10)
            chicken_list_g[id].img = chicken_image_front1;
        else if(n == 15){
            chicken_list_g[id].img = chicken_image_front3;
            n=0;
        }
        al_rest(0.01);
    }

}

void draw_nivlem(){
    al_draw_bitmap(nivlem->img,nivlem->x,nivlem->y,0);
}


void nivlem_wake_up(){
    // VALORES INICIALES
    nivlem->x = NIVLEM_X;
    nivlem->y = NIVLEM_Y;
    nivlem->img = nivlem_5;
    sleep(1);

    int n = 0;
    while(nivlem->x< -50){
        nivlem->x++;
        n++;
        if(n == 10)
            nivlem->img = nivlem_4;
        else if(n == 20)
            nivlem->img = nivlem_5;
        else if(n == 30) {
            nivlem->img = nivlem_6;
            n = 0;
        }

        al_rest(0.01);
    }
    nivlem->img = nivlem_5;
    eggs_in_basket = 0;
    next_egg_x = 0;
    next_egg_y =0;
}

void nivlem_come_back(){
    sleep(1);
    int n = 0;
    while(nivlem->x> NIVLEM_X){
        nivlem->x--;
        n++;
        if(n == 10)
            nivlem->img = nivlem_1;
        else if(n == 20)
            nivlem->img = nivlem_2;
        else if(n == 30) {
            nivlem->img = nivlem_3;
            n = 0;
        }
        al_rest(0.01);
    }
}