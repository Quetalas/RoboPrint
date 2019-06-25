#include <AccelStepper.h>
#include <SPI.h>
#include <SD.h>
#include <string.h>

File myFile;
File myFile1;
File myFile2;
File myFile4;

// Определение переменных в которые записываются команды из порта
float X,Y,Z; // координаты экструдера в заданной СК
int car1,car2,car3,car4;
int circl = 0;

#define TOWER_HEIGHT 405
#define TABLE_LENGHT 1000
#define PLATFORM_PARAMETR 70
#define EXTRUDER_LIFT 70
#define TABLE_WIDTH 230

#define Acceleration 10000000
#define MaxSpeed 1000000
#define Speed 250
#define onestep  0.1 //  шагов в одном мм

// объявление пинов

// RAMPS

#define X_STEP_PIN 54
#define X_DIR_PIN 55
#define X_ENABLE_PIN 38

#define Y_STEP_PIN 60
#define Y_DIR_PIN 61
#define Y_ENABLE_PIN 56

#define Z_STEP_PIN 46
#define Z_DIR_PIN 48
#define Z_ENABLE_PIN 62

#define E_STEP_PIN 26
#define E_DIR_PIN 28
#define E_ENABLE_PIN 24

// объявление движков
AccelStepper stepperx(1, X_STEP_PIN, X_DIR_PIN); // 1 = Driver
AccelStepper steppery(1, Y_STEP_PIN, Y_DIR_PIN); // 1 = Driver
AccelStepper stepperz(1, Z_STEP_PIN, Z_DIR_PIN); // 1 = Driver
AccelStepper steppere(1, E_STEP_PIN, E_DIR_PIN); // 1 = Driver

void func1(float X, float Y, float Z ) {
  car1 = round((X - PLATFORM_PARAMETR/sqrt(2) - sqrt(pow(TOWER_HEIGHT,2)-pow(Y-PLATFORM_PARAMETR/sqrt(2),2)-pow(Z-EXTRUDER_LIFT,2)))/onestep);
  car2 = round((X + PLATFORM_PARAMETR/sqrt(2) + sqrt(pow(TOWER_HEIGHT,2)-pow(Y-PLATFORM_PARAMETR/sqrt(2),2)-pow(Z-EXTRUDER_LIFT,2)))/onestep);
  car3 = round((X - PLATFORM_PARAMETR/sqrt(2) - sqrt(pow(TOWER_HEIGHT,2)-pow(Y+PLATFORM_PARAMETR/sqrt(2)-TABLE_WIDTH,2)-pow(Z-EXTRUDER_LIFT,2)))/onestep);
  car4 = round((X + PLATFORM_PARAMETR/sqrt(2) + sqrt(pow(TOWER_HEIGHT,2)-pow(Y+PLATFORM_PARAMETR/sqrt(2)-TABLE_WIDTH,2)-pow(Z-EXTRUDER_LIFT,2)))/onestep);

}
  
 

