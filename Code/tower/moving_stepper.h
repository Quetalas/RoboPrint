#pragma once
#include <AccelStepper.h>

#define X_END 3 //X Y E Z stopend pin's
#define Y_END 2  //у концевика инвертированная логика (отжат - 1, нажат - 0)
#define Z_END 15
#define E_END 14

#define X_STEP_PIN 54
#define X_DIR_PIN 55
#define X_ENABLE_PIN 38

#define Y_STEP_PIN 60
#define Y_DIR_PIN 61
#define Y_ENABLE_PIN 56

#define Z_STEP_PIN 46
#define Z_DIR_PIN 48
#define Z_ENABLE_PIN 62

#define EXT_STEP_PIN 26
#define EXT_DIR_PIN 28
#define EXT_ENABLE_PIN 24

#define E_STEP_PIN 36
#define E_DIR_PIN 34
#define E_ENABLE_PIN 30

#define ACCELERATION 10000
#define MAX_SPEED 1000

#define ONE_STEP 0.1

#define YZ_STEPPER_COORD 5250
#define XE_STEPPER_COORD -4000

void init_stepper(AccelStepper& stepper, const uint8_t& pin, const int& pos, const bool& dirinv);

void set_ext_pos();

void moving_gcode();

void moving_coordinats(const int x, const int y, const int z, const int e);

void ends_searc(const uint8_t pin1, AccelStepper& stepper1, AccelStepper& stepper11, 
const uint8_t pin2, AccelStepper& stepper2, AccelStepper& stepper22, const uint8_t dir, const long coordinate);

void autohome_calibrtion(); 

extern AccelStepper stepper_x, stepper_y, stepper_z, stepper_e, stepper_ext;
