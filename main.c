#include <gb/gb.h>
#include <stdio.h>
#include "./sprites/SnakeHead/SnakeHead.c"
#include "./sprites/SnakeBody/SnakeBody.c"
#include "./backgrounds/Ground/background.c"
#include "./backgrounds/Ground/Ground.c"

INT8 player[2];
BYTE jumping;
UINT8 gravity = -2;
UINT8 speedY;
UINT8 floorYPosition = 113;

void performantdelay(UINT8 numloops){
    UINT8 i;
    for (i = 0; i < numloops; i++) {
        wait_vbl_done();
    }
}

INT8 wouldhitsurface(UINT8 projectedYPosition) {
    if (projectedYPosition >= floorYPosition) {
        return floorYPosition;
    }
    return -1;
}


void jump() {

    INT8 possibleYSurface;

    if (jumping == 0) {
        jumping = 1;
        speedY = 10;
    }

    speedY = speedY + gravity;

    player[1] = player[1] - speedY;
    possibleYSurface = wouldhitsurface(player[1]);

    if (possibleYSurface > -1) {
        jumping = 0;
        move_sprite(0, player[0], possibleYSurface);
    } else {
        move_sprite(0, player[0], player[1]);
    }
}


void main() {

    player[0] = 10;
    player[1] = floorYPosition;

    jumping = 0;

    set_bkg_data(0, 3, Ground);
    set_bkg_tiles(0, 0, 40, 18, background);

    set_sprite_data(0, 1, SnakeBody);   // first sprite, load 8 sprites, from SnakeHead sprite
    set_sprite_tile(0, 0);              // first sprite, first sprite memory bank
    move_sprite(0, player[0], player[1]);             // first sprite, 88 right, 78 down

    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;

    while(1) {


        if (joypad() & J_LEFT) {
            scroll_bkg(-8, 0);
        }
        if (joypad() & J_RIGHT) {
            scroll_bkg(8, 0);
        }
        if (joypad() & J_UP || jumping == 1) {
            jump();
        }
        
        performantdelay(5);
    }
}