/*
 * ДЛЯ КОРРЕКТНОЙ РАБОТЫ СЛЕДУЕТ УВЕЛИЧИТЬ РАЗМЕР БУФЕРА АРДУИНО (256 - рабочий вариант)
 * это делается в HardwareSerial.h
 * SERIAL_RX_BUFFER_SIZE
 * SERIAL_RX_BUFFER_SIZE
 */
/*
 * Проблемы:
 *  -Избавиться от конкатенации строк
 *  -Перейти на char[] вместо String
 */
#include <Arduino.h>
#include "connection.h"
#include "temperature.h"

String Connection::data_pack = "";
uint16_t Connection::last_position = 0;

void Connection::begin(const unsigned long& speed=115200) {
  Serial.setTimeout(1000);  // 1 секунда
  Serial.begin(speed);
}

String Connection::getDataPack() { return data_pack; }

int Connection::packLen() {
  /*
  *   "" - нет данных в пакете
  *   "a" - одна еденицы данных
  *   "," - две еденицы ("" и "")
  *   ",a" - две
  *   "a," - две еденицы (a и "")
  *   "1,2" - две
  *   ",," - три : "(0),(1),(2)"
  *   "a,b,c" - три
  */
  if(data_pack.length() < 2) { return data_pack.length(); }
  
  uint16_t counter = 1;
  for(int i = 0; i < data_pack.length(); ++i) {
    if(data_pack[i] == ',') {
      counter += 1;
    }
  }
  return counter;
}

String Connection::getNextData() {
  if (data_pack.length() < 2) { // Если один символ или пусто
    if (data_pack == "") { return data_pack; }
    if (data_pack[0] == DIVIDER) { return ""; }
    return data_pack;
  }
  if (last_position > (data_pack.length() - 1)) {  // на случай если есть запятая в конце data_pack
    last_position = 0;
    return "";
  }
  
  String res;
  uint16_t divider_index = data_pack.indexOf(DIVIDER, last_position);
  if (divider_index != -1) {
    res = data_pack.substring(last_position, divider_index);
    last_position = divider_index + 1;
  }
  else {
    res = data_pack.substring(last_position); // до конца строки
    last_position = 0;
  }
  return res;
}

void Connection::listen() {
  char next_byte;
  data_pack = "";
  last_position = 0;
  while(true) {
    if(WITH_HEATING) {
      heater.control_temp();
    }
    if(Serial.find('{')) {
      while(true) {
        if(WITH_HEATING) {
          heater.control_temp();
        }
        if (Serial.available()) {
          next_byte = Serial.read();
          if(next_byte != '}') {
            data_pack += String(next_byte);
          }
          else {
            return;
          }
        }
      }
    }
  }
}

void Connection::send_pack(const String& pack) {
  while(true) {
    if(Serial.availableForWrite() >= (pack.length() + 2)) {
      Serial.print("{" + pack + "}");
      return;
    }
  }
}
