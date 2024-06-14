/* 
 *  Fall 2022
 *  ME-134 Robotics
 *  Analog Clock project by Kyu Rae Kim and Srisharan Kolige
 *  Code for receiver ESP32
*/

#include "SoundData.h"
#include "XT_DAC_Audio.h"

#define RXD2 16
#define TXD2 17

//XT_Wav_Class Sound1(chicken_am);
XT_Wav_Class Sound2(chicken_pm);
//XT_Wav_Class Sound3(zero);
XT_DAC_Audio_Class DacAudio(25,0);

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void loop() {
  DacAudio.FillBuffer();
  if (Serial2.read() == 2) {
    DacAudio.Play(&Sound2);
  }
//  else if (Serial2.read() == 1) {
//    DacAudio.Play(&Sound1);
//  }
//  else {
//    DacAudio.Play(&Sound3);
//  }
}
