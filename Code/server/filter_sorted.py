import time
import serial
import numpy as np

#NUM_TAGS = 1
#BAUDRATES = [115200]
#PORT_NAMES = ["COM7"]  # ???
#IS_OPEN = [False]
#UNO = "COM4"  # ???
#ports = []


class Navigation:
    def __init__(self, port, PORT_NAME):
        self.ports = []
        self.IS_OPEN = [False]
        self.NUM_TAGS = 1
        self.BAUDRATES = [115200]
        self.PORT_NAME = PORT_NAME
        self.port = port
        self.T = 0.01
        self.F = np.array([[1, -self.T], [0, 1]], dtype=float)  # матрица перехода
        self.H = np.array([[0], [self.T]], dtype=float)
        self.C = np.array([1, 0], dtype=float)  # матрица-строка связи
        self.K = np.empty((1, 2), dtype=float)
        self.M_waited = np.empty((2, 2), dtype=float)
        self.R = 0.2 ** 2
        self.Q = 0.01 ** 2  # ???
        self.ONE_STEP = 314 / 5400  # мм в шаге: 5400 - количество шагов для 1ого обората колеса,
                                    # 314 - длина окружности колеса
        self.V_step = 1000
        self.V = self.V_step * self.ONE_STEP  # сами задаём скорость в мм / c (перевести из щагов в мм)

        self.n_calc = 0
        self.M_calc = np.array([[self.R, 0], [0, self.Q]])

        self.sum = 0
        self.k = 0

    # не используется
    def tags_coords_processing(self, tags_coords):
        return (tags_coords[0][0], tags_coords[0][1], tags_coords[0][2])

    # инициализация тегов, нужно, чтобы ускорить вычисление среднего, когда платформа стоит
    # def init_tag(self):
    #     global tags_coords
    #     for tag in range(NUM_TAGS):
    #         if not IS_OPEN[tag]:
    #             ports.append(serial.Serial(PORT_NAMES[tag], BAUDRATES[tag], timeout=0.5))
    #             IS_OPEN[tag] = True
    #     time.sleep(2)

    def get_current_pos(self):
        tags_coords = {}
        for tag in range(self.NUM_TAGS):
            if not self.IS_OPEN[tag]:
                self.ports.append(serial.Serial(self.PORT_NAME[tag], self.BAUDRATES[tag], timeout=0.5))
                self.IS_OPEN[tag] = True
            line = ""
            ser = self.ports[tag]
            ser.reset_input_buffer()
            ser.reset_output_buffer()
            while len(line) == 0 or line[0] != 'x':
                if ser.writable():
                    ser.write(b'apg\n')
                if ser.readable():
                    line = ser.readline().decode("utf-8")
                    # print(line)
                    if len(line) > 0 and line[0] == 'x':
                        coordinates = line.split(" ")
                        x = int(coordinates[0][2:])
                        y = int(coordinates[1][2:])
                        z = int(coordinates[2][2:])
                        tags_coords[tag] = (x, y, z)
        return (tags_coords[0][0], tags_coords[0][1], tags_coords[0][2])

    def platform_sleep(self, seconds):
        start_time = time.time()
        while(time.time() - start_time) < seconds or self.k < 100:
            current_pos = self.get_current_pos()
            x_radio = current_pos[0]
            self.sum += x_radio
            self.k += 1
        return self.sum / self.k

    # send можно вынесли в основную программу
    # x_calc в основной программе x_calc = navigation.platform_sleep()

    def platform_moving(self, distance, x_calc):
        self.port.send(self.V_step)
        target_x = distance + x_calc
        last = time.time()
        while x_calc - target_x <= 0:  # едем, пока не приехали
            #print(x_calc - target_x)
            current_pos = self.get_current_pos()
            x_radio = current_pos[0]

            # x_radio += 10
            self.T = time.time() - last  # в секундах
            last = time.time()
            self.F[0, 1] = -self.T
            self.H[1] = self.T
            x_waited = x_calc + (self.V + self.n_calc) * self.T

            self.M_waited = self.F @ self.M_calc @ np.transpose(self.F) + self.Q * self.H @ np.transpose(
                self.H)  # @ - умножение матриц в numpy
            self.K = self.M_waited @ np.transpose(self.C) * 1 / (self.C @ self.M_waited @ np.transpose(self.C) + self.R)

            x_calc = x_waited + self.K[0] * (x_radio - x_waited)
            self.n_calc = self.n_calc + self.K[1] * (x_radio - x_waited)

            self.M_calc = self.M_waited - self.K * self.C * self.M_waited
        self.port.send('!')
        self.back_to_start_cond()

    def back_to_start_cond(self):

        self.n_calc = 0
        self.M_calc = np.array([[self.R, 0], [0, self.Q]])

        self.sum = 0
        self.k = 0
