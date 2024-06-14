/* 
 *  Fall 2022
 *  ME-134 Robotics
 *  Analog Clock project by Kyu Rae Kim and Srisharan Kolige
 *  Code for transmitter ESP32
*/

#include <ESP32Servo.h>
#include <WiFi.h>
#include "time.h"

#define RXD2 16                               // UART2 RX pin
#define TXD2 17                               // UART2 TX pin
#define servoPin1 27                          // Servo 1,
#define servoPin2 14                          //       2,
#define servoPin3 12                          //       3 pins
#define buttonPin 33                          // Button pin

Servo servo1;
Servo servo2;
Servo servo3;

const char* ssid     = "OnePlus 5T";          // WiFi SSID
const char* password = "12345678";            // WiFi password
const char* ntpServer = "pool.ntp.org";       // NTP server
const long  gmtOffset_sec = -18000;           // GMT offset
const int   daylightOffset_sec = 3600;        // Daylight saving time offset
int buttonState_p = 0;                        // Previous button state
int buttonState_c = 0;                        // Current button state
int h;                                        // Hours
int m;                                        // Minutes
int s;                                        // Seconds
int s2;                                       // Seconds divided by 2
int pos1 = 0;                                 // Servo 1 position
int pos2 = 0;                                 // Servo 2 position
int pos3 = 0;                                 // Servo 3 position
bool am = true;                               // AM/PM boolean
bool finished = true;                         // Servo in action boolean


// ---------------------------------------------------------------------------------------------


void getEasternTime(){
  struct tm currentTime;
  if(!getLocalTime(&currentTime)){
    Serial.println("Failed to obtain time");
    return;
  }
  
  h = currentTime.tm_hour;
  m = currentTime.tm_min;
  s = currentTime.tm_sec;
  s2 = floor(s/2);

  if (h >= 12) {
    am = false;
    h %= 12;
  }
  if (h == 0) {
    h = 12;
  }
}


void setup(){
  // UART settings
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  // Connect servos to each pins
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  servo3.attach(servoPin3);

  // Set button as input
  pinMode(buttonPin, INPUT);
  
  // Connect to WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  
  // Initialize and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  getEasternTime();

  // Initialize positions of servos
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);
}


void loop(){
  buttonState_c = digitalRead(buttonPin);

  // Prevent consecutive button press and button hold during servo motions
  if (buttonState_c == LOW || buttonState_c == buttonState_p || finished == false) {
    buttonState_p = buttonState_c;
    return;
  }

  // Get local time every time button is pressed
  getEasternTime();

  // Now servos are in action
  finished = false;
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

  // Servo for hours
  for (int i = 0; i < h; i++) {
    // if (am) { Serial2.write(1); }
    // else { Serial2.write(2); }
    Serial2.write(2);
    for (pos1 = 0; pos1 <= 180; pos1 += 10) {
      servo1.write(pos1);
      delay(15);
    }
    delay(300);
    for (pos1 = 180; pos1 >= 0; pos1 -= 10) {
      servo1.write(pos1);
      delay(15);
    }
    delay(700);
  }
  delay(500);

  // Servo for minutes
  for (int j = 0; j < m; j++) {
    for (pos2 = 0; pos2 <= 90; pos2 += 10) {
      servo2.write(pos2);
      delay(15);
    }
    for (pos2 = 90; pos2 >= 0; pos2 -= 10) {
      servo2.write(pos2);
      delay(15);
    }
  }
  // if (m == 0) { Serial2.write(3); }
  delay(500);

  // Servo for seconds
  for (int k = 0; k < s2; k++) {
    for (pos3 = 0; pos3 <= 72; pos3 += 4) {
      servo3.write(pos3);
      delay(15);
    }
    for (pos3 = 72; pos3 >= 0; pos3 -= 3) {
      servo3.write(pos3);
      delay(15);
    }
  }
  if (s % 2 == 1) {
    for (pos3 = 0; pos3 <= 72; pos3 += 3) {
      servo3.write(pos3);
      delay(15);
    }
  }
  // if (s == 0) { Serial2.write(3); }

  // Servos actions are finished
  buttonState_p = buttonState_c;
  finished = true;

  // Avoid bouncing
  delay(50);
}
