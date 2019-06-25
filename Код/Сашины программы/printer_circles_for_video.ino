#include <AccelStepper.h>

// Определение переменных в которые записываются команды из порта
int k = 1;
int x1 = 10000;
int x2 = 10000;
double xstep = 0.0595;  //при 1000 в х3 дает 1100мм проезда. дорегулировать  
double ystep =0.9546;   // дорегулировать  
//float xstep =0.0041137805;
// drobl = 5400;   за столько шагов колесо поворачивает на угол 2 pi (устарело)


int X,Y,Z,E;

// флаг

boolean flag = 1;

// UNO

#define TABLE_LENGHT 1800          //Lx
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

#define Acceleration 100000
#define MaxSpeed 25000
#define Speed 5000

void setup() {
Serial.begin(9600);

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

}

int tackti(int L){  // возвращает количество тактов для проезда расстояния L
  int vos = round(L/xstep);
  return vos;
}

void forvard(int l){//вперед ( +х)
    X = l;
    Y = -l;
    Z = l;
    E = -l;
}

void back(int l){ //назад ( -х)
    X = -l;
    Y = l;
    Z = -l;
    E = l;
}

void left(int l){ //вверх ( +у)
    X = -l;
    Y = -l;
    Z = -l;
    E = -l;
}

void right(int l){  // вниз ( -у)
    X = l;
    Y = l;
    Z = l;
    E = l;
}

void loop()
{
int x3 = round(1000/xstep); //отладка


// заготовка под разые длины проезда


int biasy = tackti(TABLE_WIDTH);   // полное смещение основания по оси х
int biasx = tackti(TABLE_LENGHT);  // полное смещение основания по оси у
int movingx = x3;                   // просто движение на заданное число оборотов
int movingy = x2/10; 
int del= 45000;  //задержка

//заменить на свич, кейс после определения профиля детали.
 if (flag == 1 ) {   
  if (k  == 1 || k==2 || k==3  ) {   
    
    forvard(movingx);
  }
  if (k == 4){   
    right(movingy);
    
  }
    if (k  == 5 || k==6 || k==7 ){  

    back(movingx);
  }
    if (k == 8){  
      
    left(movingy);
  }
    if (k >= 9 ){     // остановка
   flag = 0;
  }

  stepperx.move(X); //заднее правое
  steppery.move(Y); //переднее правое минус чтобы вперед
  stepperz.move(Z); // переднее левое
  steppere.move(E); // заднее левое минус чтобы вперед
  while ( (stepperx.distanceToGo() != 0) || (steppery.distanceToGo() != 0) || (stepperz.distanceToGo() != 0) || (steppere.distanceToGo() != 0) ) {

  stepperx.run();
  steppery.run();
  stepperz.run();
  steppere.run();
}
   k = k+1;

delay(del);
  
 }

    
    
}
