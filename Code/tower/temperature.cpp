#include <Arduino.h>
#include "temperature.h"
#include "connection.h"



Heater::Heater(){
  target_temp = 0;
  current_temp = 0;
  min_temp = 50;
  current_PWM = 255;
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
  } while(temp < target_temp && min_temp < target_temp);
}

float Heater::control_temp() {
  float temp = calculate_temp();
  if ( min_temp < target_temp ) {
    if (target_temp + DELTA_T < temp) {
      current_PWM += round( target_temp - temp) + DELTA_T;
      current_PWM = constrain(current_PWM, 0, 255);
      analogWrite(HEATER_PIN, current_PWM);
    }
    else if (target_temp - DELTA_T > temp) {
      current_PWM += round( target_temp - temp) - DELTA_T;
      current_PWM = constrain(current_PWM, 0, 255);
      analogWrite(HEATER_PIN, current_PWM);
    }
    else 
      analogWrite(HEATER_PIN, current_PWM);
  }  
  return temp; //, current_PWM
}

float Heater::get_current_temp() const {
  return current_temp;
}

Heater heater = Heater();
