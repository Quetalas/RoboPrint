def parse_gcode(line):
    """
    :param line:
    :return:
    """
    if not line:
        return False
    known_commands = ('G0', 'G1', 'M109', 'M104')
    items = line.split(' ')
    command = {'cmd': items[0]}     # Имя команды
    if command['cmd'] not in known_commands:
        return command
    for item in items[1:]:
        if item[0] == 'X' or item[0] == 'Y' or item[0] == 'Z' or item[0] == 'F' or item[0] == 'S' or item[0] == 'E':
            command[item[0]] = float(item[1:])
    return command

