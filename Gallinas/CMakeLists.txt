cmake_minimum_required(VERSION 3.2)
project(Gallinas)

set (CMAKE_C_FLAGS "-pthread -lm")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

set(SOURCE_FILES main.c)
add_executable(Gallinas ${SOURCE_FILES} aux/list.h aux/list.c aux/tcp_connection.h aux/tcp_connection.c aux/calc.h aux/calc.c graphics/paint.h graphics/paint.c)

include_directories(/usr/include/allegro5)
target_link_libraries(Gallinas liballegro.so liballegro_primitives.so liballegro_ttf.so liballegro_font.so liballegro_image.so liballegro_dialog.so liballegro_audio.so liballegro_acodec.so)