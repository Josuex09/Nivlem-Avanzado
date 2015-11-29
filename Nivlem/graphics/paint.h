//
// Created by josue on 11/11/15.
//

#ifndef NIVLEMWITHALLEGRO_PAINT_H
#define NIVLEMWITHALLEGRO_PAINT_H



#include "stdio.h"
#include "unistd.h"
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define HOUSE1_PATH "images/casa.png"
#define HOUSE2_PATH "images/casa2.png"
#define NIVLEM1_PATH "images/nivlem.png"
#define NIVLEM2_PATH "images/nivlem2.png"
#define NIVLEM3_PATH "images/nivlem3.png"
#define NIVLEM4_PATH "images/nivlem4.png"
#define NIVLEM5_PATH "images/nivlem5.png"
#define NIVLEM6_PATH "images/nivlem6.png"
#define ALARM_PATH "sounds/alarm.wav"
#define NIVLEM_X 80
#define NIVLEM_Y 100

typedef struct {  // Tipo para mantener los hilos
    ALLEGRO_BITMAP * img;
    int x,y;
} nivlem_g;

void * init_graphics();
void draw_background();
void draw_nivlem();
void draw_time();
void draw_eggs();
void draw_costs();
void nivlem_wake_up();
void nivlem_come_back();

#endif //NIVLEMWITHALLEGRO_PAINT_H
