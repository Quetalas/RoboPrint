import time

from connections import Connection, Commands
from printer import Tower
from utils import parse_gcode

MEGA = 'COM3'


if __name__ == '__main__':
    tower = Tower()
    with Connection(MEGA) as mega:
        time.sleep(2)

        file_name = 'kv.txt'
        with open(file_name) as gcode:

            for line in gcode:

                command = parse_gcode(line)
                if command['cmd'] == 'G0' or command['cmd'] == 'G1':

                    for command_for_mega in tower.move_to(command):
                        print(command_for_mega)
