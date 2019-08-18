#pragma once
#include <AccelStepper.h>

#define X_STEP_PIN 54
#define X_DIR_PIN 55
#define X_ENABLE_PIN 38

#define Y_STEP_PIN 60
#define Y_DIR_PIN 61
#define Y_ENABLE_PIN 56

#define Z_STEP_PIN 46
#define Z_DIR_PIN 48
#define Z_ENABLE_PIN 62

#define E_STEP_PIN 26
#define E_DIR_PIN 28
#define E_ENABLE_PIN 24

#define EXT_STEP_PIN 36
#define EXT_DIR_PIN 34
#define EXT_ENABLE_PIN 30

#define ACCELERATION 100000000
#define MAX_SPEED 1000

#define ONE_STEP 0.1


void init_stepper(AccelStepper& stepper, const uint8_t& pin, const int& pos, const bool& dirinv);

void moving_gcode();

extern AccelStepper stepper_x, stepper_y, stepper_z, stepper_e, stepper_ext;
