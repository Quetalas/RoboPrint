import serial


class Arduino:

    BAUD_RATE = 250000
    received_data = []
    DIVIDER = ' '
    STARTING = '{'
    ENDING = '}'

    def __init__(self, port):
        self.connection = serial.Serial(port, self.BAUD_RATE)   # Блокирующее считывание нужно методу read

        if self.connection.is_open:
            self.connection.close()
        self.connection.open()

    def send(self, string=''):
        while self.connection.out_waiting > 128:
            pass
        string = self.STARTING + string + self.ENDING
        self.connection.write(bytes(string.encode('ascii')))

    def read(self):
        """
        Если данные пришли, то начинает считывание со стартового байта,
        читает до тех пор, пока не получит байт конца данных.
        Использует байт-разделитель, чтобы получить массив данных из строки.
        :return:
        """

        self.received_data = []
        num_input_bytes = self.connection.in_waiting
        if num_input_bytes > 0:
            for i in range(num_input_bytes):

                next_byte = self.connection.read(1)
                next_byte = str(next_byte, encoding='ascii')

                if next_byte == self.STARTING:
                    data = self.connection.read_until(bytes(self.ENDING.encode('ascii')))
                    data = str(data, encoding='ascii')
                    data = data[:-1]
                    self.received_data = data.split(self.DIVIDER)
                    return self.received_data

        return self.received_data   # [] если нет стартового символа

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.__del__()

    def __del__(self):
        self.connection.close()


if __name__ == '__main__':
    import time
    ard = Arduino("COM4")
    while True:
        ard.send('Example of data ' + str(time.time()))
        st = ard.read()
        if st:
            print(st)