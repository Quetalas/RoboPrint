#include <Arduino.h>
#include "heating.h"
#include "connection.h"

float target_temp = 100;
int freeze_temp = 30;

float get_temp() {
  float temp, ur, Rth, Vo;
  Vo = analogRead(T_SENSOR_PIN); //пересчёт данных с термистра в температуру по цельсию
  Rth = R/(1023.0/Vo - 1); // thermisor resistance
  Rth = Rth/NOMINAL_RTH;
  ur = log(Rth);
  ur = ur/coef;
  ur = ur + 1.0/(NOMINAL_T + 273.15); 
  temp = 1.0/ur - 273.15;
  return temp;
}

void heating(const float& trg_temp, const bool& verbose=1) {
  analogWrite(HEATER_PIN, 255);
  while(get_temp() < trg_temp)
  {
    if(verbose){
       Connection::send_pack(String(get_temp()));
       delay(1000);
    }
  }
  analogWrite(HEATER_PIN, 50);
}

void freeze(const bool& verbose=1) {
  analogWrite(HEATER_PIN, 0);
  while(get_temp() > freeze_temp) {
    if(verbose){
       Connection::send_pack(String(get_temp()));
       delay(1000);
    }
  }
}
