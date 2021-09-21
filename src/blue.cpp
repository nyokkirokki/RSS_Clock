// ###########################################################
//
//     Bluetoothにて操作する関数 blue.cpp
//
//        
//
// ###########################################################



#include <Arduino.h>
#include "BluetoothSerial.h"
#include "blue.hpp"

unsigned char flag_blue = 0;

BluetoothSerial SerialBT;


void bluetooth() { 

  if(flag_blue == 0){
    SerialBT.begin("ESP32Bluetooth"); //Bluetooth表示名を決定
    flag_blue = 1;
    }

           
           SerialBT.println("Hello World");
  
}
