#include "Arduino.h"
#include "touch_sensor.h"

TouchProbe::TouchProbe(){
  pinMode(TOUCH_SIGNAL_PIN, INPUT);
  pinMode(TOUCH_UP_PIN, OUTPUT);
  pinMode(TOUCH_DOWN_PIN, OUTPUT);
  stopMotor();
  up_position = false;
  };

  void TouchProbe::stopMotor(){
    digitalWrite(TOUCH_UP_PIN, LOW);
    digitalWrite(TOUCH_DOWN_PIN, LOW);
  };

  void TouchProbe::down(){ 
    while (isCircuitClosed() && up_position){
      digitalWrite(TOUCH_UP_PIN, LOW);
      digitalWrite(TOUCH_DOWN_PIN, HIGH);
    }
    delay(TOUCH_MOVE_DOWN_TIME);
    stopMotor();
    up_position = false;
  };

  void TouchProbe::up(){ 
    while (!isCircuitClosed() && !up_position){
      digitalWrite(TOUCH_UP_PIN, HIGH);
      digitalWrite(TOUCH_DOWN_PIN, LOW);
    }
    delay(TOUCH_MOVE_UP_TIME);
    stopMotor();
    up_position = true;
  };

  bool TouchProbe::isCircuitClosed(){ 
    if (digitalRead(TOUCH_SIGNAL_PIN))
      return 1;
    else 
      return 0;
  };

  bool TouchProbe::isTouched(){ 
    return !isCircuitClosed();  // касается - 1    
  };

TouchProbe touchProbe = TouchProbe();

    
