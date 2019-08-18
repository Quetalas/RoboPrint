#pragma once

#define X_STEP_PIN 2
#define X_DIR_PIN 5
#define X_ENABLE_PIN 8

#define Y_STEP_PIN 3
#define Y_DIR_PIN 6
#define Y_ENABLE_PIN 8

#define Z_STEP_PIN 4
#define Z_DIR_PIN 7
#define Z_ENABLE_PIN 8

#define E_STEP_PIN 12
#define E_DIR_PIN 13
#define E_ENABLE_PIN 8

#define ACCELERATION 100000
#define MAX_SPEED 1000


void init_stepper(AccelStepper& stepper, const uint8_t& pin, const int& pos, const bool& dirinv);


extern AccelStepper stepperx, steppery, stepperz, steppere; 
