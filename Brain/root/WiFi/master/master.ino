// Nguyen Hai Duong
// Nov 23 2016

// Tree system : Master <---> Bridge <---> Root
// Its the master in Tree system
// Every send/receive between master and root was carried out via bridge (module wifi)

#include<LiquidCrystal.h>
#include<OneWire.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);      // select the pins used on the LCD panel
OneWire  ds(12);                          // on pin 12 (a 4.7K resistor is necessary)

#define SET_DEVICE "{\"set\":{\"temptTank\":{\"tempt1\": 1},\"DO\":{\"DO1\":1},\"amoniac\":{\"amoniac1\":1}}}"
#define BG_DATA "{\"data\":{"
#define ED_DATA "}}"

// #define LED 13        // show to user by led (has not used)
#define PIN_READY 2   // When module Wifi had connected successful, The module wifi will pull this pin to be HIGH

char *command[] = {"set", "data"};

String JsonTempt();       // Create a string json with temperature. this string will be send to root
float CalculateTempt();  // Calculate temperature from sensor
int GetCommand();       // get command from Root.
bool HandleSetDevice(); // Handle with command[set]. It scan all sensor to list which sensor is exist and send its infor to Root
bool HandleData();  // Handle with command[data]. It read data from sensors and send it to Root
void PrintInfor(String str, String status); // Show infor on LCD
void PrintTemptToLCD(float celsius);  // Show tempt on LCD
bool IsConnectionReady();     // check wherether connection between master and root was successful or not

byte addr[8];

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);               // start the library
  pinMode(PIN_READY, INPUT);
  delay(500); // delay to asure that eveything will start correctly. Im not sure how much time is suitable
}

void loop() {
  int index;
  
  // wait until module wifi has connected to root
  while(!IsConnectionReady());
  // HandleSetDevice(); 
  
  while(1) {

    float tempt = CalculateTempt();
    if(!tempt) return;
    // PrintTemptToLCD(tempt);
    index = GetCommand();
    switch(index) {
        case 0 : { // command: set
            HandleSetDevice();
        } break;
        case 1 : { // command: data
            HandleData();
        } break;
        default: break;
    }
  }
}

int GetCommand() {
  // get command from esp8266
  if(Serial.available()) {
    while(Serial.available()) {
      String line = Serial.readString();
      for(int i=0; i<2; i++) { // check command correspond in command array and return index of command
        if(line.equals(command[i]))
          return i;
      }
    } 
  }
  return -1;
}

bool HandleSetDevice() {
  Serial.print(SET_DEVICE);
  Serial.flush();
}

bool HandleData() {
  String data = JsonTempt();
  Serial.print(data);
  Serial.flush();
}

String JsonTempt() {
  String data;
  float tempt = CalculateTempt();
  if(!tempt) return NULL;

  char tmp[6];
  dtostrf(tempt, 1, 2, tmp);
  data.concat(BG_DATA);
  data.concat("\"temptTank\":{\"tempt1\":");
  data.concat(tmp);
  data.concat("}");
  data.concat(ED_DATA);

  return data;
}

float CalculateTempt() { 

  byte data[12]; 
  
  if ( !ds.search(addr)) {
    ds.reset_search();
  } 

  if (OneWire::crc8(addr, 7) != addr[7]) {
    return NULL;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);                // start conversion, with parasite power on at the end

  delay(1000);                      // maybe 750ms is enough, maybe not

  ds.reset();
  ds.select(addr);    
  ds.write(0xBE);                   // Read Scratchpad

  for (int i = 0; i < 9; i++) {     // we need 9 bytes
    data[i] = ds.read();
  }

  float celsius = (float)((data[1] << 8) | data[0]) / 16.0;
  return celsius;
}

void PrintTemptToLCD(float celsius) {
  lcd.clear();
  lcd.setCursor(2,0);             // set the LCD cursor   position 
  lcd.print("Song He Lab!");      // print a simple message on the LCD
  lcd.setCursor(2,1);             // move cursor to second line "1" and 9 spaces over
  lcd.print("T = ");
  lcd.setCursor(7,1);               // move cursor to second line "1" and 7 spaces over
  lcd.print(celsius);
  lcd.setCursor(13,1);
  lcd.print("oC");
}

void PrintInfor(String str, String status) {
  lcd.clear();
  lcd.setCursor(0,0);             // move cursor to second line "1" and 9 spaces over
  lcd.print(str);
  lcd.setCursor(13,0);
  lcd.print(status);
  delay(2000);
}

bool IsConnectionReady() {
  return (digitalRead(PIN_READY)) ? true : false; // if digitalRead(PIN_READY) is HIGH, It will return true
}