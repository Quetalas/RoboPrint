from connections import Connection, Commands


class Heater:

    def __init__(self, port, target_temp=30):
        self.port = port
        self.target_temp = target_temp

    def set_target_temp(self, target_temp):
        self.target_temp = target_temp
        self.port.send(*(Commands['set_temp'], target_temp))
        tmp = self.port.get()
        while not tmp:
            tmp = self.port.get()
        if tmp[0] != 0:
            print('Error code: ', tmp)

    @property
    def get_target_temp(self):
        return self.get_target_temp

    def get_temp(self):
        self.port.send(*(Commands['get_temp']))
        tmp = self.port.get()
        while not tmp:
            tmp = self.port.get()
        return tmp[0]

    def heating(self):
        self.port.send(Commands['heat'], '')
        tmp = self.port.get()
        while tmp != ['0']:
            if tmp != []:
                print('Now temperature is ', tmp)
            tmp = self.port.get()
        return tmp[0]
