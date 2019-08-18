#include <Arduino.h>
#include "heating.h"
#include "connection.h"



Heating::Heating(){
  sup_flag = false;
  target_temp = 210;
  sup_temp = 0;
  
}

void Heating::get_target_temp(float term){
  target_temp = term;
}


float Heating::calculate_temp() {
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

void Heating::heating() {
  if ( target_temp != 0 ) {
    analogWrite(HEATER_PIN, 255);
    sup_flag = true;
    sup_temp = 50;
    while(calculate_temp() <= target_temp)
    {
       Connection::send_pack(String(calculate_temp()));
       delay(1000);  //убрать, заменить
    }  
    analogWrite(HEATER_PIN, sup_temp);
  }
  else if (target_temp == 0) 
  {
    analogWrite(HEATER_PIN, 0);
    sup_flag = false;
  }
 }

void Heating::support() {
    float calc = calculate_temp();
    if (sup_flag && ( target_temp - DEL_T >= calc ) ) {
      sup_temp += round( target_temp - calc) - DEL_T;
      sup_temp = constrain(sup_temp, 0, 255);
      analogWrite(HEATER_PIN, sup_temp);
    }
    if (sup_flag && ( target_temp + DEL_T <= calc) ){
      sup_temp += round( target_temp - calc) + DEL_T;
      sup_temp = constrain(sup_temp, 0, 255);
      analogWrite(HEATER_PIN, sup_temp);
    }
    //Connection::send_pack(String(calc));
    
}

Heating HeatClass = Heating();
