import time

from connections import Connection, Commands
from printer import Tower
from utils import parse_gcode
from tests import run_tests


MEGA = 'COM4'
is_interactive = False


if __name__ == '__main__':
    tower = Tower()
    with Connection(MEGA) as mega:
        time.sleep(2)

        if (is_interactive):

            run_tests(mega, num_tests=1000)

        else:
            file_name = 'test.gcode'
            with open(file_name) as gcode:

                for line in gcode:

                    command = parse_gcode(line)
                    if command['cmd'] == 'G0' or command['cmd'] == 'G1':

                        for command_for_mega in tower.move_to(command):
                            mega.send(*command_for_mega)
                            while True:
                                tmp = mega.get()
                                if tmp != []:
                                    print(tmp)
                                    break