#include "heating.h"
#include "connection.h"
#include "moving_stepper.h"

enum commands {G0=0, G1=1, ECHO=2, GET_TEMP=3, SET_TEMP=4, HEAT=5, FREEZE=6};

void setup() {
  Connection::begin();
  
  init_stepper(stepper_x, X_ENABLE_PIN, round(-290 / ONE_STEP), true);
  init_stepper(stepper_y, Y_ENABLE_PIN, round(290/ONE_STEP), false);
  init_stepper(stepper_z, Z_ENABLE_PIN, round(290/ONE_STEP), true);
  init_stepper(stepper_e, E_ENABLE_PIN, round(-290 / ONE_STEP), false);
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
          Connection::send_pack(String(get_temp()));
          break;
      case SET_TEMP:
          target_temp = Connection::getNextData().toFloat();
          Connection::send_pack("0");
          break;
      case HEAT:
          heating(target_temp);
          Connection::send_pack("0");
          break;
      case FREEZE:
          freeze();
          Connection::send_pack("0");
          break;
    }
  }
}
