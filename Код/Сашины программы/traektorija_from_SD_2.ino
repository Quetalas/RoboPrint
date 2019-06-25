#include <AccelStepper.h>
#include <SPI.h>
#include <SD.h>

File myFile;

// Определение переменных в которые записываются команды из порта
float X,Y,Z;  // будующие координаты экструдера в заданной СК
float Xlast,Ylast,Zlast;  //координаты предыдущей/текущей точки траектории движения экструдера в заданной СК 
int car1,car2,car3,car4;

#define validway 50  //допустимое расстояние между двумя точками траектории

#define TOWER_HEIGHT 405           //R
#define TABLE_LENGHT 1000          //Lx
#define PLATFORM_PARAMETR 70       //r
#define EXTRUDER_LIFT 70           //h
#define TABLE_WIDTH 230            //Ly

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
stepperx.setCurrentPosition(0); 
steppery.setCurrentPosition(170/onestep); 
stepperz.setCurrentPosition(-170/onestep); 
steppere.setCurrentPosition(0);
  
 }


void func1(float X, float Y, float Z ) {       //пересчитываем координаты следующего положения экструдера в координаты шаговиков ножек.
  
  car1 = round((X - PLATFORM_PARAMETR/sqrt(2) - sqrt(pow(TOWER_HEIGHT,2)-pow(Y-PLATFORM_PARAMETR/sqrt(2),2)-pow(Z-EXTRUDER_LIFT,2)))/onestep);
  car2 = round((X + PLATFORM_PARAMETR/sqrt(2) + sqrt(pow(TOWER_HEIGHT,2)-pow(Y-PLATFORM_PARAMETR/sqrt(2),2)-pow(Z-EXTRUDER_LIFT,2)))/onestep);
  car3 = round((X - PLATFORM_PARAMETR/sqrt(2) - sqrt(pow(TOWER_HEIGHT,2)-pow(Y+PLATFORM_PARAMETR/sqrt(2)-TABLE_WIDTH,2)-pow(Z-EXTRUDER_LIFT,2)))/onestep);
  car4 = round((X + PLATFORM_PARAMETR/sqrt(2) + sqrt(pow(TOWER_HEIGHT,2)-pow(Y+PLATFORM_PARAMETR/sqrt(2)-TABLE_WIDTH,2)-pow(Z-EXTRUDER_LIFT,2)))/onestep);

}
  

void funccompare(float X, float Y, float Z, float Xlast, float Ylast, float Zlast ) {       
  
  //проверяем допустимое расстояние между двумя точками траектории и при необходмости вводим прромежуточные точки траектории.
    
  while (X!=Xlast && Y!=Ylast && Z!=Zlast){
  if (  sqrt(pow(X-Xlast,2)+pow(Y-Ylast,2)+pow(Z-Zlast,2)) <= validway  )   
    
    funcmove(X, Y, Z); 
  
  if (  sqrt(pow(X-Xlast,2)+pow(Y-Ylast,2)+pow(Z-Zlast,2)) > validway ){

    if( abs(Z-Zlast) > 10 ) 
    Zlast+=(Z-Zlast)/2;
    else 
    Zlast=Z;
    
    if ( abs(Y-Ylast) < 0.625*validway)
      Ylast=Y;
    else {
      int i = 1;     
      while ( abs(Y-Ylast)/i >= 0.625*validway  ){
        i++;
      }
      Ylast+=(Y-Ylast)/i;
    }

    if ( abs(X-Xlast) < 0.625*validway)
      Xlast=X;
    else {    
      int i = 1;  
      while ( abs(X-Xlast)/i >= 0.625*validway  ){
        i++;
      }
      Xlast+=(X-Xlast)/i;
    }
       
    funcmove(Xlast, Ylast, Zlast);
    }
  }
}

   void funcmove (float X, float Y, float Z){  //передвигаемся к следующей точке траектории
        
        func1(X,Y,Z);
        
        stepperx.moveTo(-car1);
        steppery.moveTo(car2);
        stepperz.moveTo(-car4);
        steppere.moveTo(car3);

}


void loop()  {
 
  myFile = SD.open("test.txt");
  if (myFile) {
    
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      X = float(myFile.parseInt())/1000;
      Y = float(myFile.parseInt())/1000;
      Z = float(myFile.parseInt())/1000;
      
    funccompare( X, Y, Z, Xlast, Ylast, Zlast )

    // обновляем координаты текущего положения   
      Xlast = X; 
      Ylast = Y;
      Zlast = Z;


//        Serial.print(X);
//        Serial.print(" ");
//        Serial.print(Y);
//        Serial.print(" ");
//        Serial.print(Z);
//        Serial.print(" ");
        Serial.print(car1);
        Serial.print(" ");
        Serial.print(car2);
        Serial.print(" ");
        Serial.print(car3);
        Serial.print(" ");
        Serial.println(car4);
       
        
        while ( (stepperx.distanceToGo() != 0) || (steppery.distanceToGo() != 0) || (stepperz.distanceToGo() != 0) || (steppere.distanceToGo() != 0) ) {

        stepperx.run();
        steppery.run();
        stepperz.run();
        steppere.run();
      }
      
    }
    // close the file:
    myFile.close();
  }

}
