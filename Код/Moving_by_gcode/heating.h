#pragma once
#include <AccelStepper.h>

#define E1_STEP_PIN 36
#define E1_DIR_PIN 34
#define E1_ENABLE_PIN 30

#define HEATER_PIN 10
#define T_SENSOR_PIN 13 // Analog pin
#define RESISTANCE 4700
#define NOMINAL_T 25 //номинальная температура, при которой Rth = номинальному сопротивлению термистра
#define coef 3950 //для термистора NTC
#define NOMINAL_RTH 100000 //thermisor EPCOS 100k is used

#define EXTR_STEP 0.1

extern AccelStepper stepper_extr;

extern bool do_print;

float get_temprature();
void run_print();
void heating();