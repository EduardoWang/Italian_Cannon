#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>

#include "mode1.h"
#include "variables.h"

double degreeToRadians(int degree) {
  return degree*3.14159/180;
}

void redrawWind();

void redrawbullet(int curX, int curY, int nextX, int nextY, int rad) {
  tft.fillCircle(curX, curY, rad, ILI9341_BLACK);
  tft.fillCircle(nextX, nextY, rad, ILI9341_WHITE);
}

void drawMap(){
	tft.fillScreen(0);
	tft.fillRect(10, 15, 120, 10, ILI9341_RED); // HP left
	tft.fillRect(190, 15, 120, 10, ILI9341_RED); // HP right
	tft.fillRect(145, 210-HeightOfWall, 30, HeightOfWall, ILI9341_YELLOW); // Wall
  tft.fillRect(0, 210, 320, 30, 0x826735); // ground
	tft.setCursor(138, 15);
	tft.setTextColor(ILI9341_WHITE);
  redrawWind();
  if(digitalRead(serverPin)) {
    tft.setCursor(10, 30);
    tft.print(Mytank.HP);
    tft.setCursor(262, 30);
    tft.print(othertank.HP);
  }
  else {
    tft.setCursor(10, 30);
    tft.print(othertank.HP);
    tft.setCursor(262, 30);
    tft.print(Mytank.HP);
  }

}

void drawMapTank(boolean server) {
  if(server) {
    drawTank(tanks[selectedTank], 20, 210 - tanks[selectedTank].height/2, 1);
    drawTank(tanks[selectedTank3], 300, 210 - tanks[selectedTank3].height/2, 2);
  }
  else {
    drawTank(tanks[selectedTank3], 20, 210 - tanks[selectedTank3].height/2, 1);
    drawTank(tanks[selectedTank], 300, 210 - tanks[selectedTank].height/2, 22222);
  }
}

void redrawWall() {
  tft.fillRect(135, 60, 50, 150, ILI9341_BLACK);
  tft.fillRect(145, 210-HeightOfWall, 30, HeightOfWall, ILI9341_YELLOW);
}

void redrawWind() {
  tft.fillRect(120, 30, 75, 16, ILI9341_BLACK);
  tft.fillTriangle(185, 46, 185, 30, 195, 38, ILI9341_WHITE);
  tft.setCursor(138, 15);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print("WIND");
  tft.setCursor(120, 30);
  tft.print(Wind);
}

void redrawHP(tank t, int full_HP, int position) {
  double pHP = t.HP;
  double newfullHP = full_HP;
  double percentHP = pHP/newfullHP;
  int HP = percentHP * 120;
  if(position == 0) {
    tft.fillRect(10, 15, 120, 10, ILI9341_BLACK);
    tft.fillRect(10, 15, HP, 10, ILI9341_RED);
    tft.fillRect(10, 30, 50, 20, ILI9341_BLACK);
    tft.setCursor(10, 30);
    tft.print(t.HP);
  }
  else {
    tft.fillRect(190, 15, 120, 10, ILI9341_BLACK);
    tft.fillRect(310-HP, 15, HP, 10, ILI9341_RED);
    tft.fillRect(310-50, 30, 50, 20, ILI9341_BLACK);
    if(t.HP >= 1000) {
      tft.setCursor(310-4*12, 30);
    }
    else if(t.HP >= 100) {
      tft.setCursor(310-3*12, 30);
    }
    else if(t.HP >= 10) {
      tft.setCursor(310-2*12, 30);
    }
    else {
      tft.setCursor(310-12, 30);
    }
    tft.print(t.HP);
  }
}

boolean hitTank(tank t, int target, int x, int y, int bullet) {
  boolean hit = false;
  if(target == 0) {
    // upper rectangle
    if(abs(x-20) < t.length/3+bullet && y > 210-t.height) {
      hit = true;
    }
    // lower rectangle
    else if(abs(x-20) < t.length/2+bullet && y > 210-t.height/2) {
      hit = true;
    }
  }
  else if(target == 1) {
    if(abs(x-300) < t.length/3+bullet && y > 210-t.height) {
      hit = true;
    }
    else if(abs(x-300) < t.length/2+bullet && y > 210-t.length/2) {
      hit = true;
    }
  }
  return hit;
}

boolean hitEdgeandGround(int x, int y, int bullet) {
  if(x >= 320 || x <= 0 || y >= 210-bullet) {
    return true;
  }
  else {
    return false;
  }
}

boolean hitWall(int x, int y, int bullet) {
  if((abs(x-160) <= bullet+15) && abs(210-y) < bullet+HeightOfWall) {
    return true;
  }
  else {
    return false;
  }
}

