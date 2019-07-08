#include <AccelStepper.h>
#include <SD.h>

#define E_STEP_PIN 26
#define E_DIR_PIN 28
#define E_ENABLE_PIN 24

#define ACCELERATION 10000000
#define MAX_SPEED 1000


struct Carriages {
  float car_x, car_y, car_z, car_e; // абсолютные координаты кареток в мм
};

struct Position {
  float X, Y, Z;  // абсолютные координаты сопла в мм   
};

Carriages cars;
Position head_pos;

AccelStepper stepper_e(AccelStepper::DRIVER, E_STEP_PIN, E_DIR_PIN);

void init_stepper(AccelStepper& stepper, const uint8_t& pin, const int& pos)
{
  stepper.setEnablePin(pin); 
  stepper.setPinsInverted(false, false, true); // почему так?
  stepper.enableOutputs();
  stepper.setMaxSpeed(MAX_SPEED);
  stepper.setAcceleration(ACCELERATION);
  stepper.setCurrentPosition(pos);
}

void setup() {
  Serial.begin(9600); 
  init_stepper(stepper_e, E_ENABLE_PIN, 0);
  stepper_e.setSpeed(-400);
}

void loop() {
  stepper_e.runSpeed();
}
