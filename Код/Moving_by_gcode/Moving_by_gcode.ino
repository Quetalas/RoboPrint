#include <AccelStepper.h>
#include <SD.h>

//#define SS 53 // SS pin must be OUTPUT for SD.h

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

#define ACCELERATION 10000000
#define MAX_SPEED 1000

#define ONE_STEP  0.1 // в одном шаге 0.1мм

#define R 405  //мм, длина ножки
long R_2 = long(R)*long(R);

#define L 1000 //мм, длина принтера (меньше чем на самом деле)

#define r 70   //мм, расстояние между ножками (на верхней платформе)
float r_sqrt_2 = r/sqrt(2);

#define T_WEIGHT_HALF 115 // мм половина ширины принтера

#define EXT_HEIGHT 70 // мм, высота головки (над верхней платформой)

struct Carriages {
  float car_x, car_y, car_z, car_e; // абсолютные координаты кареток в мм
};

struct Position {
  float X, Y, Z;  // абсолютные координаты сопла в мм   
};


int get_index(char& c)
{
  switch(c)
  {
    case 'X':
      return 0;
    case 'Y':
      return 1;
    case 'Z':
      return 2;
    case 'E':
      return 3;
    case 'F':
      return 4;
    case 'S':
      return 5;
   }
}

Carriages cars;
Position current_pos;
AccelStepper stepper_x(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper stepper_y(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN); 
AccelStepper stepper_z(AccelStepper::DRIVER, Z_STEP_PIN, Z_DIR_PIN); 
AccelStepper stepper_e(AccelStepper::DRIVER, E_STEP_PIN, E_DIR_PIN);

#define STEP_X 0.1

void move_to(float& X, float& Y, float& Z)

{ float a = X - current_pos.X;
  if (fabs(a)>0.01)
  {
    int num_steps = (X - current_pos.X)/STEP_X;
    float STEP_Y = STEP_X*(Y - current_pos.Y)/(X - current_pos.X);
    for (int i = abs(num_steps); i > 0; i-- )
    {
      if(num_steps > 0)
      {
        current_pos.X += STEP_X; 
        current_pos.Y += STEP_Y;
        
      }
      else 
      {
       current_pos.X -= STEP_X; 
       current_pos.Y -= STEP_Y; 
      }
      set_new_pos(cars, current_pos);
      stepper_x.moveTo(round(-cars.car_x / ONE_STEP));  //int vs round
      stepper_y.moveTo(round(cars.car_y / ONE_STEP));
      stepper_z.moveTo(round(-cars.car_z / ONE_STEP)); 
      stepper_e.moveTo(round(cars.car_e / ONE_STEP));
      while ( (stepper_x.distanceToGo() != 0) || (stepper_y.distanceToGo() != 0) || (stepper_z.distanceToGo() != 0) || (stepper_e.distanceToGo() != 0) )
      {
        stepper_x.run();
        stepper_y.run();
        stepper_z.run();
        stepper_e.run();
      }
    }
  }
  else
  {
    current_pos.Y = Y;
    set_new_pos(cars, current_pos);
    stepper_x.moveTo(round(-cars.car_x / ONE_STEP));  //int vs round
    stepper_y.moveTo(round(cars.car_y / ONE_STEP));
    stepper_z.moveTo(round(-cars.car_z / ONE_STEP)); 
    stepper_e.moveTo(round(cars.car_e / ONE_STEP));
    while ( (stepper_x.distanceToGo() != 0) || (stepper_y.distanceToGo() != 0) || (stepper_z.distanceToGo() != 0) || (stepper_e.distanceToGo() != 0) )
    {
      stepper_x.run();
      stepper_y.run();
      stepper_z.run();
      stepper_e.run();
    }
  }
  
}

void do_G0(String& str)
{
  unsigned int space_index;
  float params[6] = {0,0,0,0,0,0};
  for(int i = 0; i<6; i++)
  {
    space_index = str.indexOf(" ");
    if (space_index != -1)
    {
      params[get_index(str[0])] = str.substring(1, space_index).toFloat();
      str.remove(0, space_index + 1);
    }
    else if (str.length() > 0)
    {
      params[get_index(str[0])] = str.substring(1, space_index).toFloat();
      str.remove(0, str.length());
    }
  }
  /*
   * ---------------Вставьте сюда свой код для G0----------------
   * params содержит X Y Z E F S
   */
    Serial.println("G_0_1");
    Serial.println("***");
    for(int i =0; i<6; i++)
    {
      Serial.println(params[i]);
    }
    Serial.println("---");
    move_to(params[0], params[1], params[2]);
  /*
   * -------------------------------------------------------------
   */
}

void do_command(String& str)
{
  String command;
  unsigned int space_index = str.indexOf(" ");
  if (space_index != -1)
  {
    command = str.substring(0,space_index);
    str.remove(0, space_index + 1); //command params
    if (command == "G0")
    {
      do_G0(str);
    }
    
    else if (command == "G1")
    {
      do_G0(str);
    }
  }
}

void init_stepper(AccelStepper& stepper, const uint8_t& pin, const int& pos)
{
  stepper.setEnablePin(pin); 
  stepper.setPinsInverted(false, false, true); // почему так?
  stepper.enableOutputs();
  stepper.setMaxSpeed(MAX_SPEED);
  stepper.setAcceleration(ACCELERATION);
  stepper.setCurrentPosition(pos);
}

void set_new_pos(Carriages& cars, const Position& new_pos)
{
  float common_part = R_2 - (new_pos.Z - EXT_HEIGHT)*(new_pos.Z - EXT_HEIGHT);
  float dY_0 = -T_WEIGHT_HALF - (new_pos.Y-r_sqrt_2);
  float dY_1 = T_WEIGHT_HALF - (new_pos.Y+r_sqrt_2);

  float under_root_0 = common_part - (dY_0)*(dY_0);
  float under_root_1 = common_part - (dY_1)*(dY_1);
  //Serial.println(under_root_0,3);
  //Serial.println(under_root_1,3);
  if (under_root_0 >= 0 && under_root_1 >= 0)
  {
    float delta_0 = r_sqrt_2 + sqrt(under_root_0);
    float delta_l = r_sqrt_2 + sqrt(under_root_1); // в миллиметрах
    
    cars.car_x = new_pos.X - delta_0;
    cars.car_y = new_pos.X + delta_0;
    cars.car_z = new_pos.X + delta_l;
    cars.car_e = new_pos.X - delta_l;
    //Serial.println(cars.car_x,3);
    //Serial.println(cars.car_y,3);
    //Serial.println(cars.car_z,3);
    //Serial.println(cars.car_e,3);
  }
  else
  {
    Serial.println("Wrong Destination");
  }
}

void setup()
{
  Serial.begin(9600);
  SD.begin();
  //pinMode(SS, OUTPUT);
  init_stepper(stepper_x, X_ENABLE_PIN, 0);
  init_stepper(stepper_y, Y_ENABLE_PIN, 140/ONE_STEP);
  init_stepper(stepper_z, Z_ENABLE_PIN, -140/ONE_STEP); 
  init_stepper(stepper_e, E_ENABLE_PIN, 0);
}

File gcode;
String file_name = "gcode.txt";

void loop()
{ current_pos = {0,0,430};
  set_new_pos(cars, current_pos);
  stepper_x.moveTo(round(-cars.car_x / ONE_STEP));  //int vs round
  stepper_y.moveTo(round(cars.car_y / ONE_STEP));
  stepper_z.moveTo(round(-cars.car_z / ONE_STEP)); 
  stepper_e.moveTo(round(cars.car_e / ONE_STEP));
  while ( (stepper_x.distanceToGo() != 0) || (stepper_y.distanceToGo() != 0) || (stepper_z.distanceToGo() != 0) || (stepper_e.distanceToGo() != 0) )
  {
    stepper_x.run();
    stepper_y.run();
    stepper_z.run();
    stepper_e.run();
  }
  gcode = SD.open(file_name);
  if (gcode)
  {
    Serial.println("File is opened");
    int i=0;
    while (gcode.available())
    {
      String str = gcode.readStringUntil('\n'); // \n - в Linux, \r\n - в Windows
      i = i+1;
      Serial.println(i);
      Serial.println(str);
      do_command(str);
    }
    gcode.close();
  }
  else
  {
    Serial.println("Cannot open file");
  }
  delay(1000);
}
