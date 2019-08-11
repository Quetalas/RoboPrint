def parse_gcode(line):
    """
    :param line:
    :return:
    """
    if not line:
        return False

    items = line.split(' ')
    command = {'cmd': items[0]}     # Имя команды
    for item in items[1:]:
        command[item[0]] = float(item[1:])
    return command
