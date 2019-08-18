#include <Arduino.h>
#include "temperature.h"
#include "connection.h"



Heater::Heater(){
  target_temp = 210;
  min_temp = 60;
  current_temp = 0;
}

void Heater::set_temp(const float& temp) {
  target_temp = temp;
}


float Heater::calculate_temp() {
  float temp, ur, Rth, Vo;
  Vo = analogRead(T_SENSOR_PIN); //пересчёт данных с термистра в температуру по цельсию
  Rth = R/(1023.0/Vo - 1); // thermisor resistance
  Rth = Rth/NOMINAL_RTH;
  ur = log(Rth);
  ur = ur/coef;
  ur = ur + 1.0/(NOMINAL_T + 273.15); 
  temp = 1.0/ur - 273.15;
  current_temp = temp;
  return current_temp;
}

void Heater::heating() const {
  float temp;
  do {
    temp = control_temp();
    Connection::send_pack(String(temp));
    delay(1000);
  } while(temp < target_temp && min_temp > target_temp);
}

float Heater::control_temp() const {
  float temp = calculate_temp();
  
  if (target_temp + DELTA_T < temp) {
    analogWrite(HEATER_PIN, pwm_to_temperature);
  }
  if (target_temp - DELTA_T > temp) {
    analogWrite(HEATER_PIN, 255);
  }
  
  return temp;
}

float Heater::get_current_temp() const {
  return current_temp;
}

Heater heater = Heater();
