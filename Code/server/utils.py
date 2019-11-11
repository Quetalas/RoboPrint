from connections import Commands

def parse_gcode(line):
    """
    :param line:
    :return:
    """
    if line[-1] == '\n':
        line = line[:-1]
    items = line.split(' ')
    command = {'cmd': items[0]}     # Имя команды

    if command['cmd'] in Commands.keys():
        for item in items[1:]:
            try:
                value = float(item[1:])
                command[item[0]] = value
            except ValueError:
                pass
    return command
