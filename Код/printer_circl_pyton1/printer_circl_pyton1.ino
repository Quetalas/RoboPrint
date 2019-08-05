#include <AccelStepper.h>

// drobl = 5400;   за столько шагов колесо поворачивает на угол 2 pi 

#define TABLE_LENGHT 1000          //Lx
#define TABLE_WIDTH 230            //Ly

#define X_STEP_PIN 2
#define X_DIR_PIN 5
#define X_ENABLE_PIN 8

#define Y_STEP_PIN 3
#define Y_DIR_PIN 6
#define Y_ENABLE_PIN 8

#define Z_STEP_PIN 4
#define Z_DIR_PIN 7
#define Z_ENABLE_PIN 8

#define E_STEP_PIN 12
#define E_DIR_PIN 13
#define E_ENABLE_PIN 8


// объявление движков
AccelStepper stepperx(1, X_STEP_PIN, X_DIR_PIN); // 1 = Driver
AccelStepper steppery(1, Y_STEP_PIN, Y_DIR_PIN); // 1 = Driver
AccelStepper stepperz(1, Z_STEP_PIN, Z_DIR_PIN); // 1 = Driver
AccelStepper steppere(1, E_STEP_PIN, E_DIR_PIN); // 1 = Driver

#define ACCELERATION 10000
#define MAX_SPEED 1000

void init_stepper(AccelStepper& stepper, const uint8_t& pin, const int& pos, const bool& dirinv)
{ //dirinv false для x и z, true для y и e
  stepper.setEnablePin(pin); 
  stepper.setPinsInverted(dirinv, false, true); // почему так?
  stepper.enableOutputs();
  stepper.setMaxSpeed(MAX_SPEED);
  stepper.setAcceleration(ACCELERATION);
  stepper.setCurrentPosition(pos);
}

void setup() {
  Serial.begin(250000);
  Serial.flush();
  init_stepper(stepperx, X_ENABLE_PIN, 0, false);  // колесо заднее правое
  init_stepper(steppery, Y_ENABLE_PIN, 0, true);   // переднее правое
  init_stepper(stepperz, Z_ENABLE_PIN, 0, false);  //переднее левое
  init_stepper(steppere, E_ENABLE_PIN, 0, true);   //заднее левое

  Serial.println("Initialisation complete");
  Serial.flush();
}

void loop(){
  long X = 0;
  long Y = 0;
  long Z = 0;
  long E = 0;
  stepperx.setCurrentPosition(0);
  steppery.setCurrentPosition(0);
  stepperz.setCurrentPosition(0);
  steppere.setCurrentPosition(0);
  if (Serial.available()){    
    X = Serial.parseInt(); 
    Y = X;
    Z = X;
    E = X; 
    Serial.print("Data accepted ");
    Serial.println(X);
  
    stepperx.move(X); //заднее правое
    steppery.move(Y); //переднее правое 
    stepperz.move(Z); // переднее левое
    steppere.move(E); // заднее левое 
    while ( (stepperx.distanceToGo() != 0) || (steppery.distanceToGo() != 0) || (stepperz.distanceToGo() != 0) || (steppere.distanceToGo() != 0) ) {

      stepperx.run();
      steppery.run();
      stepperz.run();
      steppere.run();
    
    }
    Serial.println("End of move");
    while(Serial.available())
    {
      Serial.read();
    }
  }
  else if(Serial.availableForWrite()) {
    Serial.println("GET");
    Serial.flush();
  }
}

    
    
