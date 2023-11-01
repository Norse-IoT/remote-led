#include "nkuwifi.h"

const char* ssid = "NORSE_NET";             // SSID
const char* host = "iot.informatics.plus";  //external server domain for HTTPS connection
const int port = 443;

WiFiClientSecure* myclient;

void setup() {
  Serial.begin(115200);
  delay(10);
  myclient = connectToWifi(ssid);
}

int counter = 0;
int success = 0;

void loop() {
  if (WiFi.status() == WL_CONNECTED) {  //if we are connected to eduroam network
    counter = 0;                        //reset counter
    Serial.println("Wifi is still connected with IP: ");
    Serial.println(WiFi.localIP());            //inform user about his IP address
  } else if (WiFi.status() != WL_CONNECTED) {  //if we lost connection, retry
    WiFi.begin(ssid);
  }
  while (WiFi.status() != WL_CONNECTED) {  //during lost connection, print dots
    delay(500);
    Serial.print(".");
    counter++;
    if (counter >= 60) {  //30 seconds timeout - reset board
      ESP.restart();
    }
  }
  if (success == 0) {
    Serial.print("Connecting to website: ");
    Serial.print(host);
    Serial.print(" on port ");
    Serial.println(port);
    if (myclient->connect(host, port)) {
      String url = "/wifi.txt";
      myclient->print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "User-Agent: ESP32\r\n" + "Connection: close\r\n\r\n");
      while (myclient->connected()) {
        String header = myclient->readStringUntil('\n');
        Serial.println(header);
        if (header == "\r") {
          break;
        }
      }
      String line = myclient->readStringUntil('\n');
      Serial.println("The site returns this data:");
      Serial.println(line);
      Serial.println();
      success = 1;
    } else {
      Serial.println("Connection unsucessful");
    }
  }
  delay(30000);
}