void fire(int select, int initial, int angle, int selectBullet, int server) {
  double newAngle = degreeToRadians(angle);
  double newInitial = initial;
  if(select == 0) {
    int pX = 40;
    int pY = 205;
    int nX = pX;
    int nY = pY;
    while(true) {
      redrawbullet(pX, pY, nX, nY, selectBullet);
      pX = nX;
      pY = nY;
      nX += initial*cos(newAngle)+Wind;
      nY -= newInitial*sin(newAngle);
      newInitial -= 1;
      redrawWind();
      if(server == 0) {
        redrawHP(othertank, tanks[selectedTank3].HP, 1);
      }
      else {
        redrawHP(Mytank, tanks[selectedTank].HP, 1);
      }
      if(server == 0) {
        if(hitTank(othertank, 1, pX, pY, selectBullet)) {
          if(selectBullet == 2) {
            othertank.HP -= Mytank.power_small;
          }
          else {
            othertank.HP -= Mytank.power_big;
          }
          redrawHP(othertank, tanks[selectedTank3].HP, 1);
          tft.fillRect(0, 0, 10, 240, ILI9341_BLACK);
          tft.fillRect(310, 0, 10, 240, ILI9341_BLACK);
          tft.fillRect(0, 180, 320, 60, ILI9341_BLACK);
          tft.fillRect(0, 210, 320, 30, 0x826735);
          redrawWall();
          drawMapTank(true);
          break;
        }
      }
      else {
        if(hitTank(Mytank, 1, pX, pY, selectBullet)) {
          if(selectBullet == 2) {
            Mytank.HP -= othertank.power_small;
          }
          else {
            Mytank.HP -= othertank.power_big;
          }
          redrawHP(Mytank, tanks[selectedTank].HP, 1);
          tft.fillRect(0, 0, 10, 240, ILI9341_BLACK);
          tft.fillRect(310, 0, 10, 240, ILI9341_BLACK);
          tft.fillRect(0, 180, 320, 60, ILI9341_BLACK);
          tft.fillRect(0, 210, 320, 30, 0x826735);
          redrawWall();
          drawMapTank(false);
          break;
        }
      }

      if(hitWall(pX, pY, selectBullet)) {
        redrawWall();
        break;
      }
      else if(hitEdgeandGround(pX, pY, selectBullet)) {
        tft.fillRect(0, 0, 10, 240, ILI9341_BLACK);
        tft.fillRect(310, 0, 10, 240, ILI9341_BLACK);
        tft.fillRect(0, 180, 320, 60, ILI9341_BLACK);
        tft.fillRect(0, 210, 320, 30, 0x826735);
        redrawWall();
        if(server == 0) {
          drawMapTank(true);
        }
        else {
          drawMapTank(false);
        }
        break;
      }
      delay(50);
    }
  }
  else if(select == 1) {
    int pX = 280;
    int pY = 205;
    int nX = pX;
    int nY = pY;
    while(true) {
      redrawbullet(pX, pY, nX, nY, selectBullet);
      pX = nX;
      pY = nY;
      nX -= initial*cos(newAngle)-Wind;
      nY -= newInitial*sin(newAngle);
      newInitial -= 1;
      redrawWind();
      if(server == 0) {
        redrawHP(Mytank, tanks[selectedTank].HP, 0);
      }
      else {
        redrawHP(othertank, tanks[selectedTank3].HP, 0);
      }
      if(server == 0) {
        if(hitTank(Mytank, 0, pX, pY, selectBullet)) {
          if(selectBullet == 2) {
            Mytank.HP -= othertank.power_small;
          }
          else {
            Mytank.HP -= othertank.power_big;
          }
          redrawHP(Mytank, tanks[selectedTank].HP, 0);
          tft.fillRect(0, 0, 10, 240, ILI9341_BLACK);
          tft.fillRect(310, 0, 10, 240, ILI9341_BLACK);
          tft.fillRect(0, 180, 320, 60, ILI9341_BLACK);
          tft.fillRect(0, 210, 320, 30, 0x826735);
          redrawWall();
          drawMapTank(true);
          break;
        }
      }
      else {
        if(hitTank(othertank, 0, pX, pY, selectBullet)) {
          if(selectBullet == 2) {
            othertank.HP -= Mytank.power_small;
          }
          else {
            othertank.HP -= Mytank.power_big;
          }
          redrawHP(othertank, tanks[selectedTank3].HP, 0);
          tft.fillRect(0, 0, 10, 240, ILI9341_BLACK);
          tft.fillRect(310, 0, 10, 240, ILI9341_BLACK);
          tft.fillRect(0, 180, 320, 60, ILI9341_BLACK);
          tft.fillRect(0, 210, 320, 30, 0x826735);
          redrawWall();
          drawMapTank(false);
          break;
        }
      }

      if(hitWall(pX, pY, selectBullet)) {
        redrawWall();
        break;
      }
      else if(hitEdgeandGround(pX, pY, selectBullet)) {
        tft.fillRect(0, 0, 10, 240, ILI9341_BLACK);
        tft.fillRect(310, 0, 10, 240, ILI9341_BLACK);
        tft.fillRect(0, 180, 320, 60, ILI9341_BLACK);
        tft.fillRect(0, 210, 320, 30, 0x826735);
        redrawWall();
        if(server == 0) {
          drawMapTank(true);
        }
        else {
          drawMapTank(false);
        }
        break;
      }
      delay(50);
    }
  }
}


