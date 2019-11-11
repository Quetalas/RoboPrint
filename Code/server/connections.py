import serial

Commands = {'G0': 0, 'G1': 1, 'echo': 2, 'get_temp': 3, 'M104': 4, 'M109': 5, 'G92': 7, 'G28': 8,
            'probe_up': 9, 'probe_down': 10, 'is_touched': 11, 'edge_search': 12, 'G29': 29}   # 6 пусто

"""
    G0 - move, extruder off;    // движение с выключенным экструдером;
    G1 - move, extruder on;    // движение с включенным экструдером;
    G28 - autohome calibration // 
    G29 - auto calibrate table;    // автокалибровка стола;
    G92 - set extruder position;    // устанавливает положение экструдера;
    M104 - set temperature;    // установка температуры нагрева;
    M105 - get heater temperature;    // возврат начения текущей температуры сопла;
    M109 - set temperature and heat from this temperature;    // нагрев сопла до заданной температуры;
    
"""

class Connection(serial.serialwin32.Serial):

    def __init__(self, port):

        self.BAUD_RATE = 115200
        super().__init__(port, self.BAUD_RATE)  # Блокирующее считывание нужно методу get

        self.DIVIDER = ','
        self.STARTING = bytes('{'.encode('ascii'))
        self.ENDING = bytes('}'.encode('ascii'))

        self.received_data = []

    def send(self, *args):

        if len(args) == 0:  # Незачем тратить время на отправку пустых данных
            return False

        self.write(self.STARTING)
        for i, d in enumerate(args):

            data_pack = str(d) + self.DIVIDER if i < (len(args) - 1) else str(d)
            while self.out_waiting > 10:
                pass
            self.write(bytes(data_pack.encode('ascii')))

        self.write(self.ENDING)

        return True

    def get(self):
        """
        Начинает считывание со стартового байта,
        читает до тех пор, пока не получит байт конца данных.
        Использует байт-разделитель, чтобы получить массив данных из строки.
        :return:
        """
        self.received_data.clear()

        self.read_until(self.STARTING)
        data = self.read_until(self.ENDING)

        data = str(data, encoding='ascii')
        data = data[:-1]
        self.received_data = data.split(self.DIVIDER)

        return self.received_data


