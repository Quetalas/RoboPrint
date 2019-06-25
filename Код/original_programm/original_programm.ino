#include <AccelStepper.h>

// Определение переменных в которые записываются команды из порта
float X,Y,Z; // координаты экструдера в заданной СК
boolean flag = false; // флаг буфера порта
boolean stopflag = false; // флаг остановки, каретки едут в ноль

float R = 405; //мм, длина штанги
float L = 1000;//мм, длина принтера (меньше чем на самом деле)
float r = 70; //мм, расстояние между штангами
float l = 230;// мм ширина принтера

// переменные принимающие участие в расчете координат
float delta_0;
float delta_l;
float car1; // X - координаты кареток в заданной СК
float car2;
float car3;
float car4;


//в одном шаге двигателя 20 мкм
//float  onestep = 0.2; // в миллиметрах
float  onestep = 0.1; // в миллиметрах
// ноль (в шагах)

int X0 = 0;
int Y0 = 170/onestep;
int Z0 = -170/onestep;
int E0 = 0;

const int N = 419; // размер массива в который записывается траектория
float t[N];
float x_t[N];
float y_t[N];
float z_t[N];

int i;



// объявление пинов



// UNO

//#define X_STEP_PIN 2
//#define X_DIR_PIN 5
//#define X_ENABLE_PIN 8
//
//#define Y_STEP_PIN 3
//#define Y_DIR_PIN 6
//#define Y_ENABLE_PIN 8
//
//#define Z_STEP_PIN 4
//#define Z_DIR_PIN 7
//#define Z_ENABLE_PIN 8
//
//#define E_STEP_PIN 12
//#define E_DIR_PIN 13
//#define E_ENABLE_PIN 8

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

long Acceleration = 10000000;
long MaxSpeed = 1000;
long Speed = 250;
 

void setup() {
Serial.begin(9600);

// траектория

for (i=0;i<N;i++)
{ t[i] = i*0.015;

  x_t[i] = 300*cos(t[i])+500;
  y_t[i] = 130*sin(2*t[i])+115;
  z_t[i] = 80*cos(2*t[i])+380;

}

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
stepperx.setCurrentPosition(X0); 
steppery.setCurrentPosition(Y0); 
stepperz.setCurrentPosition(Z0); 
steppere.setCurrentPosition(E0);


// едем в начальную точку траектории
    X = 800;
    Y = 115;
    Z = 460;
    Serial.println(X);
    Serial.println(Y);
    Serial.println(Z);
    
    // расчет координат кареток
    delta_0 = sqrt(pow(R,2)-pow(Y-r/sqrt(2),2)-pow(Z-r,2));
    delta_l = sqrt(pow(R,2)-pow(Y+r/sqrt(2)-l,2)-pow(Z-r,2));
  
    car1 = X - r/sqrt(2) - delta_0;
    car2 = X + r/sqrt(2) + delta_0;
    car3 = X - r/sqrt(2) - delta_l;
    car4 = X + r/sqrt(2) + delta_l;
    
        stepperx.moveTo(round(-car1/onestep));
        steppery.moveTo(round(car2/onestep));
        stepperz.moveTo(round(-car4/onestep));
        steppere.moveTo(round(car3/onestep));
        Serial.println(round(-car1/onestep));
        Serial.println(round(car2/onestep));
        Serial.println(round(-car4/onestep));
        Serial.println(round(car3/onestep));
        while ( (stepperx.distanceToGo() != 0) || (steppery.distanceToGo() != 0) || (stepperz.distanceToGo() != 0) || (steppere.distanceToGo() != 0) ) {

        stepperx.run();
        steppery.run();
        stepperz.run();
        steppere.run();
      }



}


void loop()

       
{

     
x_t[0] = 800;
y_t[0] = 115;
z_t[0] = 460;
  
  
  
   for (i = 0; i<N; i++) {
    Serial.println(i);
    X = x_t[i];
    Y = y_t[i];
    Z = z_t[i];
    Serial.println(X);
    Serial.println(Y);
    Serial.println(Z);
    
    // расчет координат кареток
    delta_0 = sqrt(pow(R,2)-pow(Y-r/sqrt(2),2)-pow(Z-r,2));
    delta_l = sqrt(pow(R,2)-pow(Y+r/sqrt(2)-l,2)-pow(Z-r,2));
  
    car1 = X - r/sqrt(2) - delta_0;
    car2 = X + r/sqrt(2) + delta_0;
    car3 = X - r/sqrt(2) - delta_l;
    car4 = X + r/sqrt(2) + delta_l;
    
        stepperx.moveTo(round(-car1/onestep));
        steppery.moveTo(round(car2/onestep));
        stepperz.moveTo(round(-car4/onestep));
        steppere.moveTo(round(car3/onestep));
        Serial.println(round(-car1/onestep));
        Serial.println(round(car2/onestep));
        Serial.println(round(-car4/onestep));
        Serial.println(round(car3/onestep));
        while ( (stepperx.distanceToGo() != 0) || (steppery.distanceToGo() != 0) || (stepperz.distanceToGo() != 0) || (steppere.distanceToGo() != 0) ) {

        stepperx.run();
        steppery.run();
        stepperz.run();
        steppere.run();
      }
   }

    
    
    
}
