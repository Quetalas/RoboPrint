import time

from connections import Connection, Commands
from printer import Tower
from utils import parse_gcode
from interactive import random_echo

MEGA = 'COM3'

is_interactive = True


if __name__ == '__main__':
    tower = Tower()
    with Connection(MEGA) as mega:
        time.sleep(2)

        if (is_interactive):

            num_tests = 1000

            start_time = time.time()
            for i in range(num_tests):
                random_echo(mega)
            new_time = time.time()
            avarage = (new_time - start_time) / 1000
            print("Среднее время на отправку-приём сообщения", avarage)

            times = []
            counter = 0
            errors = []
            for i in range(num_tests):
                local_time = time.time()

                msg, answ = random_echo(mega)

                new_local_time = time.time()
                answ = [i for i in map(lambda i: int(i), answ)]
                times.append(new_local_time-local_time)

                if msg != answ:
                    counter += 1
                    errors.append((msg,answ))

            print("Максимальное время отправки-приёма сообщения: ", max(times))
            print("Минимальное время отправки-приёма сообщения: ", min(times))
            print("Количество ошибок отправки-приёма: ", counter)
            if counter > 0:
                print(errors[: len(errors)  if len(errors) < 10 else 10])
            print('Время на тест: ', time.time() - start_time)

        else:
            file_name = 'kv.txt'
            with open(file_name) as gcode:

                for line in gcode:

                    command = parse_gcode(line)
                    if command['cmd'] == 'G0' or command['cmd'] == 'G1':

                        for command_for_mega in tower.move_to(command):
                            mega.send(command_for_mega)
