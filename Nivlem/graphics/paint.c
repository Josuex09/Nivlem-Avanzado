//
// Created by josue on 11/11/15.
//

#include "paint.h"


extern int eggs_amount;
extern long int missing;
extern int water_actual_cost;
extern int food_actual_cost;
int wake_up = 0; // bool para saber si el nivlem esta despierto o dormido
nivlem_g *nivlem;
char eggs_buffer[10];
char water_actual_cost_buffer[10];
char food_actual_cost_buffer[10];
char time_buffer[100];
ALLEGRO_DISPLAY *display;
ALLEGRO_BITMAP *house_1,*house_2,*nivlem_1,*nivlem_2,*nivlem_3,*nivlem_4,*nivlem_5,*nivlem_6;
ALLEGRO_SAMPLE *alarm_sound;
ALLEGRO_FONT *font1,*font2,*font3;


void* init_graphics(){

    nivlem = malloc(sizeof(nivlem_g));

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
    al_init_font_addon();
    al_init_ttf_addon();

    display = al_create_display(SCREEN_WIDTH,SCREEN_HEIGHT);
    if(!display) {
        printf("Error display\n");
        return NULL;
    }

    //Creacion de todas las pantallas.
    house_1 = al_load_bitmap(HOUSE1_PATH);
    house_2 = al_load_bitmap(HOUSE2_PATH);
    nivlem_1 = al_load_bitmap(NIVLEM1_PATH);
    nivlem_2 = al_load_bitmap(NIVLEM2_PATH);
    nivlem_3 = al_load_bitmap(NIVLEM3_PATH);
    nivlem_4 = al_load_bitmap(NIVLEM4_PATH);
    nivlem_5 = al_load_bitmap(NIVLEM5_PATH);
    nivlem_6 = al_load_bitmap(NIVLEM6_PATH);

    //Creacion de todos los sonidos
    alarm_sound = al_load_sample(ALARM_PATH);

    //Creacion de las fuentes
    font1 = al_load_ttf_font("DejaVuSans.ttf",32,0);
    font2 = al_load_ttf_font("DejaVuSans.ttf",50,0);
    font3 = al_load_ttf_font("DejaVuSans.ttf",14,0);

    //  al_play_sample(chickens_sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
    while(1) {
        draw_background();
        draw_eggs();
        //draw_time();
        draw_nivlem();
        draw_costs();
        al_flip_display();
        al_clear_to_color(al_map_rgb(0, 0, 0));
    }
    return NULL;
}

void draw_background(){
    if(wake_up){
        al_draw_bitmap(house_1,0,0,0);
    }
    else
        al_draw_bitmap(house_2,0,0,0);
}
void draw_nivlem(){
    if(wake_up){
        al_draw_bitmap(nivlem->img,nivlem->x,nivlem->y,0);
    }
}


void draw_time(){
    sprintf(time_buffer,"%lu",missing);
    al_draw_text(font2,al_map_rgb(0,0,0),80,60,ALLEGRO_ALIGN_LEFT,time_buffer);

}

void draw_eggs(){
    sprintf(eggs_buffer,"%d",eggs_amount);
    al_draw_text(font1,al_map_rgb(0,0,0),60,2,ALLEGRO_ALIGN_LEFT,eggs_buffer);

}

void draw_costs(){
    sprintf(water_actual_cost_buffer,"%d",water_actual_cost);
    sprintf(food_actual_cost_buffer,"%d",food_actual_cost);
    al_draw_text(font3,al_map_rgb(0,0,0),28,50,ALLEGRO_ALIGN_LEFT,"COSTOS:");
    al_draw_text(font3,al_map_rgb(0,0,0),28,90,ALLEGRO_ALIGN_LEFT,"AGUA:");
    al_draw_text(font3,al_map_rgb(0,0,0),28,110,ALLEGRO_ALIGN_LEFT,"ALIMENTO:");
    al_draw_text(font3,al_map_rgb(0,0,0),110,90,ALLEGRO_ALIGN_LEFT,water_actual_cost_buffer);
    al_draw_text(font3,al_map_rgb(0,0,0),110,110,ALLEGRO_ALIGN_LEFT,food_actual_cost_buffer);
}


void nivlem_wake_up(){
    if(wake_up == 1)return;
    wake_up =1;
    al_play_sample(alarm_sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
    // VALORES INICIALES
    nivlem->x = NIVLEM_X;
    nivlem->y = NIVLEM_Y;
    nivlem->img = nivlem_5;
    sleep(1);

    int n = 0;
    while(nivlem->x< 600){
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

}

void nivlem_come_back(){
    int n = 0;
    sleep(6);
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
    wake_up=0;
}