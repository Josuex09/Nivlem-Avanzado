//
// Created by josue on 11/9/15.
//

#ifndef NIVLEM_PAINT_H
#define NIVLEM_PAINT_H

#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "stdio.h"


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define BACKGROUND_IMAGE_PATH "images/Gallinero.png"

#define CHICKEN1_IMAGE_PATH "images/gallina.png"
#define CHICKEN2_IMAGE_PATH "images/gallina1.png"
#define CHICKEN3_IMAGE_PATH "images/gallina2.png"
#define CHICKEN4_IMAGE_PATH "images/gallinaleft.png"
#define CHICKEN5_IMAGE_PATH "images/gallina1left.png"
#define CHICKEN6_IMAGE_PATH "images/gallina2left.png"

#define CHANCHO_IMAGE_PATH "images/chancho.png"



#define CHICKENBACK1_IMAGE_PATH "images/gallinaatras.png"
#define CHICKENBACK2_IMAGE_PATH "images/gallinaatras2.png"
#define CHICKENBACK3_IMAGE_PATH "images/gallinaatras3.png"


#define CHICKENFRONT1_IMAGE_PATH "images/gallinaadelante.png"
#define CHICKENFRONT2_IMAGE_PATH "images/gallinaadelante2.png"
#define CHICKENFRONT3_IMAGE_PATH "images/gallinaadelante3.png"


#define EGG_IMAGE_PATH "images/huevo.png"
#define NIVLEM1_PATH "images/nivlem.png"
#define NIVLEM2_PATH "images/nivlem2.png"
#define NIVLEM3_PATH "images/nivlem3.png"
#define NIVLEM4_PATH "images/nivlem4.png"
#define NIVLEM5_PATH "images/nivlem5.png"
#define NIVLEM6_PATH "images/nivlem6.png"
#define CHICKEN_AUDIO_PATH "sounds/chickens.wav"

#define INITIAL_CHICKEN_X 120
#define INITIAL_CHICKEN_Y 300

#define WATER_X 270
#define FOOD_X 500
#define BASKET_X 80
#define EGG_X 20
#define EGG_Y 235
#define WALL_Y 200
#define NIVLEM_X -200
#define NIVLEM_Y 50

typedef struct {  // Tipo para mantener los hilos
    ALLEGRO_BITMAP * img;
    int x,y;
} nivlem_g;


typedef struct {  // Tipo para mantener los hilos
    int id;
    ALLEGRO_BITMAP * img;
    int x,y;
} chicken_g;

typedef struct{
    int x;
    int y;
} egg_g;

void draw_chickens();
void draw_eggs();
void * init_graphics(void*  length);

void water_move(int id);
void water_move_back(int id,int initial_x,int initial_y);

void food_move(int id);
void food_move_back(int id,int initial_x,int initial_y);


void basket_move(int id);
void basket_move_back(int id,int initial_x,int initial_y);

void move_left(int id,int x);
void move_right(int id,int x);
void move_up(int id,int x);
void move_down(int id,int x);
void new_egg();
void draw_nivlem();
void nivlem_wake_up();
void nivlem_come_back();
int get_chicken_x(int id);
int get_chicken_y(int id);
#endif //NIVLEM_PAINT_H
