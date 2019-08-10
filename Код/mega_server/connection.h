#pragma once
#define BAUD_RATE 9600
#define DIVIDER ','

class Connection {
  
  private:
    static String data_pack;  // Считанный с COM-порта пакет данных ввиде "data1,data2,...,datan" - строка
    static uint16_t last_position;
    
  public:
    static void begin();
    
    static String getDataPack();
    
    static int packLen();
    
    static String getNextData();
    
    static bool read_pack();
    
    static void send_pack(const String& pack);
};
