from connections import Arduino
from moving import Tower
file_name = 'cube.txt'
port_mega = 'COM4'
def parse_line(line):
    """
    :param line:
    :return:
    """
    items = line.split(' ')
    command = {'cmd': items[0]} # имя команды
    for item in items[1:]:
        command[item[0]]  = float(item[1:])

    return command

if __name__ == '__main__':
    mega = Arduino(port_mega)
    tower = Tower()
    with open(file_name) as gcode:
        for line in gcode:
            command = parse_line(line)
            if command['cmd'] == 'G0' or command['cmd'] == 'G1':
                tower.move_to(command, mega)
            else:
                pass