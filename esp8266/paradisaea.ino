#include <SPI.h>
#include <SD.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiClient.h>
#include <Adafruit_GFX.h> 
#include <Adafruit_ST7735.h>

#define TFT_CS 15 // D8
#define TFT_RST 16 // D0
#define TFT_DC 2 // D4
#define SD_CS 4 // D2
#define NEXT_BUTTON 5 // D1
#define DELETE_BUTTON 0 // D3

WiFiClientSecure client;
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

char apiDomain[200];
char apiDeviceID[40];
char apiImages[20][13];

char wifiSSID[35];
char wifiPassword[200];

bool nextPress = true;
bool deletePress = true;

int currentImageIndex = 0;

void setup() {
  startTFT();
  startApiClient();

  pinMode(NEXT_BUTTON, INPUT_PULLUP);
  pinMode(DELETE_BUTTON, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(NEXT_BUTTON), onNextPress, FALLING);
  attachInterrupt(digitalPinToInterrupt(DELETE_BUTTON), onDeletePress, FALLING);

  digitalWrite(TFT_CS, HIGH);
  delay(10);

  if(!startSDCard()) {
    onError(1);
  }

  if(!readConfig("SSID", wifiSSID, 35)) {
    onError(2);
  }

  if(!readConfig("PASSWORD", wifiPassword, 200)) {
    onError(3);
  }

  if(!readConfig("DOMAIN", apiDomain, 200)) {
    onError(4);
  }

  if(!readConfig("DEVICEID", apiDeviceID, 40)) {
    onError(5);
  }

  digitalWrite(TFT_CS, LOW);
  delay(10);

  if(!connectWifi(wifiSSID, wifiPassword, 20000, drawWifiConnect)) {
    onError(6);
  }

  drawSuccess();
  delay(1000);

  refreshImageList();
  nextPress = false;
  deletePress = false;
}

void loop() {
  
  if(nextPress) {
    nextImage();
    nextPress = false;
  }

  if (deletePress) {
    deleteImage();
    deletePress = false;
  }

  delay(16);
}

ICACHE_RAM_ATTR void onDeletePress() {
  if(!nextPress && !deletePress) {
    deletePress = true;
  }
}

ICACHE_RAM_ATTR void onNextPress() {
  if(!nextPress && !deletePress) {
    nextPress = true;
  }
}

void refreshImageList() {
  if(!apiGetImageList(apiDomain, apiDeviceID, apiImages, 20)) {
    onError(7);
  }

  currentImageIndex = 0;

  if(strlen(apiImages[currentImageIndex]) == 0) {
    drawNoImages();
    return;
  }

  if(!apiGetImage(apiDomain, apiDeviceID, apiImages[currentImageIndex])) {
    onError(8);
  }
}

void nextImage() {
  clearScreen();
  currentImageIndex++;

  if(currentImageIndex >= 20 || strlen(apiImages[currentImageIndex]) == 0) {
    refreshImageList();
  }

  if(!apiGetImage(apiDomain, apiDeviceID, apiImages[currentImageIndex])) {
    onError(9);
  }
}

void deleteImage() {

  if(strlen(apiImages[currentImageIndex]) == 0) {
    return;
  }

  clearScreen();

  if(!apiDeleteImage(apiDomain, apiDeviceID, apiImages[currentImageIndex])) {
    onError(10);
  }

  refreshImageList();
}

void onError(uint8_t errorCode) {
  drawError(errorCode);
  while(true) {
    delay(10000);
  }
}
