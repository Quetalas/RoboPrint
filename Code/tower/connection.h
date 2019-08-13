#pragma once
#define DIVIDER ','

class Connection {
  private:
    static String data_pack;  // Считанный с COM-порта пакет данных ввиде "data1,data2,...,datan" - строка
    static uint16_t last_position;
  
  public:
    static void begin(const unsigned long& baud_rate=115200);
    static String getDataPack();    
    static int packLen();
    static String getNextData();    
    static void listen();
    static void send_pack(const String& pack);
};
