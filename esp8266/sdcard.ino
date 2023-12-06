
bool startSDCard() {
  return SD.begin(SD_CS);
}

bool readConfig(char *name, char *value, int length) {
  digitalWrite(SD_CS, LOW);
  delay(10);

  File configFile = SD.open("CONFIG.INI", FILE_READ);

  if (!configFile) {
    digitalWrite(SD_CS, HIGH);
    delay(10);
    return false;
  }

  char configName[200];
  int bufferIndex = 0;

  bool inConfigName = false;
  bool inConfigValue = false;
  bool inComment = false;

  while(configFile.available()) {

    char c = configFile.read();

    if (inConfigName) {
      if (c == '=') {
        inConfigName = false;
        inConfigValue = true;
        configName[bufferIndex] = '\0';
        bufferIndex = 0;
      } else {
        configName[bufferIndex] = c;
        bufferIndex++;
      }
    } else if (inConfigValue) {
      if (c == '\n' || c == '\r') {
        if (strcmp(configName, name) == 0) {
          value[bufferIndex % length] = '\0';
          break;
        }
        inConfigValue = false;
        bufferIndex = 0;
      } else if (strcmp(configName, name) == 0) {
        value[bufferIndex % (length - 1)] = c;
        bufferIndex++;
      }
    } else if (inComment) {
      if (c == '\n' || c == '\r') {
        inComment = false;
      }
    } else if (c == '#') {
      inComment = true;
    } else if (!ignoreChar(c)) {
      inConfigName = true;
      configName[bufferIndex] = c;
      bufferIndex++;
    }
  }

  configFile.close();
  digitalWrite(SD_CS, HIGH);
  delay(10);

  return true;
}

bool ignoreChar(char c) {
  return c <= 32 || c >= 127;
}
