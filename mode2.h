#ifndef _MODE2_H_
#define _MODE2_H_

#include "defTank.h"

double degreeToRadians(int degree);
void redrawWind();
void redrawbullet();
void drawMap();
void drawMapTank(boolean server);
void redrawWall();
void redrawHP(tank t, int full_HP, int position);
boolean hitTank(tank t, int target, int x, int y, int bullet);
boolean hitEdgeandGround(int x, int y, int bullet);
boolean hitWall(int x, int y, int bullet);
void fire(int select, int initial, int angle, int selectBullet, int server);
void attack(int select, int server);
void response(int select, int server);

#endif