void setup() {
Serial.begin(9600);
  if (!SD.begin(53)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
// траектория



// установка пина на разрешение управления движком

stepperx.setEnablePin(X_ENABLE_PIN); 
stepperx.setPinsInverted(false, false, true); //invert logic of enable pin
stepperx.enableOutputs();

steppery.setEnablePin(Y_ENABLE_PIN); 
steppery.setPinsInverted(false, false, true); //invert logic of enable pin
steppery.enableOutputs();

stepperz.setEnablePin(Z_ENABLE_PIN); 
stepperz.setPinsInverted(false, false, true); //invert logic of enable pin
stepperz.enableOutputs();

steppere.setEnablePin(E_ENABLE_PIN); 
steppere.setPinsInverted(false, false, true); //invert logic of enable pin
steppere.enableOutputs();

// установка скорости, максимальной скорости, ускорения

stepperx.setMaxSpeed(MaxSpeed);
steppery.setMaxSpeed(MaxSpeed);
stepperz.setMaxSpeed(MaxSpeed);
steppere.setMaxSpeed(MaxSpeed);

stepperx.setAcceleration(Acceleration);
steppery.setAcceleration(Acceleration);
stepperz.setAcceleration(Acceleration);
steppere.setAcceleration(Acceleration);


// при включении определяем нули всех шаговиков
//*
stepperx.setCurrentPosition(806); 
steppery.setCurrentPosition(3206); 
stepperz.setCurrentPosition(-3206); 
steppere.setCurrentPosition(-806);
// */


/*        //back
stepperx.setCurrentPosition(-3694); 
steppery.setCurrentPosition(7706); 
stepperz.setCurrentPosition(-7706); 
steppere.setCurrentPosition(3694);
// */       
 }


void loop()  {
 
int del = 20000;
  if (circl == 0){
    delay(5000);
    circl++;
  }
 
if (circl == 1 || circl == 2 || circl == 3)
 {  
   myFile = SD.open("test.txt");
  if (myFile) {
    while (myFile.available()) {
      
      X = float(myFile.parseInt())/1000;
      Y = float(myFile.parseInt())/1000;
      Z = float(myFile.parseInt())/1000;
      if ( X!= 0 && Y!=0 && Z!=0){
      func1(X,Y,Z);
      }  
        stepperx.moveTo(-car1);
        steppery.moveTo(car2);
        stepperz.moveTo(-car4);
        steppere.moveTo(car3);
       
        while ( (stepperx.distanceToGo() != 0) || (steppery.distanceToGo() != 0) || (stepperz.distanceToGo() != 0) || (steppere.distanceToGo() != 0) ) {

        stepperx.run();
        steppery.run();
        stepperz.run();
        steppere.run();
        delay (1);
      }
      
    }
     myFile.close(); 
     delay (del);
     circl++;
    }
 }

if (circl == 4)
 {  
   myFile1 = SD.open("testend.txt");
  if (myFile1) {
    while (myFile1.available()) {
      
      X = float(myFile1.parseInt())/1000;
      Y = float(myFile1.parseInt())/1000;
      Z = float(myFile1.parseInt())/1000;
      if ( X!= 0 && Y!=0 && Z!=0){
      func1(X,Y,Z);
      }  
        stepperx.moveTo(-car1);
        steppery.moveTo(car2);
        stepperz.moveTo(-car4);
        steppere.moveTo(car3);
       
        while ( (stepperx.distanceToGo() != 0) || (steppery.distanceToGo() != 0) || (stepperz.distanceToGo() != 0) || (steppere.distanceToGo() != 0) ) {

        stepperx.run();
        steppery.run();
        stepperz.run();
        steppere.run();
        delay (1);
      }      
    }
     myFile1.close(); 
     delay (del);
     circl++;
    }
 }

if (circl == 6 || circl == 5 || circl == 7)
 { 
   myFile2 = SD.open("testbackend.txt");
  if (myFile2) {
    while (myFile2.available()) 
      
      X = float(myFile2.parseInt())/1000;
      Y = float(myFile2.parseInt())/1000;
      Z = float(myFile2.parseInt())/1000;
     if ( X!= 0 && Y!=0 && Z!=0){
      func1(X,Y,Z);
      }  
        
        stepperx.moveTo(-car1);
        steppery.moveTo(car2);
        stepperz.moveTo(-car4);
        steppere.moveTo(car3);
 
        while ( (stepperx.distanceToGo() != 0) || (steppery.distanceToGo() != 0) || (stepperz.distanceToGo() != 0) || (steppere.distanceToGo() != 0) ) {

        stepperx.run();
        steppery.run();
        stepperz.run();
        steppere.run();
        delay (1);
      }     
    }  
    myFile2.close();
    delay (del);
    circl++;
    }
 }
 
   if (circl == 8)
 {
  myFile4 = SD.open("testback.txt");
  if (myFile4) {
    while (myFile4.available()) {
      
      X = float(myFile4.parseInt())/1000;
      Y = float(myFile4.parseInt())/1000;
      Z = float(myFile4.parseInt())/1000;
     if ( X!= 0 && Y!=0 && Z!=0){
      func1(X,Y,Z);
      }  
        
        stepperx.moveTo(-car1);
        steppery.moveTo(car2);
        stepperz.moveTo(-car4);
        steppere.moveTo(car3);
 
        while ( (stepperx.distanceToGo() != 0) || (steppery.distanceToGo() != 0) || (stepperz.distanceToGo() != 0) || (steppere.distanceToGo() != 0) ) {

        stepperx.run();
        steppery.run();
        stepperz.run();
        steppere.run();
        delay (1);
      }     
    }
     myFile4.close();
     delay (del);
     circl++;
    }
 }
if (circl >=8){
    delay(1000000);
    circl++;
  } 
}