void attack(int select, int server) {
  uint8_t initial = 0;
  uint8_t angle = 15;
  uint8_t selectBullet = 2;

  if(select == 0) {
    tft.fillCircle(40, 205, 4, ILI9341_BLACK);
    tft.fillCircle(40, 205, selectBullet, ILI9341_WHITE);
  }
  else {
    tft.fillCircle(280, 205, 4, ILI9341_BLACK);
    tft.fillCircle(280, 205, selectBullet, ILI9341_WHITE);
  }


  while(true) {
    int yval = analogRead(JOY_VERT);
    int oldBullet = selectBullet;


    if((yval - JOY_CENTER) > JOY_SPEED && selectBullet > 2) {
      selectBullet -= 2;
    }
    else if((JOY_CENTER - yval) > JOY_SPEED && selectBullet < 4) {
      selectBullet += 2;
    }

    if(oldBullet != selectBullet) {
      if(select == 0) {
        tft.fillCircle(40, 205, 4, ILI9341_BLACK);
        tft.fillCircle(40, 205, selectBullet, ILI9341_WHITE);
      }
      else {
        tft.fillCircle(280, 205, 4, ILI9341_BLACK);
        tft.fillCircle(280, 205, selectBullet, ILI9341_WHITE);
      }
    }
    if(!digitalRead(JOY_SEL)) {
      while(!digitalRead(JOY_SEL)) { delay(10); }
      break;
    }
    delay(100);
  }

  if(select == 0) {
    tft.fillRect(5, 160, 70, 20, ILI9341_BLACK);
    tft.setCursor(5, 160);
    tft.setTextSize(1);
    tft.print("speed: ");
    tft.print(initial);
    tft.print("00");
    tft.setCursor(5, 170);
    tft.print("angle: ");
    tft.print(angle);
  }
  else {
    tft.fillRect(245, 160, 70, 20, ILI9341_BLACK);
    tft.setCursor(245, 160);
    tft.setTextSize(1);
    tft.print("speed: ");
    tft.print(initial);
    tft.print("00");
    tft.setCursor(245, 170);
    tft.print("angle: ");
    tft.print(angle);
  }

  while(true) {
    int yVal = analogRead(JOY_VERT);
    int xVal = analogRead(JOY_HORIZ);

    uint8_t oldi = initial;
    uint8_t olda = angle;
    if((yVal - JOY_CENTER) > JOY_SPEED && angle > 0) {
      angle -= 1;
    }
    else if((JOY_CENTER - yVal) > JOY_SPEED) {
      angle += 1;
    }

    if(select == 0) {
      if((xVal - JOY_CENTER) > JOY_SPEED && initial > 0) {
        initial -= 1;
      }
      else if((JOY_CENTER - xVal) > JOY_SPEED) {
        initial += 1;
      }
    }
    else if(select == 1) {
      if((xVal - JOY_CENTER) > JOY_SPEED) {
        initial += 1;
      }
      else if((JOY_CENTER - xVal) > JOY_SPEED && initial > 0) {
        initial -= 1;
      }
    }


    initial = constrain(initial, 0, 20);
    angle = constrain(angle, 15, 60);

    if(oldi != initial || olda != angle) {
      if(select == 0) {
        tft.fillRect(5, 160, 70, 20, ILI9341_BLACK);
        tft.setCursor(5, 160);
        tft.setTextSize(1);
        tft.print("speed: ");
        tft.print(initial);
        tft.print("00");
        tft.setCursor(5, 170);
        tft.print("angle: ");
        tft.print(angle);
      }
      else {
        tft.fillRect(245, 160, 70, 20, ILI9341_BLACK);
        tft.setCursor(245, 160);
        tft.setTextSize(1);
        tft.print("speed: ");
        tft.print(initial);
        tft.print("00");
        tft.setCursor(245, 170);
        tft.print("angle: ");
        tft.print(angle);
      }
    }

    if(!digitalRead(JOY_SEL)) {
      tft.fillRect(5, 160, 70, 20, ILI9341_BLACK);
      tft.fillRect(245, 160, 70, 20, ILI9341_BLACK);
      break;
    }

    delay(100);
  }

  Serial3.write(selectBullet);
  Serial3.write(initial);
  Serial3.write(angle);
  fire(select, initial, angle, selectBullet, server);
}

void response(int select, int server) {
  while(Serial3.available() > 0) {
    Serial3.read();
  }
  while(Serial3.available() == 0) { delay(10); }
  uint8_t selectBullet = Serial3.read();
  uint8_t initial = Serial3.read();
  uint8_t angle = Serial3.read();
  fire(select, initial, angle, selectBullet, server);
}
