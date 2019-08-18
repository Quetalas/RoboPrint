import time

from connections import Connection, Commands
from printer import Tower
from utils_S import parse_gcode
from tests import run_tests
from heater_S import Heater

MEGA = 'COM3'
is_interactive = False
heater = Heater(MEGA)

if __name__ == '__main__':
    tower = Tower(car_x=-293, car_y=293, car_z=293, car_e=-293)
    with Connection(MEGA) as mega:
        time.sleep(2)
        heater = Heater(mega)
        print('start')
        if (is_interactive):

            run_tests(mega, num_tests=1000)

        else:
            file_name = '220x50.gcode'
            with open(file_name) as gcode:
                for line in gcode:

                    command = parse_gcode(line)
                    if command['cmd'] == 'G0' or command['cmd'] == 'G1':

                        for command_for_mega in tower.move_to(command):
                            x, y, z, e = command_for_mega[1:5]
                            print(tower.pos_from_cars(x / 10, y / 10 , z/10, e/10))
                            print(command_for_mega)
                            mega.send(*command_for_mega)
                            while True:
                                tmp = mega.get()
                                if tmp != []:
                                    print(tmp)
                                    break
                    if command['cmd'] == 'M109':
                        heater.set_target_temp(command['S'])  # command['S'] содержит температуру нагрева
                        heater.heating()
                    if command['cmd'] == 'M104':
                        heater.set_target_temp(command['S'])
                        heater.heating()

