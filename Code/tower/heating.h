#pragma once

#define HEATER_PIN 10
#define T_SENSOR_PIN 13 // Analog pin
#define R 4700
#define NOMINAL_T 25 //номинальная температура, при которой Rth = номинальному сопротивлению термистра
#define coef 3950 //для термистора NTC
#define NOMINAL_RTH 100000 //thermisor EPCOS 100k is used
#define DEL_T 5 // амплитуда изменения температуры без регулировки

class Heating {
  public:
    Heating();
    void get_target_temp(float term);
    float calculate_temp();
    void heating();
    void support();
  private:
    bool sup_flag;
    float target_temp;
    int sup_temp;
    

};

extern Heating HeatClass;
