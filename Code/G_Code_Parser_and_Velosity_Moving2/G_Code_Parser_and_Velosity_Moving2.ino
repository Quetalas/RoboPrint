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

#define EXT_STEP_PIN 36
#define EXT_DIR_PIN 34
#define EXT_ENABLE_PIN 30

#define HEATER_PIN 10
#define T_SENSOR_PIN 13 // Analog pin
#define R 4700
#define NOMINAL_T 25 //номинальная температура, при которой Rth = номинальному сопротивлению термистра
#define coef 3950 //для термистора NTC
#define NOMINAL_RTH 100000 //thermisor EPCOS 100k is used
#define TEMP_WISH 228

#define ACCELERATION 10
#define MAX_SPEED 1500

#define ONE_STEP  0.1 // в одном шаге 0.1мм

#define TOWER_HEIGHT 405.0  //мм, длина ножки
long T_H_2 = long(TOWER_HEIGHT)*long(TOWER_HEIGHT);

#define LONG 1000 //мм, длина принтера (меньше чем на самом деле)

#define platform_parametr 70.0   //мм, расстояние между ножками (на верхней платформе)
float p_pr_sqrt_2 = platform_parametr/sqrt(2);

#define TABLE_WIDTH 230.0 // мм ширина принтера
long T_W_2 = long(TABLE_WIDTH)*long(TABLE_WIDTH);


#define EXTRUDER_HEIGHT 70.0 // мм, высота головки (над верхней платформой)

struct Carriages {
  float x, y, z, e; // абсолютные координаты кареток в мм
};

struct Position {
  float X, Y, Z;  // абсолютные координаты сопла в мм   
};

struct Velosity {
  float Vx, Vy;  // скорость сопла в мм/с
};

struct Cars_speed {
  float vx, vy, vz, ve;
};

struct Direction {
  float X, Y;
};

inline float kv(float x){
  return x*x;
}

float get_temp()
{
  float temp, ur, Rth, Vo;
  Vo = analogRead(T_SENSOR_PIN); //пересчёт данных с термистра в температуру по цельсию
  Rth = R/(1023.0/Vo - 1); // thermisor resistance
  Rth = Rth/NOMINAL_RTH;
  ur = log(Rth);
  ur = ur/coef;
  ur = ur + 1.0/(NOMINAL_T + 273.15); 
  temp = 1.0/ur - 273.15;
  return temp;
}

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

Carriages cars1, cars2;
Position head_pos1, head_pos2, head_pos_t1, head_pos_t2;
Velosity vel;
Cars_speed c_s;
Direction dr;

