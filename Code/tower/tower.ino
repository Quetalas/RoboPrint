/*
 * ДЛЯ КОРРЕКТНОЙ РАБОТЫ СЛЕДУЕТ УВЕЛИЧИТЬ РАЗМЕР БУФЕРА АРДУИНО (256 - рабочий вариант)
 * это делается в HardwareSerial.h
 * SERIAL_RX_BUFFER_SIZE
 * SERIAL_RX_BUFFER_SIZE
 */

// экстредер добавлен в эту прогу
#include "heating.h"
#include "connection.h"
#include "moving_stepper.h"


enum commands {G0=0, G1=1, ECHO=2, GET_TEMP=3, SET_TEMP=4, HEAT=5};

void setup() {
  Connection::begin();
  
  init_stepper(stepper_x, X_ENABLE_PIN, round(-293 / ONE_STEP), true);
  init_stepper(stepper_y, Y_ENABLE_PIN, round(293/ONE_STEP), false);
  init_stepper(stepper_z, Z_ENABLE_PIN, round(293/ONE_STEP), true);
  init_stepper(stepper_e, E_ENABLE_PIN, round(-293 / ONE_STEP), false);
  init_stepper(stepper_ext, EXT_ENABLE_PIN, 0, true);  //какое направление ???
  //для нагрева
  pinMode(T_SENSOR_PIN, INPUT);
  pinMode(HEATER_PIN, OUTPUT);
}

void loop() {
  Connection::listen();
  if(Connection::packLen() > 0) {
    switch(Connection::getNextData().toInt()) {
      case G0:
          moving_gcode();
          Connection::send_pack("0");
          break;
      case G1:
          moving_gcode();
          Connection::send_pack("0");
          break;
      case ECHO:
          Connection::send_pack(Connection::getDataPack());
          break;
       case GET_TEMP:
          Connection::send_pack(String(HeatClass.calculate_temp()));
          break;
      case SET_TEMP:
          HeatClass.get_target_temp( Connection::getNextData().toFloat() );
          Connection::send_pack("0");
          break;
      case HEAT:
          HeatClass.heating();
          Connection::send_pack("0");
          break;
    }
  }
}
