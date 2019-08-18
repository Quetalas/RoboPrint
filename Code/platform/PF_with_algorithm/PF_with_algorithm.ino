#include "connection.h"
#include <AccelStepper.h>
#include "moving_platform_stepper.h"

void setup() {
  Connection::begin();
  
  init_stepper(stepperx, X_ENABLE_PIN, 0, false);  // колесо заднее правое
  init_stepper(steppery, Y_ENABLE_PIN, 0, true);   // переднее правое
  init_stepper(stepperz, Z_ENABLE_PIN, 0, false);  //переднее левое
  init_stepper(steppere, E_ENABLE_PIN, 0, true);   //заднее левое

}

void loop() {
  char command[3];
  Connection::listen();
  stepperx.setSpeed(Connection::getNextData().toFloat());
  steppery.setSpeed(Connection::getNextData().toFloat());
  stepperz.setSpeed(Connection::getNextData().toFloat());
  steppere.setSpeed(Connection::getNextData().toFloat());
  while(true)
  {
      stepperx.runSpeed();
      steppery.runSpeed();
      stepperz.runSpeed();
      steppere.runSpeed();
      
      if (Serial.available() >= 3) //ждём сообщение {!}
      {
        Serial.readBytes(command, 3); 
        if (command[1] == '!')  // "!"
        {
          stepperx.setSpeed(0);
          steppery.setSpeed(0);
          stepperz.setSpeed(0);
          steppere.setSpeed(0);
          Connection::send_pack('0');
          break;
        }
       }
  }

}
