#include <AccelStepper.h>
#include "moving_platform_stepper.h"
#include "connection.h"

AccelStepper stepperx(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper steppery(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN); 
AccelStepper stepperz(AccelStepper::DRIVER, Z_STEP_PIN, Z_DIR_PIN);
AccelStepper steppere(AccelStepper::DRIVER, E_STEP_PIN, E_DIR_PIN);
//AccelStepper stepper_ext(AccelStepper::DRIVER, EXT_STEP_PIN, EXT_DIR_PIN);

void init_stepper(AccelStepper& stepper, const uint8_t& pin, const int& pos, const bool& dirinv)
{ //dirinv true для x и z, false для y и e
  stepper.setEnablePin(pin); 
  stepper.setPinsInverted(dirinv, false, true); 
  stepper.enableOutputs();
  stepper.setMaxSpeed(MAX_SPEED);
  stepper.setAcceleration(ACCELERATION);
  stepper.setCurrentPosition(pos);
}
