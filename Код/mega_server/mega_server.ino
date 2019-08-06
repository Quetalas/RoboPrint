
enum commands {G0, G1, ECHO};

void setup() {
  
}

#define BAUD_RATE 250000
#define DIVIDER ','

class Computer {
  
  private:
    String data_pack;  // Считанный с COM-порта пакет данных ввиде "data1,data2,...,datan" - строка
    uint16_t last_position;
    
  public:
    explicit Computer() : data_pack(""), last_position(0) {
      Serial.setTimeout(300000);  // 5 минут
      Serial.begin(BAUD_RATE);
    }
    
    String getDataPack() const { return data_pack; }
    
    int packLen() {
     /*
      *   "" - нет данных в пакете
      *   "a" - одна еденицы данных
      *   "," - одна еденица ("" после запятой)
      *   ",a" - одна еденица (a)
      *   "a," - две еденицы (a и "")
      *   "1,2" - две
      *   ",," - две : ",(1),(2)"
      *   "a,b,c" - три
      */
      if(data_pack.length() < 2) { return data_pack.length(); }
      
      uint16_t counter;
      (data_pack[0] == ",") ? counter = 0 : counter = 1;
      for(int i = 0; i < data_pack.length(); ++i) {
        (data_pack[i] == ",") ? ++counter : counter;
      }
      return counter;
    }
    
    String getNextData() {
      if (data_pack.length() < 2) { // Если один символ или пусто
        if (data_pack == "") { return data_pack; }
        if (data_pack == DIVIDER) { return ""; }
        return data_pack;
      }
      if (last_position == (data_pack.length() - 1)) {  // на случай если есть запятая в конце data_pack
        last_position = 0;
        return "";
      }
      
      String res;
      uint16_t divider_index = data_pack.indexOf(DIVIDER, last_position + 1);
      if (divider_index != -1) {
        res = data_pack.substring(last_position + 1, divider_index);
        last_position = divider_index;
      }
      else {
        res = data_pack.substring(last_position + 1); // до конца строки
        last_position = 0;
      }
      return res;
    }
    
    bool read_pack() {
      data_pack = "";
      last_position = 0;
      String next_byte = "";
      if (Serial.available() && Serial.find('{')) {
        while(true) {
          if(Serial.available()) {
            next_byte = Serial.read();
            (next_byte != "}") ? data_pack += next_byte : data_pack;
          }
        }
        return true;
      }
      else { data_pack = "";}
      return false;
    }
    
    void send_pack(const String& pack) const {
      if(Serial.availableForWrite() >= (pack.length() + 2)) {
        Serial.print("{" + pack + "}");
      }
    }
};

Computer comp();

void loop() {
//  comp.read_datas();
//  int cmd = comp.get_next_data().toInt(); // command
//  if (cmd != "")
//  {
//    switch(cmd)
//    {
//      case G0:
//          break;
//      case G1:
//         break;
//      case ECHO:
//         break;
//      default:
//         break;
//    }
//  }
}
