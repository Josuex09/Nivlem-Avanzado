//
// Created by josue on 11/19/15.
//

#ifndef BOT_PAINT_H
#define BOT_PAINT_H

#include "stdio.h"
#include "unistd.h"
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include <allegro5/allegro_acodec.h>
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 400
#define BACKGROUND_IMAGE_PATH "images/background.png"

void * init_graphics();
#endif //BOT_PAINT_H
