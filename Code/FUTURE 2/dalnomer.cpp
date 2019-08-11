#include <AccelStepper.h>
#include "dalnomer.h"
#include "connection.h"
#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;

void init_dalnomer()
{
  Wire.begin();
  sensor.init();
  sensor.setTimeout(SENSOR_TIMEOUT);
  sensor.startContinuous();
}

float measure_distance ()
{
  int count;
  float sum = 0;
  for(count=0; count<CON_MEASURE; count++)
  {
    sum += sensor.readRangeContinuousMillimeters();
  }
  sum = sum/count;
  //if (sensor.timeoutOccurred()) { 
  //Serial.println(" TIMEOUT"); }
  return sum;
}
