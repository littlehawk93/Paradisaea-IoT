
bool connectWifi(char *ssid, char *password, long timeout, void (*callback)(int)) {

  WiFi.hostname("Paradisaea");
  
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);

  long startTime = millis();
  int attempt = 0;

  while(millis() - startTime < timeout) {
    
    if (WiFi.status() == WL_CONNECTED) {
      return true;
    }
    callback(attempt);
    attempt++;
    delay(1000);
  }
  return false;
}