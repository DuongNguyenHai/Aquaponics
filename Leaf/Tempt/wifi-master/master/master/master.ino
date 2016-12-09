#include <Arduino.h>

// Nguyen Hai Duong
// Nov 23 2016

// Tree system : Master <---> Bridge <---> Root
// Master : arduino
// Bridge : module wifi esp 8266
// Root : raspberry Pi / computer
// This code for the master
// The master will receive request from Root (via Bridge) and send value of temperature of fish tank to Root.
// In addition, its will automatic read temperature after each 30 minutes.

#include <LiquidCrystal.h>
#include <OneWire.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);      // select the pins used on the LCD panel
OneWire  ds(12);                          // on pin 12 (a 4.7K resistor is necessary)

#define SET_DEVICE "{\"set\":{\"temptTank\":{\"tempt1\": 1},\"DO\":{\"DO1\":1},\"amoniac\":{\"amoniac1\":1}}}"
#define BG_DATA "{\"data\":{"
#define ED_DATA "}}"
#define NOT_REAL -100 // the value will not never exist
// #define LED 13        // show to user by led (has not used)
#define PIN_READY 2   // When module Wifi had connected successful, The module wifi will pull this pin to be HIGH
#define PIN_READ 3    // it was used when user wanna read temperature. (ex: press button)
const char *command[] = {"set", "data"};
volatile int count=0;
volatile int temptSave = 0;
inline void SetTimer();   // set up timer for caculate tempt. The value of Timer was set on ISR function
float CalculateTempt();  // Calculate temperature from sensor
String JsonTempt();       // Create a string json with temperature. this string will be send to root
void ReadAndShowTempt();  // Call CalculateTempt() and show tempt to LCD
int GetCommand();       // get command from Root.
bool HandleSetDevice(); // Handle with command[set]. It scan all sensor to list which sensor is exist and send its infor to Root
bool HandleData();  // Handle with command[data]. It read data from sensors and send it to Root
void PrintInfor(String str, String status); // Show infor on LCD
void PrintTemptToLCD(float celsius);  // Show tempt on LCD
bool IsConnectionReady();     // check wherether connection between master and root was successful or not

byte addr[8];

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);               // start LCD library
  pinMode(PIN_READY, INPUT);
  pinMode(PIN_READ, INPUT);
  SetTimer();
}

void loop() {

  int index;
  // while(!IsConnectionReady()); // wait until module wifi has connected to root
  // HandleSetDevice();
  PrintInfor("SONG HE LAB", "READY");

  // Show temperature
  ReadAndShowTempt();
  // Waitting request from Root and hande correlative command
  while(1) {

    index = GetCommand();
    if(digitalRead(PIN_READ)) // Read tempt when PIN_READ got HIGH
      ReadAndShowTempt();

    switch(index) {
        case 0 : { // command: set
            // HandleSetDevice(); // Its was created for future functionality
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
  return true;
}

bool HandleData() {
  String data = JsonTempt();
  if(data.equals("F")) {
    PrintInfor("Calculate tempt ","FAIL");
    PrintTemptToLCD(temptSave);
    return false;
  }

  Serial.print(data);
  Serial.flush();
  PrintInfor("Send to root ","SUCCESS");
  PrintTemptToLCD(temptSave);
  return true;
}

String JsonTempt() {
  String data;
  float tempt = CalculateTempt();
  if(tempt==NOT_REAL) return "F";

  char tmp[6];
  dtostrf(tempt, 1, 2, tmp);
  data.concat(BG_DATA);
  data.concat("\"temptTank\":{\"tempt\":");
  data.concat(tmp);
  data.concat("}");
  data.concat(ED_DATA);
  return data;
}

void ReadAndShowTempt() {
  float tempt = CalculateTempt();
  if(tempt==NOT_REAL || tempt==0) { // the tempperature is really rarely got 0 oC
    PrintInfor("Reading SENSOR", "FAILED");
  }else {
    PrintTemptToLCD(tempt);
    temptSave = tempt;
  }
}

float CalculateTempt() {

  byte data[12];

  if (!ds.search(addr)) {
    ds.reset_search();
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
    return NOT_REAL; // the value will not never exist
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
  lcd.setCursor(0,1);
  lcd.print(status);
  delay(3000);
}

bool IsConnectionReady() {
  return (digitalRead(PIN_READY)) ? true : false; // if digitalRead(PIN_READY) is HIGH, It will return true
}

inline void SetTimer() {
  cli();                                  // Turn off global interrupt flag
  /* Reset Timer/Counter1 */
  TCCR1A = 0;
  TCCR1B = 0;
  TIMSK1 = 0;
  /* Setup Timer/Counter1 */
  TCCR1B |= (1 << CS12);                  // prescale = 256
  TCNT1 = 3036;
  TIMSK1 = (1 << TOIE1);                  // Overflow interrupt enable
  sei();                                  // Turn on global interrupt flag. We must do that if wanna use interrupt function
}

ISR (TIMER1_OVF_vect) {
    TCNT1 = 3036;
    count++;
    if(count==1799) { // 1799 seconds. CalculateTempt() will take approximate 1s. so the total time's about 30' minutes
      count=0;
      ReadAndShowTempt();
    }
}
