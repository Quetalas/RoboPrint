/*
 * ДЛЯ КОРРЕКТНОЙ РАБОТЫ СЛЕДУЕТ УВЕЛИЧИТЬ РАЗМЕР БУФЕРА АРДУИНО (256 - рабочий вариант)
 * это делается в HardwareSerial.h
 * SERIAL_RX_BUFFER_SIZE
 * SERIAL_RX_BUFFER_SIZE
 */
#include "temperature.h"
#include "connection.h"
#include "moving_stepper.h"
#include "touch_sensor.h"

enum commands {G0=0, G1=1, ECHO=2, GET_TEMP=3, SET_TEMP=4, HEAT=5, G92=7, G28=8,
                PROBE_UP = 9, PROBE_DOWN = 10, IS_TOUCHED = 11}; //, G28=8,  6  пусто

void setup() {
  Connection::begin();

  
  init_stepper(stepper_x, X_ENABLE_PIN, round(-293 / ONE_STEP), true);
  init_stepper(stepper_y, Y_ENABLE_PIN, round(293/ONE_STEP), false);
  init_stepper(stepper_z, Z_ENABLE_PIN, round(293/ONE_STEP), true);
  init_stepper(stepper_e, E_ENABLE_PIN, round(-293 / ONE_STEP), false);
  init_stepper(stepper_ext, EXT_ENABLE_PIN, 0, true);
  
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
          Connection::send_pack(String(heater.calculate_temp()));
//          Connection::send_pack(heater.calculate_temp());
          break;
      case SET_TEMP:
          heater.set_temp(Connection::getNextData().toFloat());
          Connection::send_pack("0");
          break;
      case HEAT:
          heater.heating();
          Connection::send_pack("0");
          break;
      case G92:
          set_ext_pos();
          Connection::send_pack("0");
          break;
      case G28:
          autohome_calibrtion(); 
          Connection::send_pack("0");
          break;
      case IS_TOUCHED:              
          if (touchProbe.isTouched()){
            Connection::send_pack("y");
          }
          else {
            Connection::send_pack("n");
          }
          break;
      case PROBE_UP:
          touchProbe.up();
          Connection::send_pack("0");
          break;
      case PROBE_DOWN:
          touchProbe.down();
          Connection::send_pack("0");
          break;        
    }
  }
}
