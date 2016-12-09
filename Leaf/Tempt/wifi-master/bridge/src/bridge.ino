// Nguyen Hai Duong
// Nov 23 2016

// Tree system : Master <---> Bridge <---> Root
// Master : arduino
// Bridge : module wifi esp 8266
// Root : raspberry Pi / computer
// This code for the Bridge
// Every send/receive between master and root was carried out by bridge (module wifi)

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti WiFiMulti;
WiFiClient client;          // Use WiFiClient class to create TCP connections
#define port 8888           // port open of server in raspberry
const char * host = "192.168.13.13"; // ip of Root
const char * host2 = "192.168.13.33"; // ip 2 of Root
#define WIFI_NAME "AquaOs"
#define WIFI_PASSWORD "ketvanbon"


#define PIN_READY 5 // Set this pin to be HIGH to inform MCU that connect to Root successed;
#define LED 2       // has the same meaning with PIN_READY but show to user by led

void setup() {

    // Firstly, should set PIN_READY
    pinMode(PIN_READY, OUTPUT);
    digitalWrite(PIN_READY, LOW);

    Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network
    WiFiMulti.addAP(WIFI_NAME, WIFI_PASSWORD);

    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(500);
    }

    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH); // set the light off
    delay(500);
}

void loop() {

  if (!client.connect(host, port)) {
      delay(5000);
      if (!client.connect(host2, port)) {
          delay(5000);
          return;
      }
  }

  digitalWrite(PIN_READY, HIGH); // inform to mcu that wifi has connected successful.
  digitalWrite(LED, LOW); // turn on the light (show to user that connection was established successful)

  while(1) {

    // Check if there is any data from MCU, and send all data through wifi
    if(Serial.available()) {
      while(Serial.available()){
        String strMaster = Serial.readString();
        client.print(strMaster);
        client.flush();
      }
    }

    // Check if there is any data was transmitted from wifi, and send data to MCU
    if(client.available()) {
      while(client.available()) {
        String strRoot = client.readStringUntil('\r');
        Serial.print(strRoot);
        Serial.flush();
      }
    }
  }
}
