// Nguyen Hai Duong
// Dec 10 2016

// Tree system : Master <---> Bridge <---> Root
// Master : arduino
// Bridge : module wifi esp 8266. its UDP server
// Root : raspberry Pi / computer
// This code for the Bridge
// Every send/receive between master and root was carried out by bridge (module wifi)
// In case the connection is lost, ESP8266 will automatically reconnect to the last used access point, once it is again available. (its default of ESP8266 Station)
// Reference :
// https://github.com/esp8266/Arduino/blob/master/doc/esp8266wifi/readme.md#quick-start

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

WiFiUDP Udp;         // Use WiFiClient class to create UDP connections
#define port 8888           // port open of server in raspberry
#define ssid "AquaOs"  // network name
#define password "ketvanbon" // password
#define PIN_READY 5 // Set this pin to be HIGH to inform MCU that connect to Root successed;
#define LED 2       // has the same meaning with PIN_READY but show to user by led
#define BUFFER_SIZE 255
IPAddress ip_static(192,168,13,13);
IPAddress ip_gateway(192,168,13,1);
IPAddress ip_subnet(255,255,255,0);

String root_ip;
int root_port;
int packetSize;
char incomingPacket[BUFFER_SIZE];  // buffer for incoming packets

WiFiEventHandler gotIpEventHandler, disconnectedEventHandler;

void setup() {

  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH); // set the light off

  gotIpEventHandler = WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP& event) {
    digitalWrite(LED, LOW); // set the light on
  });

  disconnectedEventHandler = WiFi.onStationModeDisconnected([](const WiFiEventStationModeDisconnected& event) {
    digitalWrite(LED, HIGH); // set the light off
  });

  WiFi.begin(ssid, password);
  // DHCP was setted by default. To set static ip use config line below.
  // bool config(IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dns1 = (uint32_t)0x00000000, IPAddress dns2 = (uint32_t)0x00000000);
  WiFi.config(ip_static, ip_gateway, ip_subnet); // set static ip. ignore this line to use DHCP

  while (WiFi.status() != WL_CONNECTED) { delay(500); }

  Udp.begin(port);
  // Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), port);
}

void loop() {

  // Check if there is any data was transmitted from wifi, and send data to MCU
  if (packetSize = Udp.parsePacket()) {
    // Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    root_ip = Udp.remoteIP().toString();  // important to save root_ip cause its will be erase in the next parsePacket()
    root_port = Udp.remotePort(); // important to save root_port
    int len = Udp.read(incomingPacket, BUFFER_SIZE);
    if (len > 0) {
      incomingPacket[len] = 0;
    }

    Serial.print(incomingPacket);
    Serial.flush();
  }

  // Check if there is any data from MCU, and send all data through wifi
  if(Serial.available()) {
    while(Serial.available()) {
      String strMaster = Serial.readString();
      Udp.beginPacket(root_ip.c_str(), root_port);
      Udp.print(strMaster);
      Udp.endPacket();
    }
  }

}
