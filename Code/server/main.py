import time
import numpy as np

from connections import Connection, Commands
from printer import Tower
from utils import parse_gcode
from tests import run_tests
from heater import Heater
from touch_probe import TouchProbe
from calibration_table import TableCalibrate
from edge_search import EdgeSearch
from filter_sorted import Navigation

# import camera
# import board
# from calibration import Calib
# from scanner import Scanner
"""
    Лазерный дальномер заменен на датчик касания. 
    Включена калибровка по столу по датчику касания.    
    Добавлен поиск края детали.
"""

MEGA = "COM4"
# UNO = "COM5"
TAG_PORT = ["COM6"]
is_interactive = False

start_car_pos = [-300, 300, 300, -300]
standart_v = 40

if __name__ == '__main__':
    # camera = camera.Camera(camera_id=1)
    # camera.connect()
    # camera.set_brightness(100)
    # camera.set_exposure(30)
    # board = board.Board(serial_name="COM9")
    # board.connect()
    # # board.laser_on(0)
    # calib = Calib((7, 5), camera, board, squareSize=33)
    # # calib.makeSamples()
    # calib.calibrate(loadPlane='laserPlane.npy')
    #
    # scanner = Scanner(camera, board, calib)
    # objectScan = None

    tower = Tower(car_x=start_car_pos[0], car_y=start_car_pos[1], car_z=start_car_pos[2], car_e=start_car_pos[3],
                  step_length=2, std_v=standart_v)
    # with Connection(MEGA) as mega, \
    #         Connection(UNO) as uno:
    with Connection(MEGA) as mega:
        """
            блок инициализации объектов
        """
        # navi = Navigation(uno, TAG_PORT)
        touch_probe = TouchProbe(mega, tower)
        heater = Heater(mega)
        table = TableCalibrate(mega, tower, touch_probe)
        edge_search = EdgeSearch(mega, tower, table)
        time.sleep(2)
        print('start')
        if (is_interactive):

            run_tests(mega, num_tests=1000)

        else:
            # list_of_files = ['1st_move.gcode', '2nd_move.txt']
            # for file_name in list_of_files:
            for i in range(1):
                file_name = 'aramka200x100.gcode'
                with open(file_name) as gcode:
                    for line in gcode:
                        command = parse_gcode(line)
                        h = heater.get_temp()
                        print(h)
                        if command['cmd'] == 'G0' or command['cmd'] == 'G1':
                            table.transform_command(command)
                            for command_for_mega in tower.move_to(command):
                                mega.send(*command_for_mega)
                                tmp = mega.get()
                            # X = scanner.measureLaserLineDistances()
                            # if objectScan is not None:
                            #     np.concatenate((objectScan, X), axis=0)
                            # else:
                            #     objectScan = X
                        if command['cmd'] == 'M104':
                            heater.set_target_temp(command['S'])  # command['S'] содержит температуру нагрева
                        if command['cmd'] == 'M109':
                            heater.heating()
                        if command['cmd'] == 'G29':
                            table.transform_matrix_creation()
                        if command['cmd'] == 'G28':
                            cx, cy, cz, ce = tower.home_car_pos
                            data_pack = (Commands['G28'], cx, cy, cz, ce)
                            mega.send(*data_pack)
                            tmp = mega.get()
                            print(tower.START_X, tower.START_Y, tower.START_Z)
                            print(cx, cy, cz, ce)
                        if command['cmd'] == 'edge_search':
                            edge_search.border_seeker_algorithm()
                        if command['cmd'] == 'G92':
                            mega.send(Commands['G92'], tower.set_ext_pos(command))
                            tmp = mega.get()
                    # fig = go.Figure(data=[go.Scatter3d(x=objectScan[:, 0], y=objectScan[:, 1], z=objectScan[:, 2], mode='markers')])
                    # fig.show()
                table.special_move_to(X=tower.START_X, Y=tower.START_Y, Z=tower.START_Z)
                print("10 sec")
                # x_calc = navi.platform_sleep(10)  # abuyz, not doing
                # navi.platform_moving(200, x_calc)  # 100

            table.special_move_to(X=tower.START_X, Y=tower.START_Y, Z=tower.START_Z)
