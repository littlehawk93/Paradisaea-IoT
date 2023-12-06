
void startTFT() {
  digitalWrite(TFT_CS, LOW);
  delay(10);

  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(rgb4(0x0F, 0x0F, 0x0F));
}

void clearScreen() {
  tft.fillScreen(ST77XX_WHITE);
}

void drawNoImages() {
  digitalWrite(TFT_CS, LOW);
  delay(10);

  tft.fillScreen(ST77XX_RED);
  tft.fillCircle(64,80,30,ST77XX_WHITE);

  tft.drawLine(46,62,82,98,ST77XX_RED);
  tft.drawLine(46,98,82,62,ST77XX_RED);

  tft.drawLine(46,61,82,97,ST77XX_RED);
  tft.drawLine(46,97,82,61,ST77XX_RED);

  tft.drawLine(46,63,82,99,ST77XX_RED);
  tft.drawLine(46,99,82,63,ST77XX_RED);
}

void drawSuccess() {
  digitalWrite(TFT_CS, LOW);
  delay(10);

  tft.fillScreen(ST77XX_GREEN);
  tft.fillCircle(64,80,30,ST77XX_WHITE);

  tft.fillTriangle(64, 95, 76, 80, 70, 80, ST77XX_GREEN);
  tft.fillTriangle(76, 80, 70, 80, 54, 65, ST77XX_GREEN);
}

void drawError(uint8_t errorCode) {
  digitalWrite(TFT_CS, LOW);
  delay(10);

  tft.fillScreen(ST77XX_RED);

  uint8_t mask = 1;

  for(int i=0;i<4;i++) {
    if ((mask & errorCode) != 0) {
      tft.fillCircle(64, ((i % 4)+1) * 32, 8, ST77XX_WHITE);
    } else {
      tft.drawCircle(64, ((i % 4)+1) * 32, 8, ST77XX_WHITE);
    }
    mask = mask << 1;
  }
}

void drawWifiConnect(int count) {
  
  tft.fillScreen(ST77XX_WHITE);

  count = count % 4;

  if (count != 3) {
    tft.fillCircle(64, (count+1) * 40, 15, ST77XX_BLUE);
  }
}

void drawApiPixel(int x, int y, char red, char green, char blue) {
  uint8_t r = parseHex(red);
  uint8_t g = parseHex(green);
  uint8_t b = parseHex(blue);
  tft.drawPixel(y, 159 - x, rgb4(r, g, b));
}

uint8_t parseHex(char c) {
  uint8_t val = ((c < 58) ? (uint8_t)(c - 48) : (uint8_t)(c - 55)) & 0x0F;
  return val;
}

uint16_t rgb4(uint8_t r, uint8_t g, uint8_t b) {

  r = map(r, 0, 15, 0, 31);
  g = map(g, 0, 15, 0, 63);
  b = map(b, 0, 15, 0, 31);

  uint16_t col = 0;

  col |= ((r & 0x1F) << 11) | ((g & 0x3F) << 5) | (b & 0x1F);
  return col;
}