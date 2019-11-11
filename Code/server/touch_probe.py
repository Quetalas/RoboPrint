from connections import Commands

import numpy as np

class TouchProbe:
    def __init__(self, port, tower):
        self.port = port
        self.tower = tower
        self.deltaX = 10
        self.deltaY = 10
        self.deltaZ = 2

    def up_prob(self):
        self.port.send(Commands['probe_up'])
        self.port.get()
        print("up sensor")

    def down_prob(self):
        self.port.send(Commands['probe_down'])
        self.port.get()

    def isTouched(self):
        self.port.send(Commands['is_touched'])
        if self.port.get()[0] == 'y':
            return True
        else:
            return False

    def get_point(self):
        point = np.zeros(3)
        point[0] = self.tower.x + self.deltaX
        point[1] = self.tower.y + self.deltaY
        point[2] = self.tower.z - self.deltaZ
        return point
# тест
# if __name__ == '__main__':
#     from connections import Connection
#     import time
#
#     with Connection('COM12') as mega:
#         time.sleep(2)
#         touch = TouchProbe(mega)
#         while (1):
#             print(touch.isTouched())
