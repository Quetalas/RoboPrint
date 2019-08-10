#pragma once

#define HEATER_PIN 10
#define T_SENSOR_PIN 13 // Analog pin
#define R 4700
#define NOMINAL_T 25 //номинальная температура, при которой Rth = номинальному сопротивлению термистра
#define coef 3950 //для термистора NTC
#define NOMINAL_RTH 100000 //thermisor EPCOS 100k is used

float get_temp();

void heating(const float& trg_temp, const bool& verbose=1);

void freeze(const bool& verbose=1);

extern float target_temp;
extern int freeze_temp;