AccelStepper stepper_x(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper stepper_y(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN); 
AccelStepper stepper_z(AccelStepper::DRIVER, Z_STEP_PIN, Z_DIR_PIN);
AccelStepper stepper_e(AccelStepper::DRIVER, E_STEP_PIN, E_DIR_PIN);
AccelStepper stepper_ext(AccelStepper::DRIVER, EXT_STEP_PIN, EXT_DIR_PIN);

void init_stepper(AccelStepper& stepper, const uint8_t& pin, const int& pos, const bool& dirinv)
{ //dirinv true для x и z, false для y и e
  stepper.setEnablePin(pin); 
  stepper.setPinsInverted(dirinv, false, true); // почему так?
  stepper.enableOutputs();
  stepper.setMaxSpeed(MAX_SPEED);
  stepper.setAcceleration(ACCELERATION);
  stepper.setCurrentPosition(pos);
}





void set_car_pos(Carriages& cars, const Position& new_pos)
{
  float root1, root2;
  //Serial.print("Go to car_pos "); Serial.println(millis());
  root1 = sqrt(T_H_2-kv(new_pos.Y-p_pr_sqrt_2)-kv(new_pos.Z-EXTRUDER_HEIGHT));
  root2 = sqrt(T_H_2-kv(TABLE_WIDTH-new_pos.Y-p_pr_sqrt_2)-kv(new_pos.Z-EXTRUDER_HEIGHT));
  //Serial.print("root1 = "); Serial.println(root1);
  //Serial.print("root2 = "); Serial.println(root2);
  //Serial.println(millis());
  cars.x = new_pos.X-p_pr_sqrt_2-root1;
  cars.y = new_pos.X+p_pr_sqrt_2+root1;
  cars.z = new_pos.X+p_pr_sqrt_2+root2;
  cars.e = new_pos.X-p_pr_sqrt_2-root2;
  //Serial.print("End car_pos "); Serial.println(millis());
}

void set_head_pos(const Carriages& cars, Position& new_pos)    //устанавловием положение головки
{
  new_pos.X = (cars.x+cars.y)/2.0;
  new_pos.Y = ((kv(cars.y-cars.x)-kv(cars.z-cars.e))/4.0+p_pr_sqrt_2*(cars.x-cars.y+cars.z-cars.e)+2.0*p_pr_sqrt_2*TABLE_WIDTH-T_W_2)/(4.0*p_pr_sqrt_2-2.0*TABLE_WIDTH);
  new_pos.Z = EXTRUDER_HEIGHT+sqrt(T_H_2-kv((cars.y-cars.x)/2.0-p_pr_sqrt_2)-kv(new_pos.Y-p_pr_sqrt_2));
}
/*
void set_velosity(const Position& pos1, const Position& pos2, const float& V, Velosity& Vel)    //устанавливаем вектор скорости головки
{
  float root = sqrt(kv(pos2.X-pos1.X)+kv(pos2.Y-pos1.Y));
  Vel.Vx = V*(pos2.X-pos1.X)/root;
  Vel.Vy = V*(pos2.Y-pos1.Y)/root;
}
*/
/*
void set_cars_speed(const Position& pos1, const Velosity& Vel, Cars_speed& c_s)     //установка скорости движков
{
 float drob1, drob2;
 //Serial.print("Go to car_speed "); Serial.println(millis());
 drob1 = (Vel.Vy*(pos1.Y-p_pr_sqrt_2))/sqrt(T_H_2-kv(pos1.Y-p_pr_sqrt_2)-kv(pos1.Z-EXTRUDER_HEIGHT));
 drob2 = (Vel.Vy*(TABLE_WIDTH-pos1.Y-p_pr_sqrt_2))/sqrt(T_H_2-kv(TABLE_WIDTH-pos1.Y-p_pr_sqrt_2)-kv(pos1.Z-EXTRUDER_HEIGHT));
 //Serial.print("drob1 = "); Serial.println(drob1);
 //Serial.print("drob2 = "); Serial.println(drob2);
 //Serial.println(millis());
 c_s.vx = Vel.Vx+drob1;
 c_s.vy = Vel.Vx-drob1;
 c_s.vz = Vel.Vx+drob2;
 c_s.ve = Vel.Vx-drob2;
 //Serial.print("End car_speed "); Serial.println(millis());
}
*/
float z_start;

void setup()
{
  Serial.begin(250000);
  Serial.flush();
  SD.begin();
  //pinMode(SS, OUTPUT);
  init_stepper(stepper_x, X_ENABLE_PIN, 0, true);
  init_stepper(stepper_y, Y_ENABLE_PIN, round(437/ONE_STEP), false);
  init_stepper(stepper_z, Z_ENABLE_PIN, round(437/ONE_STEP), true);
  init_stepper(stepper_e, E_ENABLE_PIN, 0, false);
  init_stepper(stepper_ext, EXT_ENABLE_PIN, 0, true);

  cars1.x = stepper_x.currentPosition()*ONE_STEP;
  cars1.y = stepper_y.currentPosition()*ONE_STEP;
  cars1.z = stepper_z.currentPosition()*ONE_STEP;
  cars1.e = stepper_e.currentPosition()*ONE_STEP;
  
  set_head_pos(cars1, head_pos1);
  z_start = head_pos1.Z;

//  while(get_temp() < TEMP_WISH)
//  {
//    analogWrite(HEATER_PIN, 255);
//    Serial.println(get_temp());
//    delay(1000);
//  }
//  analogWrite(HEATER_PIN, 50);
}

File gcode;
String file_names[] = {"first.txt", "second.txt"};
int j;
//int k = 0;
float V = 40.0;
float E_past = 0;
float E_t1, E_t2, E_step;

void start_print(String file_name)
{
  gcode = SD.open(file_name);
  if (gcode)
  {
    Serial.println("File is opened");
    int i=0;
    while (gcode.available())
    {
      String str = gcode.readStringUntil('\n'); // \n - в Linux, \r\n - в Windows
      i = i+1;
      
      //if(i > 1100)
      {
        //Serial.println(i);
        //Serial.println(str);
        do_command(str);
      }
    }
    gcode.close();
    analogWrite(HEATER_PIN, 0);
  }
  else
  {
    Serial.println("Cannot open file");
  }
}

String message;
int num_print = 0;

unsigned long current_time;

void loop(){ 
  if (Serial.available()){    
    message = Serial.readStringUntil('.');
    Serial.print("Data accepted ");
    Serial.println(message);
    Serial.flush();
    if (message == "ON")
    {
      current_time = millis();
      while (millis() - current_time < 2000)
      {
        Serial.println("START_PRINT");
      }
      //start_print(file_names[num_print]);
      current_time = millis();
      while (millis() - current_time < 3000)
      {
        Serial.println("END_PRINT");
      }
      num_print += 1;
    }
    
    
  }
  else if(Serial.availableForWrite()) {
    Serial.println("READY");
    Serial.flush();
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
    /*Serial.println("G_0_1");
    Serial.println("***");
    for(int i =0; i<6; i++)
    {
      Serial.println(params[i]);
    }
    Serial.println("---");*/
    
      move_to(params[0], params[1], params[2], params[4], params[3]);
  /*
   * -------------------------------------------------------------
   */
}

 //модуль скорости головки

void move_to(float x_to_go, float y_to_go, float z_to_go, float F, float E){

    float time_for_step = 0.5;
    //E = E*2;
    
    if(z_to_go != 0)
    {
      head_pos2 = head_pos1;
      head_pos2.Z = z_start - z_to_go;
      set_car_pos(cars1, head_pos1);
      set_car_pos(cars2, head_pos2);
      
      stepper_x.moveTo(round(cars2.x / ONE_STEP));  //int vs round
      stepper_y.moveTo(round(cars2.y / ONE_STEP));
      stepper_z.moveTo(round(cars2.z / ONE_STEP)); 
      stepper_e.moveTo(round(cars2.e / ONE_STEP));
      
      c_s.vx = (cars2.x-cars1.x)/time_for_step;
      c_s.vy = (cars2.y-cars1.y)/time_for_step;
      c_s.vz = (cars2.z-cars1.z)/time_for_step;
      c_s.ve = (cars2.e-cars1.e)/time_for_step;

      stepper_x.setSpeed(fabs(c_s.vx/ONE_STEP));
      stepper_y.setSpeed(fabs(c_s.vy/ONE_STEP));
      stepper_z.setSpeed(fabs(c_s.vz/ONE_STEP));
      stepper_e.setSpeed(fabs(c_s.ve/ONE_STEP));

      while( (stepper_x.distanceToGo() != 0) || (stepper_y.distanceToGo() != 0) || (stepper_z.distanceToGo() != 0) || (stepper_e.distanceToGo() != 0) )
      {
        stepper_x.runSpeedToPosition();
        stepper_y.runSpeedToPosition();
        stepper_z.runSpeedToPosition();
        stepper_e.runSpeedToPosition();
      }
      head_pos1 = head_pos2;
      //Serial.println("                                                    End of if");
    }
    
    if(F != 0) V = F/60.0/4.0;
    if(x_to_go != 0) head_pos2.X = x_to_go+200.0;
    if(y_to_go != 0) head_pos2.Y = y_to_go+115.0;
    head_pos2.Z = head_pos1.Z;
    //set_velosity(head_pos1, head_pos2, V, vel);
    
    float del_y = 10;  //желаемый шаг разбиения по y в мм
    int Num_del = round(fabs(head_pos2.Y-head_pos1.Y)/del_y);
    if(Num_del == 0) {Num_del++;}
    //число внутренних отрезков  /\    //
    float root = sqrt(kv(head_pos2.X-head_pos1.X)+kv(head_pos2.Y-head_pos1.Y));
    if(root == 0) return;
    float real_del = root/float(Num_del);   
    //реальный шаг разбиения по длине траектории  /\   //
    time_for_step = real_del/V;
    dr.X = real_del*(head_pos2.X-head_pos1.X)/root;
    dr.Y = real_del*(head_pos2.Y-head_pos1.Y)/root;
    // вектор  перемещения на одном промежуточном отрезке    /\  //
    float v_ext = 0.1;
    if(E != 0)
    {
      E_step = (E-E_past)/Num_del;
      v_ext = E_step/time_for_step;
      E_t1 = E_past;
      E_t2 = E_past;
      E_past = E;
    }
    
    head_pos_t1 = head_pos1;
    head_pos_t2 = head_pos1;
       
    for (j = 0; j < Num_del; j++)
    { 
      //k++;
      if(get_temp() < TEMP_WISH) analogWrite(HEATER_PIN, 255);
      else if(get_temp() > 230) analogWrite(HEATER_PIN, 0);
        else analogWrite(HEATER_PIN, 50);
      Serial.println(get_temp());
      
      head_pos_t2.X = head_pos_t1.X+dr.X;
      head_pos_t2.Y = head_pos_t1.Y+dr.Y;
      E_t2 = E_t1 + E_step;
      //Serial.print("Go to count ");Serial.print(k);Serial.print("     "); Serial.println(millis());
      
      set_car_pos(cars1, head_pos_t1);
      set_car_pos(cars2, head_pos_t2);
      
      c_s.vx = (cars2.x-cars1.x)/time_for_step;
      c_s.vy = (cars2.y-cars1.y)/time_for_step;
      c_s.vz = (cars2.z-cars1.z)/time_for_step;
      c_s.ve = (cars2.e-cars1.e)/time_for_step;
      //Serial.println(time_for_step);
      
      //Serial.print("End of count ");  Serial.println(millis());
      stepper_x.moveTo(round(cars2.x / ONE_STEP));  //int vs round
      stepper_y.moveTo(round(cars2.y / ONE_STEP));
      stepper_z.moveTo(round(cars2.z / ONE_STEP)); 
      stepper_e.moveTo(round(cars2.e / ONE_STEP));
      
      stepper_x.setSpeed(fabs(c_s.vx/ONE_STEP));
      stepper_y.setSpeed(fabs(c_s.vy/ONE_STEP));
      stepper_z.setSpeed(fabs(c_s.vz/ONE_STEP));
      stepper_e.setSpeed(fabs(c_s.ve/ONE_STEP));

      /*Serial.println(fabs(c_s.vx/ONE_STEP));
      Serial.println(fabs(c_s.vx/ONE_STEP));
      Serial.println(fabs(c_s.vx/ONE_STEP));
      Serial.println(fabs(c_s.vx/ONE_STEP));*/

      if(E != 0)
      {
        stepper_ext.moveTo(round(E_t2 / ONE_STEP));
        stepper_ext.setSpeed(fabs(v_ext/ONE_STEP));
      }
      //Serial.println("Go to move  ");  //Serial.println(millis());
      while( (stepper_x.distanceToGo() != 0) || (stepper_y.distanceToGo() != 0) || (stepper_z.distanceToGo() != 0) || (stepper_e.distanceToGo() != 0) )
          {
            stepper_x.runSpeedToPosition();
            stepper_y.runSpeedToPosition();
            stepper_z.runSpeedToPosition();
            stepper_e.runSpeedToPosition();
            stepper_ext.runSpeedToPosition();
          }
      head_pos_t1 = head_pos_t2;
      E_t1 = E_t2;
      //Serial.println("End of move ");  //Serial.println(millis());
    }       
  head_pos1 = head_pos2;
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
