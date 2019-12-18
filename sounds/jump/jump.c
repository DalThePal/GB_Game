#include "./jump.h"
#include <gb/gb.h>


void jumpSound() {
    
    /*
    channel 1 register 0, frequency sweep settings
    7   unused 
    6-4 sweep time (update rate) (if 0, sweeping is off)
    3   sweep direction (1: decrease, 0: increase)
    2-0 sweep RtShift amount (if 0, sweeping is off)
    0001 0110 is 0x16, sweep time 1, sweep direction increase, shift amout per step 110 (6 decimal)
    */
    NR10_REG = 0x16;

    /*
    channel 1 register 1, wave pattern duty and sound length
    channesls 1 2 and 4
    7-6 wave pattern duty cycle 0-3 (12.5%, 25%, 50%, 75%)
        duty cycle is how long a quadrangular wave is "on" vs "of"
        so 50% (2) is both equal.
    5-0 sound length (higher the number the shorter the sound)
    01000000 is 0x40, duty cycle 1 (25%), wave length 0 (long)
    */
   NR11_REG = 0x40;

    /*
    channel 1 register 2: volume envelope (makes the volume get louder or quiter each 'tick')
    channels 1 2 and 4
    7-4 (initial) channel volume
    3   volume sweep direction (0: down, 1: up)
    2-0 length of each steap in sweep (if 0, sweeping is off)
    note: each step is n/64 seconds long, where n is 1-7
    0111 0011 is 0x73, volume 7, sweep down, step length 3
    */
   NR12_REG = 0x73;

    /*
    chanel 1 register 3: frequency LSbs (Least Significant bits) and noise options
    channels 1 2 and 3
    7-0 8 Least Significant bits of frequency (3 Most Significant Bits are set in register 4)
    */
   NR13_REG = 0x00;

   /*
    chanel 1 register 4, Playback and frequency MSbs
    channels 1 2 3 and 4
    7   Initialize (trigger channel start, AKA channel INIT) (Write only)
    6   Consecutive select/length counter enable (Read/Write) when "0", regardless of length
        of data on the NR11, sound can be produced
    5-3 unused
    2-0 3 Most Significant bits of frequency
    1100 0011 is 0xC3, initialize, no consecutive, frequency = MSB + LSB = 011 0000 0000 = ox300
   */
  NR14_REG = 0xC3;
}