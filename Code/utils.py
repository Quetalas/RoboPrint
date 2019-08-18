from connections import Commands

def parse_gcode(line):
    """
    :param line:
    :return:
    """
    items = line.split(' ')
    command = {'cmd': items[0]}     # Имя команды

    if command['cmd'].lower() in Commands.keys():
        for item in items[1:]:
            try:
                value = float(item[1:])
                command[item[0]] = value
            except ValueError:
                pass
    return command
