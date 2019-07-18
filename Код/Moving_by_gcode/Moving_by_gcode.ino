#include <AccelStepper.h>
#include <SD.h>
#include "printer.h"
#include "gparser.h"
#include "heating.h"

//#define SS 53 // SS pin must be OUTPUT for SD.h

Carriages cars;
Position current_pos;
AccelStepper stepper_x(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper stepper_y(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN); 
AccelStepper stepper_z(AccelStepper::DRIVER, Z_STEP_PIN, Z_DIR_PIN); 
AccelStepper stepper_e(AccelStepper::DRIVER, E_STEP_PIN, E_DIR_PIN);
AccelStepper stepper_extr(AccelStepper::DRIVER, E1_STEP_PIN, E1_DIR_PIN);

#define CARS_POINTS_DISTANCE 300

void setup()
{
  Serial.begin(9600);
  SD.begin();
  //pinMode(SS, OUTPUT);
  init_stepper(stepper_x, X_ENABLE_PIN, -(CARS_POINTS_DISTANCE / 2) / ONE_STEP);
  init_stepper(stepper_y, Y_ENABLE_PIN, (CARS_POINTS_DISTANCE / 2) / ONE_STEP);
  init_stepper(stepper_z, Z_ENABLE_PIN, -(CARS_POINTS_DISTANCE / 2) / ONE_STEP); 
  init_stepper(stepper_e, E_ENABLE_PIN, (CARS_POINTS_DISTANCE / 2) / ONE_STEP);
  init_stepper(stepper_extr, E1_ENABLE_PIN, 0);

  pinMode(T_SENSOR_PIN, INPUT);
  pinMode(HEATER_PIN, OUTPUT);
  analogWrite(HEATER_PIN, 0);
}

File gcode;
String file_name = "jent.txt";

void loop()
{
  current_pos = {0,0,450};
  go(cars, current_pos);
  
  gcode = SD.open(file_name);
  if (gcode)
  {
    Serial.println("File is opened");
    heating();
    while (gcode.available())
    {
      String str = gcode.readStringUntil('\n'); // \n - в Linux, \r\n - в Windows
      do_command(str);
    }
    gcode.close();
  }
  else
  {
    Serial.println("Cannot open file");
  }
  Serial.println("Printing in completed");
  delay(100000);
}
