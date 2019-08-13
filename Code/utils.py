def parse_gcode(line):
    """
    :param line:
    :return:
    """
    if not line:
        return False

    items = line.split(' ')
    command = {'cmd': items[0]}     # Имя команды
    if command['cmd'] != 'G1' and command['cmd'] != 'G0':
        return command
    for item in items[1:]:
        if item[0] == 'X' or item[0] == 'Y' or item[0] == 'Z' or item[0] == 'F' or item[0] == 'S' or item[0] == 'E':
            command[item[0]] = float(item[1:])
    return command
