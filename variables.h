#ifndef _VARIABLES_H_
#define _VARIABLES_H_

#define TFT_DC 9
#define TFT_CS 10

#define serverPin 13

#include "defTank.h"

extern Adafruit_ILI9341 tft;

#define JOY_VERT  A1 // should connect A1 to pin VRx
#define JOY_HORIZ A0 // should connect A0 to pin VRy
#define JOY_SEL   2

#define JOY_CENTER   512
#define JOY_SPEED 64

extern uint8_t selectedTank;
extern uint8_t selectedStart;
extern uint8_t selectedTank3;

extern tank tanks[3];
extern tank Mytank;
extern tank othertank;

extern double Wind;
extern int HeightOfWall;

#endif
