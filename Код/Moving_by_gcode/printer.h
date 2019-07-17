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

#define ACCELERATION 10000000
#define MAX_SPEED 200

#define ONE_STEP  0.1 // в одном шаге 0.1мм

#define R 405  //мм, длина ножки
extern long R_2;

#define L 1000 //мм, длина принтера (меньше чем на самом деле)

#define r 70   //мм, расстояние между ножками (на верхней платформе)
extern float r_sqrt_2;

#define T_WEIGHT_HALF 115 // мм половина ширины принтера

#define EXT_HEIGHT 70 // мм, высота головки (над верхней платформой)

struct Carriages {
  float car_x, car_y, car_z, car_e; // абсолютные координаты кареток в мм
};

struct Position {
  float X, Y, Z;  // абсолютные координаты сопла в мм   
};

void init_stepper(AccelStepper& stepper, const uint8_t& pin, const int& pos);

void set_new_pos(Carriages& cars, const Position& new_pos);

extern Carriages cars;
extern Position current_pos;
extern AccelStepper stepper_x;
extern AccelStepper stepper_y;
extern AccelStepper stepper_z;
extern AccelStepper stepper_e;

void move_to(float& X, float& Y, float& Z, float& E);

void move_down();