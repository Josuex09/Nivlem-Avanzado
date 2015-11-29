//
// Created by josue on 11/19/15.
//

#include "paint.h"
ALLEGRO_DISPLAY *display;
ALLEGRO_BITMAP *background;
ALLEGRO_FONT *font1,*font2,*font3;
extern int hours;
extern int minutes;
extern int seconds;
extern long total_water_used;
extern long total_food_used;
extern int total_eggs_picked;
extern int actual_water;
extern int actual_food;

void* init_graphics(){
    if(!al_init()) {
        printf("Error al_init\n");
        return NULL;
    }
    if(!al_init_image_addon()) {
        printf("Error init_image_addon\n");
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
    background = al_load_bitmap(BACKGROUND_IMAGE_PATH);


    //Creacion de las fuentes
    font1 = al_load_ttf_font("fonts/timefont.ttf",80,0);
    font2 = al_load_ttf_font("fonts/DejaVuSans.ttf",20,0);
    font3 = al_load_ttf_font("fonts/DejaVuSans.ttf",12,0);

    while(1) {
        al_draw_bitmap(background,0,0,0);

        al_draw_textf(font1,al_map_rgb(0,0,0),20,50,ALLEGRO_ALIGN_LEFT,"%d : %d : %d",hours,minutes,seconds);

        al_draw_text(font3,al_map_rgb(0,0,0),492,50,ALLEGRO_ALIGN_RIGHT,"CONCENTRADO ACTUAL:");
        al_draw_textf(font3,al_map_rgb(0,0,0),564,50,ALLEGRO_ALIGN_RIGHT,"%d g",actual_food);

        al_draw_text(font3,al_map_rgb(0,0,0),435,100,ALLEGRO_ALIGN_RIGHT,"AGUA ACTUAL:");
        al_draw_textf(font3,al_map_rgb(0,0,0),570,100,ALLEGRO_ALIGN_RIGHT,"%d ml",actual_water);

        al_draw_text(font2,al_map_rgb(0,0,0),20,180,ALLEGRO_ALIGN_LEFT,"CONCENTRADO CONSUMIDO:");
        al_draw_textf(font2,al_map_rgb(0,0,0),500,180,ALLEGRO_ALIGN_LEFT,"%lu g",total_food_used);

        al_draw_text(font2,al_map_rgb(0,0,0),20,260,ALLEGRO_ALIGN_LEFT,"AGUA CONSUMIDA:");
        al_draw_textf(font2,al_map_rgb(0,0,0),500,260,ALLEGRO_ALIGN_LEFT,"%lu ml",total_water_used);

        al_draw_text(font2,al_map_rgb(0,0,0),20,340,ALLEGRO_ALIGN_LEFT,"HUEVOS RECOGIDOS:");
        al_draw_textf(font2,al_map_rgb(0,0,0),500,340,ALLEGRO_ALIGN_LEFT,"%d",total_eggs_picked);

        al_flip_display();
        al_clear_to_color(al_map_rgb(0, 0, 0));
    }
    return NULL;
}
