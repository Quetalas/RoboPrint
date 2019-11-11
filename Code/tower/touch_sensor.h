#pragma once

#define TOUCH_SIGNAL_PIN 19  
#define TOUCH_UP_PIN 42
#define TOUCH_DOWN_PIN 40

#define TOUCH_MOVE_UP_TIME 3000
#define TOUCH_MOVE_DOWN_TIME 5000 //80000
class TouchProbe {
  public:
    TouchProbe();
      void stopMotor();
      void up();
      void down();
      bool isTouched();
  
  private:
    bool isCircuitClosed();
    bool up_position;
};

extern TouchProbe touchProbe;
