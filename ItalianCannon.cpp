#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>

#include "mode0.h"
#include "mode1.h"
#include "mode2.h"
#include "variables.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

uint8_t selectedTank = 0;
uint8_t selectedStart = 0;
uint8_t selectedTank3 = 0;

tank tanks[3];
tank Mytank;
tank othertank;

double Wind = 0;
int HeightOfWall = 80;



void setup() {
  init();
  Serial.begin(9600);
  Serial3.begin(9600);
  tft.begin();

  tft.setRotation(3);

  tft.fillScreen(ILI9341_BLACK);

  pinMode(JOY_SEL, INPUT_PULLUP);
  pinMode(serverPin, INPUT);

  tanks[0] = {24, 10, 100, 400, 3000, ILI9341_RED};
  tanks[1] = {18, 8, 150, 450, 2000, ILI9341_YELLOW};
  tanks[2] = {12, 6, 250, 550, 1000, ILI9341_BLUE};
}

void newWind() {
  int windf =  23 * sin((Wind+11)*29 + 1.875) + cos((Wind-11)*17);
  Wind = (double)windf / 10;
}

void newWall() {
  double Wallfactor = 30 * sin(HeightOfWall * 3) + 50;
  HeightOfWall = Wallfactor;
}





void serverMode2(int att) {
  if(att == 0) {
    attack(0,0);
  }
  else if(att == 1) {
    response(1,0);
  }
}

void clientMode2(int att) {
  if(att == 0) {
    response(0,1);
  }
  else if(att == 1) {
    attack(1,1);
  }
}

void server() {
  startMenu();
  drawpanel();
  Mytank = tanks[selectedTank];
  othertank = tanks[selectedTank3];
  drawMap();
  drawMapTank(true);
}

void client() {
  tft.fillScreen(0);
  tft.setCursor(55,113);
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_WHITE,ILI9341_BLACK);
  tft.print("WAIT FOR BEGINNING");
  while(Serial3.available() == 0) { delay(10); }
  selectedStart = Serial3.read();
  drawpanel();
  Mytank = tanks[selectedTank];
  othertank = tanks[selectedTank3];
  drawMap();
  drawMapTank(false);
}






int main() {
  setup();

  boolean SOrC = digitalRead(serverPin);
  if(SOrC) {
    server();
  }
  else {
    client();
  }


  if(selectedStart == 0) {
    int select = 0;

    while(true) {
      if(SOrC) {
        serverMode2(select);
      }
      else {
        clientMode2(select);
      }
      if(select == 0) {
        select = 1;
      }
      else{
        select = 0;
      }
      newWind();
      newWall();
      redrawWind();
      redrawWall();
      if(Mytank.HP <= 0) {
        tft.fillScreen(0);
        tft.setCursor(40, 100);
        tft.setTextSize(5);
        tft.print("YOU LOSE");
        break;
      }
      else if(othertank.HP <= 0) {
        tft.fillScreen(0);
        tft.setCursor(55, 100);
        tft.setTextSize(5);
        tft.print("YOU WIN");
        break;
      }
    }
  }
  else {
    int select = 1;

    while(true) {
      if(SOrC) {
        serverMode2(select);
      }
      else {
        clientMode2(select);
      }
      if(select == 0) {
        select = 1;
      }
      else{
        select = 0;
      }
      newWind();
      newWall();
      redrawWind();
      redrawWall();
      if(Mytank.HP <= 0) {
        tft.fillScreen(0);
        tft.setCursor(40, 100);
        tft.setTextSize(5);
        tft.print("YOU LOSE");
        break;
      }
      else if(othertank.HP <= 0) {
        tft.fillScreen(0);
        tft.setCursor(55, 100);
        tft.setTextSize(5);
        tft.print("YOU WIN");
        break;
      }
    }
  }
  Serial.flush();
  Serial3.flush();

  return 0;
}
