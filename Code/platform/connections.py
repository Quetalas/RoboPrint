import serial

Commands = {'g0': 0, 'g1': 1, 'echo': 2, 'get_temp': 3, 'set_temp': 4, 'heat': 5, 'freeze': 6}


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

    def get(self):  # Сделать блокирующим
        """
        Если данные пришли, то начинает считывание со стартового байта,
        читает до тех пор, пока не получит байт конца данных.
        Использует байт-разделитель, чтобы получить массив данных из строки.
        :return:
        """
        self.received_data.clear()
        if self.in_waiting > 0:
            for i in range(self.in_waiting):
                next_byte = self.read(1)

                if next_byte == self.STARTING:
                    data = self.read_until(self.ENDING)
                    data = str(data, encoding='ascii')
                    data = data[:-1]
                    self.received_data = data.split(self.DIVIDER)
                    return self.received_data

        return self.received_data   # [] если нет стартового символа

