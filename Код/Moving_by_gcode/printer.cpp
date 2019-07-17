#include "printer.h"
#include "heating.h"

long R_2 = long(R)*long(R);
float r_sqrt_2 = r/sqrt(2);

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

void go(Carriages& cars, const Position& new_pos)
{
  set_new_pos(cars, new_pos);
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

#define STEP_X 0.1

void move_to(float& X, float& Y, float& Z, float& E)
{ 

  if (fabs(current_pos.Z - 430) < Z)
  {
  	current_pos.Z -= Z;
  	move_down();
  }
  
  if (E > 0)
  {
    stepper_extr.setSpeed(-20);
    do_print = true;
  }
  else
  {
    do_print = false;
    stepper_extr.stop(); 
    stepper_extr.setCurrentPosition(0);
    stepper_extr.moveTo(2.0/EXTR_STEP);
      
    while (stepper_extr.distanceToGo() != 0)
    {
      stepper_extr.run();
    }
  }


  float a = X - current_pos.X;
  if (fabs(a)>0.01)
  {
    int num_steps = (X - current_pos.X)/STEP_X;
    float STEP_Y = STEP_X*(Y - current_pos.Y)/(X - current_pos.X);
    for (int i = abs(num_steps); i > 0; i-- )
    {
      if (do_print)
      {
        run_print();
      }
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
      go(cars, current_pos);
    }
  }
  else
  {
    current_pos.Y = Y;
    go(cars, current_pos);
  }
}

void move_down()
{
  go(cars, current_pos);
}

