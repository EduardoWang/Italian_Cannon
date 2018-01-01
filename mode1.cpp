#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>

#include "variables.h"

void drawTank(tank t, int x, int y, int LorR) {
  tft.fillRect(x-t.length/3, y-t.height/2, t.length*2/3, t.height/2, t.color);
  tft.fillRect(x-t.length/2, y, t.length, t.height/2, t.color);
  if(LorR == 1) {
    tft.fillRect(x,y-t.height/2+1, 16, 2, t.color);
  }
  else {
    tft.fillRect(x-16,y-t.height/2+1, 16, 2, t.color);
  }
  for(int i = -1; i < 2; i++) {
    tft.fillCircle(x + i*t.length/3, y+t.height/2+2, t.length/6, t.color);
  }
}

void selectTank() {
  for(int i = 0; i < 3; i++) {
    int Vcentre = 50+i*100+(i+1)*5;
    if(i == selectedTank) {
      tft.fillRect(Vcentre-50, 10, 100, 206,ILI9341_WHITE);
      tft.fillRect(Vcentre-30, 30, 60, 60, ILI9341_BLACK);
      tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
      tft.fillCircle(Vcentre-15, 110, 2, ILI9341_BLACK);
      tft.fillCircle(Vcentre+15, 110, 4, ILI9341_BLACK);
    }
    else {
      tft.drawRect(Vcentre-50, 10, 100, 206,ILI9341_WHITE);
      tft.drawRect(Vcentre-30, 30, 60, 60, ILI9341_WHITE);
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.fillCircle(Vcentre-15, 110, 2, ILI9341_WHITE);
      tft.fillCircle(Vcentre+15, 110, 4, ILI9341_WHITE);
    }
    tft.setCursor(Vcentre-40, 130);
    tft.setTextSize(2);
    tft.print(tanks[i].power_small);
    tft.setCursor(Vcentre+5, 130);
    tft.print(tanks[i].power_big);
    tft.setCursor(Vcentre-30, 160);
    tft.setTextSize(2);
    tft.print("HP:");
    tft.setCursor(Vcentre-30, 180);
    tft.print(tanks[i].HP);

    drawTank(tanks[i], Vcentre, 60, 1);
  }
}

void drawpanel() {
  while(Serial3.available() > 0) {
    Serial3.read();
  }
  tft.fillScreen(0);
  selectTank();
  while(true) {
    int xVal = analogRead(JOY_HORIZ);
    int oldtank = selectedTank;
    if((xVal - JOY_CENTER) > JOY_SPEED && selectedTank > 0) {
      selectedTank--;
    }
    else if((JOY_CENTER - xVal) > JOY_SPEED) {
      selectedTank++;
    }

    selectedTank = constrain(selectedTank, 0, 2);

    if(oldtank != selectedTank) {
      tft.fillRect(oldtank*100+5*(oldtank+1), 10, 100, 206, ILI9341_BLACK);
      selectTank();
    }


    if(!digitalRead(JOY_SEL)) {
      Serial3.write(selectedTank);
      while(Serial3.available() == 0) { delay(10); }
      selectedTank3 = Serial3.read();
      break;
    }
    delay(100);
  }
}
