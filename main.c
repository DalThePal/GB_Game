#include <gb/gb.h>
#include <stdio.h>
#include "./sprites/SnakeHead/SnakeHead.c"
#include "./sprites/SnakeBody/SnakeBody.c"


int determineDirection() {

    switch(joypad()) {
        case J_LEFT:
            return 0;
            break;
        case J_UP:
            return 1;
            break;
        case J_RIGHT:
            return 2;
            break;
        case J_DOWN:
            return 4;
            break;
    }
}



void main() {
    UINT8 currentSpriteIndex = 0;

    int direction = 0;
    int xPos = 80;
    int yPos = 78;

    set_sprite_data(0, 8, SnakeHead);   // first sprite, load 8 sprites, from SnakeHead sprite
    set_sprite_tile(0, 0);              // first sprite, first sprite memory bank
    move_sprite(0, xPos, yPos);             // first sprite, 88 right, 78 down

    set_sprite_data(8, 1, SnakeBody);
    set_sprite_tile(1, 8);
    move_sprite(1, 88, 78);


    SHOW_SPRITES;

    while(1) {
        if (joypad()) {
            direction = determineDirection();
        }

        switch(direction) {
            case 0:
                set_sprite_tile(0, 0);
                xPos -= 8;
                break;
            case 1:
                set_sprite_tile(0, 2);
                yPos -= 8;
                break;
            case 2:
                set_sprite_tile(0, 4);
                xPos += 8;
                break;
            case 4:
                set_sprite_tile(0, 6);
                yPos += 8;
                break;
        }

        move_sprite(0, xPos, yPos);
        delay(200);
    }
}