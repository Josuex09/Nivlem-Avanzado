cmake_minimum_required(VERSION 3.2)
project(Bot)

set (CMAKE_C_FLAGS "-pthread")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
set(SOURCE_FILES main.c)

add_executable(Bot ${SOURCE_FILES} graphics/paint.h graphics/paint.c)

include_directories(/usr/include/allegro5)
target_link_libraries(Bot liballegro.so liballegro_primitives.so liballegro_ttf.so liballegro_font.so liballegro_image.so liballegro_dialog.so liballegro_audio.so liballegro_acodec.so)
