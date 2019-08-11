#include <AccelStepper.h>
#include "heating.h"
#include "connection.h"
#include "dalnomer.h"

enum commands {G0=0, G1=1, ECHO=2, GET_TEMP=3, SET_TEMP=4, HEAT=5, FREEZE=6};

void setup() {
  Connection::begin();
  init_dalnomer();
}

void loop() {
  String cmd;
  if (Connection::read_pack()) {
    uint16_t pack_size = Connection::packLen();
    if(pack_size > 0) {
      cmd = Connection::getNextData();
    }
    switch(cmd.toInt()) {
      case G0:
          break;
      case G1:
          break;
      case ECHO:
          Connection::send_pack(Connection::getDataPack());
          break;
      case GET_TEMP:
          Connection::send_pack(String(get_temp()));
          break;
      case SET_TEMP:
          target_temp = Connection::getNextData().toFloat();
          break;
      case HEAT:
          heating(target_temp);
          Connection::send_pack("Complete");
          break;
      case FREEZE:
          freeze();
          break;
    }
  }
}
