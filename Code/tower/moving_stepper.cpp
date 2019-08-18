#include <AccelStepper.h>
#include "moving_stepper.h"
#include "connection.h"

AccelStepper stepper_x(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper stepper_y(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN); 
AccelStepper stepper_z(AccelStepper::DRIVER, Z_STEP_PIN, Z_DIR_PIN);
AccelStepper stepper_e(AccelStepper::DRIVER, E_STEP_PIN, E_DIR_PIN);
AccelStepper stepper_ext(AccelStepper::DRIVER, EXT_STEP_PIN, EXT_DIR_PIN);

void init_stepper(AccelStepper& stepper, const uint8_t& pin, const int& pos, const bool& dirinv) { 
  //dirinv true для x и z, false для y и e
  stepper.setEnablePin(pin); 
  stepper.setPinsInverted(dirinv, false, true); 
  stepper.enableOutputs();
  stepper.setMaxSpeed(MAX_SPEED);
  stepper.setAcceleration(ACCELERATION);
  stepper.setCurrentPosition(pos);
}

void moving_gcode() {
  stepper_x.moveTo(Connection::getNextData().toInt());  
  stepper_y.moveTo(Connection::getNextData().toInt());
  stepper_z.moveTo(Connection::getNextData().toInt()); 
  stepper_e.moveTo(Connection::getNextData().toInt());
  
  stepper_x.setSpeed(Connection::getNextData().toFloat());
  stepper_y.setSpeed(Connection::getNextData().toFloat());
  stepper_z.setSpeed(Connection::getNextData().toFloat());
  stepper_e.setSpeed(Connection::getNextData().toFloat());
  
  if (Connection::packLen() > 9) {
    stepper_ext.moveTo(Connection::getNextData().toInt());
  	stepper_ext.setSpeed(Connection::getNextData().toFloat());
  }
  
  while((stepper_x.distanceToGo() != 0) || 
        (stepper_y.distanceToGo() != 0) ||
        (stepper_z.distanceToGo() != 0) || 
        (stepper_e.distanceToGo() != 0) ||
        (stepper_ext.distanceToGo() != 0)
        ) {
          
    stepper_x.runSpeedToPosition();
    stepper_y.runSpeedToPosition();
    stepper_z.runSpeedToPosition();
    stepper_e.runSpeedToPosition();
    stepper_ext.runSpeedToPosition();
  }  
}
