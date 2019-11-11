#pragma once

#define HEATER_PIN 10
#define T_SENSOR_PIN 13 // Analog pin
#define R 4700
#define NOMINAL_T 25 //номинальная температура, при которой Rth = номинальному сопротивлению термистра
#define coef 3950 //для термистора NTC
#define NOMINAL_RTH 100000 //thermisor EPCOS 100k is used

#define DELTA_T 2 // амплитуда изменения температуры без регулировки

#define WITH_HEATING true

class Heater {
  public:
    Heater();
    
    void set_temp(const float& temp);
    
    float calculate_temp();
    
    void heating() const;
    
    float control_temp();

    float get_current_temp() const;
    
  private:
    float target_temp;
    int8_t min_temp;
    float current_temp;
    int current_PWM;
};

extern Heater heater;
