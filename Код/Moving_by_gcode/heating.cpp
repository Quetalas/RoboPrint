#include "heating.h"

bool do_print = false;

void extr_init()
{
	pinMode(T_SENSOR_PIN, INPUT);
  pinMode(HEATER_PIN, OUTPUT);
  analogWrite(HEATER_PIN, 0);
}

float get_temprature()
{
	float temp, ur, Rth, Vo;
	Vo = analogRead(T_SENSOR_PIN); //пересчёт данных с термистра в температуру по цельсию
	Rth = RESISTANCE/(1023.0/Vo - 1); // thermisor resistance
	Rth = Rth/NOMINAL_RTH;
	ur = log(Rth);
	ur = ur/coef;
	ur = ur + 1.0/(NOMINAL_T + 273.15); 
	temp = 1.0/ur - 273.15;
	Serial.print("Current tmp ");
	Serial.println(temp);
	return temp;
}

void run_print()
{
	int pwm;
	float temp = get_temprature();

    if (temp < 210)
	{
	  analogWrite(HEATER_PIN, 255);
	  Serial.println(temp);
	}
	else if (temp > 225)
	{
	  analogWrite(HEATER_PIN, 0);
	}
	if (temp < 225 && temp > 210)
	{
	  stepper_extr.runSpeed();
	  pwm = -17.0*temp+3825;
	  analogWrite(HEATER_PIN, pwm);
	  Serial.println(temp);
	}  
}

void heating()
{
	float temp = get_temprature();
	while (temp < 220)
	{
		analogWrite(HEATER_PIN, 255);
		Serial.print("temp ");
		Serial.println(temp);
		temp = get_temprature();
	}
	analogWrite(HEATER_PIN, 0);
}	
