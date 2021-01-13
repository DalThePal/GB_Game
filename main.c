#include <gb/gb.h>
#include <stdio.h>
#include "./sprites/SnakeHead/SnakeHead.c"
#include "./sprites/SnakeBody/SnakeBody.c"
#include "./backgrounds/Ground/background.c"
#include "./backgrounds/Ground/backgroundTwo.c"
#include "./backgrounds/Ground/Ground.c"



// char = 8bits
// int = 16bits
// long = 32bits
// probably never use long
// use char for byte

unsigned char playerPos[2];
unsigned char playerTile[2];
char emptyBlock[1] = {0x00};
unsigned char jumping;
unsigned char falling;

signed char speedY;
signed char xOffset = 8;
signed char yOffset = 16;
unsigned int xScroll = 0;






void performantdelay(unsigned int numloops){
  unsigned int i;
  for (i = 0; i < numloops; i++) {
    wait_vbl_done();
  }
}









unsigned int findBGTile(unsigned int tileX, unsigned int tileY) {
  unsigned int bgLength = 32;
  unsigned int bgTile;

  bgTile = (tileY * 32) + tileX + ((xScroll) / 8);
  // printf("%u ", (unsigned int)(tileX));
  
  return bgTile;
}








unsigned char canLand(unsigned int bgTile) {
  unsigned char inBetween = xScroll % 8;

  if (inBetween) {
    if (background[bgTile] != 0x00 || background[bgTile + 1] != 0x00) {
      return 1;
    } else {
      return 0;
    }
  } else {
    if (background[bgTile] == 0x00) {
      return 0;
    } else {
      return 1;
    }
  }
}

unsigned char canFall(unsigned int bgTile) {
  unsigned char inBetween = xScroll % 8;

  if (inBetween) {
    if (background[bgTile] == 0x00 || background[bgTile + 1] == 0x00) {
      return 1;
    } else {
      return 0;
    }
  } else {
    if (background[bgTile] == 0x00) {
      return 1;
    } else {
      return 0;
    }
  }
}





void findCurrentTile() {
  unsigned char tileX = (playerPos[0] - xOffset) / 8;
  unsigned char tileY = (playerPos[1] - yOffset) / 8;
  

  playerTile[0] = tileX;
  playerTile[1] = tileY;

}

void jump() {
  if (!jumping && !falling) { 
    speedY = 10;
    jumping = 1;
  }

  gravity();
}


void fall() {
  if (!falling && !jumping) { 
    speedY = 2;
    falling = 1;
  }

  gravity();
}


void gravity() {
  unsigned char i;
  unsigned int nextTileY;
  unsigned int nextBGTile;
  int newPos = 0;
  signed char force = 2;

  // if (falling == 0) {    
  //   speedY = 2; // reset speed to 10
  //   falling = 1; // set jumping to true to continue loop
  // }

  speedY -= force;
  // printf("g:%d ", (INT16)(force));
  // printf("s:%d ", (INT16)(speedY));
  if (speedY <= 0) {
    jumping = 0;
    falling = 1;
  }

  for (i = 0; i <= (speedY / -8); i++) {
    // printf("i:%d", i);
    // printf("loop:%d", (speedY / -8));
    nextTileY = playerTile[1] + i + 1;
    nextBGTile = findBGTile(1, nextTileY);
    if (canLand(nextBGTile) && !jumping) {
      newPos = (nextTileY * 8) + 8;
      // printf("s:%d ", nextTileY);
      falling = 0;
      jumping = 0;
      // playerTile[1] = nextTileY - 1;
      // printf("pt:%u ", (INT16)(playerTile[1]));
      // printf("xs:%u ", (INT16)(xScroll));
      // printf("xs:%u ", (INT16)(nextBGTile));

      break;
    } 
  }
  if (newPos) {
    playerPos[1] = newPos;
  } else {
    playerPos[1] = playerPos[1] - speedY;
  }


  findCurrentTile();
  move_sprite(0, playerPos[0], playerPos[1]);

}




void scrollX(signed char value, char dir) {
  
  if (dir) {
    if ((xScroll + value) >= (8 * 32)) {
      xScroll = 0;
    } else {
      xScroll += value;
    }
  } else {
    if ((xScroll - value ) >= (8 * 32)) {
      xScroll = 8 * 32;
    } else {
      xScroll -= value;
    }
  }
   
}




void main() {
  playerPos[0] = 16;
  playerPos[1] = 144;
  findCurrentTile();

  jumping = 0;

  set_bkg_data(0, 3, Ground);
  set_bkg_tiles(0, 0, 32, 18, background);

  set_sprite_data(0, 1, SnakeBody);   // first sprite, load 8 sprites, from SnakeHead sprite
  set_sprite_tile(0, 0);              // first sprite, first sprite memory bank
  move_sprite(0, playerPos[0], playerPos[1]);             // first sprite, 88 right, 78 down

  SHOW_BKG;
  SHOW_SPRITES;
  DISPLAY_ON;

  while(1) {

    if (falling && !jumping) {
      fall();
    }
    
    if (joypad() & J_LEFT) {
      scroll_bkg(-4, 0);
      scrollX(4, 0);
      fall();
    }
    if (joypad() & J_RIGHT) {
      scroll_bkg(4, 0);
      scrollX(4, 1);
      fall();
    }
    if (joypad() & J_UP || jumping) {
      jump();
    }
    if (joypad() & J_DOWN) {
      printf("%u", (signed int)(xScroll));
    }
    
    performantdelay(5);
  }
}
