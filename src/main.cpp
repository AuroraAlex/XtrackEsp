#include <Arduino.h>
#include <TFT_eSPI.h>   
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI(); 

void setup() {
  // put your setup code here, to run once:
  tft.begin();
  tft.setRotation(1); // landscape

  tft.fillScreen(TFT_BLACK);
}

void loop() {
  // put your main code here, to run repeatedly:
}

