
void startApiClient() {
  client.setInsecure();
}

bool apiGetImageList(char *host, char *deviceId, char ids[][13], int max) {

  char path[70];
  sprintf(path, "/api/device/%s/image?idsOnly", deviceId);

  if(!client.connect(host, 443)) {
    client.stop();
    return false;
  }

  writeRequestHeaders(path, host, "GET", "application/json");
  readUntilResponseBody();

  bool inArray = false;
  int idIndex = 0;
  int idArrIndex = 0;

  while(client.available()) {
    char c = client.read();

    if (!inArray) {
      if ( c == '[') {
        inArray = true;
      }
      continue;
    }

    if (c == ']') {
      break;
    }

    if (c == ',') {
      ids[idArrIndex][idIndex] = '\0';
      idArrIndex++;
      idIndex = 0;

      if(idArrIndex >= max) {
        break;
      }
    }

    if (c >= 48 && c <= 57) {
      ids[idArrIndex][idIndex] = c;
      idIndex++;
    }
  }

  for(int i = idArrIndex;i<max;i++) {
    ids[i][idIndex] = '\0';
    idIndex = 0;
  }

  return true;
}

bool apiDeleteImage(char *host, char *deviceId, char *imageId) {

  char path[70];
  sprintf(path, "/api/device/%s/image/%s", deviceId, imageId);

  if(!client.connect(host, 443)) {
    client.stop();
    return false;
  }

  writeRequestHeaders(path, host, "DELETE", "*/*");
  readUntilResponseBody();

  return true;
}

bool apiGetImage(char *host, char *deviceId, char *imageId) {

  char path[70];
  sprintf(path, "/api/device/%s/image/%s/3", deviceId, imageId);

  if(!client.connect(host, 443)) {
    client.stop();
    return false;
  }

  writeRequestHeaders(path, host, "GET", "*/*");
  readUntilResponseBody();

  int x = 0;
  int y = 0;

  while(client.available()) {
    String line = client.readStringUntil('\n');
    
    if (line.length() > 3) { 
      x = 0;
      for(int i=0;i<line.length();i+=3) {
        drawApiPixel(x, y, line.charAt(i), line.charAt(i+1), line.charAt(i+2));
        x++;
      }
      y++;
    }
  }
  return true;
}

void readUntilResponseBody() {
  while(client.available() == 0);

  char buffer[4];

  while(client.available()) {
    char c = client.read();
    for (int i=0;i<3;i++) {
      buffer[i] = buffer[i+1];
    }
    buffer[3] = c;

    if (buffer[0] == '\r' && buffer[1] == '\n' && buffer[2] == '\r' && buffer[3] == '\n') {
      return;
    } else if (buffer[0] == '\n' && buffer[1] == '\r' && buffer[2] == '\n' && buffer[3] == '\r') {
      return;
    } else if (buffer[2] == '\n' && buffer[3] == '\n') {
      return;
    }
  }
}

void writeRequestHeaders(char *path, char *host, char *method, char *contentType) {
  client.print(method);
  client.print(" ");
  client.print(path);
  client.print(" HTTP/1.0\r\n");
  client.print("Host: ");
  client.print(host);
  client.print("\r\n");
  client.print("Connection: close\r\n");
  client.print("User-Agent: Arduino/1.0 ESP8266 (Paradisaea)\r\n");
  client.print("Accept: ");
  client.print(contentType);
  client.print("\r\n\r\n");
}
