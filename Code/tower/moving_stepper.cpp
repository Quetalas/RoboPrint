#include <AccelStepper.h>
#include "moving_stepper.h"
#include "connection.h"

AccelStepper stepper_x(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper stepper_y(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN); 
AccelStepper stepper_z(AccelStepper::DRIVER, Z_STEP_PIN, Z_DIR_PIN);
AccelStepper stepper_e(AccelStepper::DRIVER, E_STEP_PIN, E_DIR_PIN);
AccelStepper stepper_ext(AccelStepper::DRIVER, EXT_STEP_PIN, EXT_DIR_PIN);

void init_stepper(AccelStepper& stepper, const uint8_t& pin, const int& pos, const bool& dirinv) { 
  //dirinv true для x и z, false для y и e
  stepper.setEnablePin(pin); 
  stepper.setPinsInverted(dirinv, false, true); 
  stepper.enableOutputs();
  stepper.setMaxSpeed(MAX_SPEED);
  stepper.setAcceleration(ACCELERATION);
  stepper.setCurrentPosition(pos);
}

void set_ext_pos(){
  stepper_ext.setCurrentPosition(Connection::getNextData().toFloat());
}

void moving_gcode() {
  stepper_x.moveTo(Connection::getNextData().toInt());  
  stepper_y.moveTo(Connection::getNextData().toInt());
  stepper_z.moveTo(Connection::getNextData().toInt()); 
  stepper_e.moveTo(Connection::getNextData().toInt());
  
  stepper_x.setSpeed(Connection::getNextData().toFloat());
  stepper_y.setSpeed(Connection::getNextData().toFloat());
  stepper_z.setSpeed(Connection::getNextData().toFloat());
  stepper_e.setSpeed(Connection::getNextData().toFloat());
  
  if (Connection::packLen() > 9) {
    stepper_ext.moveTo(Connection::getNextData().toInt());
  	stepper_ext.setSpeed(Connection::getNextData().toFloat());
  }
  
  while((stepper_x.distanceToGo() != 0) || 
        (stepper_y.distanceToGo() != 0) ||
        (stepper_z.distanceToGo() != 0) || 
        (stepper_e.distanceToGo() != 0) ||
        (stepper_ext.distanceToGo() != 0)
        ) {
          
    stepper_x.runSpeedToPosition();
    stepper_y.runSpeedToPosition();
    stepper_z.runSpeedToPosition();
    stepper_e.runSpeedToPosition();
    stepper_ext.runSpeedToPosition();
  }  
}

void moving_coordinats(const int x, const int y, const int z, const int e){
    stepper_x.moveTo(x);
    stepper_y.moveTo(y);
    stepper_z.moveTo(z);
    stepper_e.moveTo(e);
    
    stepper_x.setSpeed(500);
    stepper_y.setSpeed(500);
    stepper_z.setSpeed(500);
    stepper_e.setSpeed(500);    
    while((stepper_x.distanceToGo() != 0) || 
          (stepper_y.distanceToGo() != 0) ||
          (stepper_z.distanceToGo() != 0) || 
          (stepper_e.distanceToGo() != 0) ) {
            
      stepper_x.runSpeedToPosition();
      stepper_y.runSpeedToPosition();
      stepper_z.runSpeedToPosition();
      stepper_e.runSpeedToPosition();
    }   
}

void ends_searc(const uint8_t pin1, AccelStepper& stepper1, AccelStepper& stepper11, const uint8_t pin2, AccelStepper& stepper2, AccelStepper& stepper22, const uint8_t dir, const long coordinate){ 
  // отрицательную скорость можно подавать
  int v = dir*500;
  stepper1.setSpeed(v);
  stepper11.setSpeed(v);
  stepper2.setSpeed(v);
  stepper22.setSpeed(v);
   
  while( digitalRead(pin1) && digitalRead(pin2) ){
      stepper1.runSpeed();
      stepper11.runSpeed();
      stepper2.runSpeed();
      stepper22.runSpeed();
  }
   if ( digitalRead(pin1) ||  digitalRead(pin2) ){
         if ( digitalRead(pin1) == 0 &&  digitalRead(pin2) != 0 ){
          stepper1.setCurrentPosition(coordinate);
          stepper1.setSpeed(0);
          stepper11.setSpeed(0);
          stepper2.setSpeed(v);
          stepper22.setSpeed(-v);       
        }
        if ( digitalRead(pin2) == 0 &&  digitalRead(pin1) != 0 ){
          stepper2.setCurrentPosition(coordinate);
          stepper2.setSpeed(0);
          stepper22.setSpeed(0);
          stepper1.setSpeed(v);
          stepper11.setSpeed(-v);
        }
        while( digitalRead(pin1) != 0 || digitalRead(pin2) != 0 ){
            stepper1.runSpeed();
            stepper11.runSpeed();
            stepper2.runSpeed();
            stepper22.runSpeed();
        }
        stepper1.setSpeed(0);
        stepper11.setSpeed(0);
        stepper2.setSpeed(0);
        stepper22.setSpeed(0);
     }
     stepper1.setCurrentPosition(coordinate);
     stepper2.setCurrentPosition(coordinate);
}

void autohome_calibrtion() { 
  /*
     *  скорость в +, пока не сработает 1 концевик. 
     *  меняем сорости. 2 ноги стоят, 2 расходятся.
     *  на все скорость в -. 
     *  повторяем. переезд в координаты стартового положения 
     */
  int x_start = Connection::getNextData().toInt();
  int y_start = Connection::getNextData().toInt();
  int z_start = Connection::getNextData().toInt();
  int e_start = Connection::getNextData().toInt();
     
  ends_searc(Y_END, stepper_y, stepper_x, Z_END, stepper_z, stepper_e, 1, YZ_STEPPER_COORD);  // zy концевики 
  ends_searc(X_END, stepper_x, stepper_y, E_END, stepper_e, stepper_z, -1, XE_STEPPER_COORD); 
  moving_coordinats(x_start, y_start, z_start, e_start);  
}
