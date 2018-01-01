#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include "variables.h"

void selectStartMode() {
  String startMenu[3] = {"Server Start", "Client Start", "Random Start"};
  tft.setTextSize(2);
  tft.fillRect(0, 100, 320, 140,ILI9341_BLACK);

  do {
    int yVal = analogRead(JOY_VERT);
    int oldStart = selectedStart;
    if((yVal - JOY_CENTER) > JOY_SPEED) {
      selectedStart++;
    }
    else if((JOY_CENTER - yVal) > JOY_SPEED && selectedStart != 0) {
      selectedStart--;
    }

    selectedStart = constrain(selectedStart,0,2);

    for(int i = 0; i < 3; i++) {
      tft.setCursor(100,110+i*40);
      if(i != selectedStart) {
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      }
      else {
        tft.setTextColor(ILI9341_BLACK,ILI9341_WHITE);
      }
      tft.print(startMenu[i]);
      tft.print("\n");
    }
    tft.print("\n");

    if(!digitalRead(JOY_SEL)) {
      if(selectedStart == 2) {
        selectedStart = random(2);
      }
      Serial3.write(selectedStart);
      break;
    }

    delay(100);
  } while(true);
}

void startMenu(){
	tft.fillScreen(0);
  tft.setCursor(30, 50);
  tft.setTextColor(ILI9341_WHITE,ILI9341_BLACK);
  tft.setTextSize(3);
  tft.println("ITALIAN CANNON");

  tft.setTextSize(3);
  tft.setCursor(120, 150);
  tft.println("Start");

  tft.fillTriangle(90, 174, 90, 150, 108, 162, ILI9341_WHITE);
  while(true) {
    if(!digitalRead(JOY_SEL)) {
      selectStartMode();
      break;
    }
  }
}
