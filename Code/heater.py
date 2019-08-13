from connections import Connection, Commands

class Heater:

    def __init__(self, port, target_temp=30):
        self.port = port
        self.target_temp = target_temp

    @property
    def target_temp(self, target_temp):
        self.target_temp = target_temp
        self.port.send(*(Commands['set_temp'], target_temp))
        tmp = self.port.get()
        while not tmp:
            tmp = self.port.get()
        if tmp[0] != 0:
            print('Error code: ', tmp)

    @property
    def target_temp(self):
        return self.target_temp

    def get_temp(self):
        self.port.send(*(Commands['get_temp']))
        tmp = self.port
        while not tmp:
            tmp = self.port.get()
        return tmp[0]


