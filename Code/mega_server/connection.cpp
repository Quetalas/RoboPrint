#include <AccelStepper.h>
#include "connection.h"

String Connection::data_pack = "";
uint16_t Connection::last_position = 0;

void Connection::begin() {
      Serial.setTimeout(300000);  // 5 минут
      Serial.begin(BAUD_RATE);
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
        if(String(data_pack[i]) == ",") {
          counter += 1;
        }
        //Serial.println("Counter " + String(counter));
      }
      return counter;
}

String Connection::getNextData() {
      if (data_pack.length() < 2) { // Если один символ или пусто
        if (data_pack == "") { return data_pack; }
        if (data_pack == String(DIVIDER)) { return ""; }
        return data_pack;
      }
      if (last_position > (data_pack.length() - 1)) {  // на случай если есть запятая в конце data_pack
        last_position = 0;
        return "";
      }
      //Serial.println("last_position " + String(last_position));
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

bool Connection::read_pack() {
      data_pack = "";
      last_position = 0;
      String next_byte = "";
      if (Serial.available() && Serial.find('{')) {
        while(true) {
          if(Serial.available()) {
            char b = Serial.read();
            next_byte = String(b);
            //Serial.println("next_byte " + next_byte);
            if(next_byte != "}") {
              data_pack += next_byte;
            }
            else { break; }
          }
        }
        return true;
      }
      else { data_pack = "";}
      return false;
}

void Connection::send_pack(const String& pack) {
      if(Serial.availableForWrite() >= (pack.length() + 2)) {
        Serial.print("{" + pack + "}");
      }
}
