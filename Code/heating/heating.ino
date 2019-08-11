#include <AccelStepper.h>
#include <SD.h>

#define E_STEP_PIN 36
#define E_DIR_PIN 34
#define E_ENABLE_PIN 30

#define HEATER_PIN 10
#define T_SENSOR_PIN 13 // Analog pin
#define R 4700
#define NOMINAL_T 25 //номинальная температура, при которой Rth = номинальному сопротивлению термистра
#define coef 3950 //для термистора NTC
#define NOMINAL_RTH 100000 //thermisor EPCOS 100k is used

#define ACCELERATION 10000000
#define MAX_SPEED 1000
#define ONESTEP 0.1

AccelStepper stepper_e(AccelStepper::DRIVER, E_STEP_PIN, E_DIR_PIN);

void init_stepper(AccelStepper& stepper, const uint8_t& pin, const int& pos)
{
  stepper.setEnablePin(pin); 
  stepper.setPinsInverted(true, false, true); 
  stepper.enableOutputs();
  stepper.setMaxSpeed(MAX_SPEED);
  stepper.setAcceleration(ACCELERATION);
  stepper.setCurrentPosition(pos);
}

void setup() {
  Serial.begin(9600);
  pinMode(T_SENSOR_PIN, INPUT);
  pinMode(HEATER_PIN, OUTPUT);
  analogWrite(HEATER_PIN, 0);
  init_stepper(stepper_e, E_ENABLE_PIN, 0);
  stepper_e.setSpeed(10); //Speed? 
}

bool flag = false;
  
void loop() {
  float temp, ur, Rth, Vo, val;
  String command;
  
  Vo = analogRead(T_SENSOR_PIN); //пересчёт данных с термистра в температуру по цельсию
  Rth = R/(1023.0/Vo - 1); // thermisor resistance
  Rth = Rth/NOMINAL_RTH;
  ur = log(Rth);
  ur = ur/coef;
  ur = ur + 1.0/(NOMINAL_T + 273.15); 
  temp = 1.0/ur - 273.15;
  
  if (Serial.available() > 0)
  {
    command = Serial.readString();
    if (command == "p\n")
    {
      stepper_e.setSpeed(20);
      flag = true;
    } 
      
    else if (command == "s\n")
    { 
      flag = false;
      stepper_e.stop();
      stepper_e.setCurrentPosition(0);
      analogWrite(HEATER_PIN, 0);
      stepper_e.moveTo(-5.0/ONESTEP);
      
      while (stepper_e.distanceToGo() != 0)
      {
        stepper_e.run();
      }
    }
  }
  if (flag == true)
  {
    if (temp < 210)
    {
      analogWrite(HEATER_PIN, 255);
      Serial.println(temp);
    }
    else if (temp > 225)
    {
      analogWrite(HEATER_PIN, 0);
    }
    if (temp < 225 && temp > 210)
    {
      stepper_e.runSpeed();
      val = -17*temp+3825;
      analogWrite(HEATER_PIN, val);
      Serial.println(temp);
    }  
  }
  else
  {
    Serial.println(temp);
    delay(1000);
  }
}
