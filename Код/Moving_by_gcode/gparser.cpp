#include "gparser.h"

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
    //Serial.println("G_0_1");
    //Serial.println("***");
    //for(int i =0; i<6; i++)
    //{
    //  Serial.println(params[i]);
    //}
    //Serial.println("---");
    move_to(params[0], params[1], params[2], params[3]);
  /*
   * -------------------------------------------------------------
   */
}
