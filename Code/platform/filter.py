import numpy as np
import serial
import time
from connections import Connection

NUM_TAGS = 1
BAUDRATES = [115200]
PORT_NAMES = ["COM7"]  # ???
IS_OPEN = [False]
UNO = "COM4"  # ???
ports = []


def tags_coords_processing(tags_coords):
    return (tags_coords[0][0], tags_coords[0][1], tags_coords[0][2])

tags_coords = {}
def init_tag():
    global tags_coords
    for tag in range(NUM_TAGS):
        if not IS_OPEN[tag]:
            ports.append(serial.Serial(PORT_NAMES[tag], BAUDRATES[tag], timeout=0.5))
            IS_OPEN[tag] = True
    time.sleep(2)


def get_current_pos():
    tags_coords = {}
    for tag in range(NUM_TAGS):
        if not IS_OPEN[tag]:
            ports.append(serial.Serial(PORT_NAMES[tag], BAUDRATES[tag], timeout=0.5))
            IS_OPEN[tag] = True
        line = ""
        ser = ports[tag]
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


def back_platform(port):
    a = input()
    port.send(-1000, -1000, -1000, -1000)


if __name__ == "__main__":
    with Connection(UNO) as uno:
        time.sleep(2)
        T = 0.01
        F = np.array([[1, -T], [0, 1]], dtype=float)  # матрица перехода
        H = np.array([[0], [T]], dtype=float)
        C = np.array([1, 0], dtype=float)  # матрица-строка связи
        K = np.empty((1, 2), dtype=float)
        M_waited = np.empty((2, 2), dtype=float)
        R = 0.2 ** 2
        Q = 0.01 ** 2  # ???
        ONE_STEP = 0.1
        V_step = 1000
        V = V_step * 0.058  # сами задаём скорость в мм / c (перевести из щагов в мм)

        sum = 0
        k = 0
        # target_distance = 1000
        time1 = time.time()
        while time.time() - time1 < 10:  # когда стоим, например, пока не получили сигнал о завершении печати10
            current_pos = get_current_pos()
            x_radio = current_pos[0]
            # x_radio += 0.1
            sum += x_radio
            k += 1
        x_calc = sum / k
        print("x start position: ", x_calc)
        # print(x_calc)
        n_calc = 0
        M_calc = np.array([[R, 0], [0, Q]])
        uno.send(V_step, V_step, V_step, V_step)
        target_x = 1500
        last = time.time()
        while x_calc-target_x <= 0:  # когда едем, например, едем, пока не приехали
            current_pos = get_current_pos()
            x_radio = current_pos[0]

            # x_radio += 10
            T = time.time() - last  # в секундах
            last = time.time()
            F[0, 1] = -T
            H[1] = T
            x_waited = x_calc + (V + n_calc) * T

            M_waited = F @ M_calc @ np.transpose(F) + Q * H @ np.transpose(H)  # @ - умножение матриц в numpy
            K = M_waited @ np.transpose(C) * 1 / (C @ M_waited @ np.transpose(C) + R)

            x_calc = x_waited + K[0] * (x_radio - x_waited)
            n_calc = n_calc + K[1] * (x_radio - x_waited)

            M_calc = M_waited - K * C * M_waited
            print(x_calc)
            #print("condition: ", target_x - x_calc, " x_calc: ", x_calc, " x_waited: ", x_waited, "Time", T)
        # отправляем стоп
        uno.send('!')
        back_platform(uno)
