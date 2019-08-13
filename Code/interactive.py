from connections import Commands


def listen_heat(arduino):
    tmp = ''
    while tmp != ['Complete']:
        tmp = arduino.get()
        if tmp:
            print(tmp)
    tmp = ''
    while tmp != 's':
        for i in range(50):
            arduino.send(Commands['get_temp'])
            tmp = arduino.get()
            while not tmp:
                tmp = arduino.get()
                if tmp:
                    print(tmp)
        tmp = input()


def interactive_connection(arduino):
    line = ''
    while True:
        line = input()
        if line == 'exit':
            break
        line_parts = line.split(' ')
        cmd = line_parts[0]
        print('input', *line_parts)
        if (len(line_parts) > 1):
            arduino.send(Commands[line_parts[0]], *line_parts[1:])
        else:
            arduino.send((Commands[line_parts[0]]))
        if cmd == 'heat':
            listen_heat(arduino)
        else:
            tmp = ''
            while not tmp:
                tmp = arduino.get()
                if tmp:
                    print(tmp)

