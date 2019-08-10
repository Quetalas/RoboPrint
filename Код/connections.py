import serial
import time

class Arduino:

    BAUD_RATE = 9600
    received_data = []
    DIVIDER = ','
    STARTING = '{'
    ENDING = '}'

    def __init__(self, port):
        self.connection = serial.Serial(port, self.BAUD_RATE)   # Блокирующее считывание нужно методу read

        if self.connection.is_open:
            self.connection.close()
        self.connection.open()
        time.sleep(2)
        print("Connection complete")

    def send(self, *args):

        if len(args) == 0:
            return False

        data_pack = self.STARTING
        for i, d in enumerate(args):
            if i == len(args) - 1:
                data_pack += str(d) + self.ENDING
            else:
                data_pack += str(d) + ","
            while self.connection.out_waiting > min(2 * len(data_pack), 64):
                pass
            self.connection.write(bytes(data_pack.encode('ascii')))
            data_pack = ""

        return True

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


commands = {'g0': 0,
            'g1': 1,
            'echo': 2,
            'get_temp': 3,
            'set_temp': 4,
            'heat': 5,
            'freeze': 6
            }
