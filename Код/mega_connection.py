from connections import Arduino, commands

def listen_heat(arduino):
    tmp = ''
    while tmp != ['Complete']:
        tmp = arduino.read()
        if tmp:
            print(tmp)
    tmp = ''
    while tmp != 's':
        for i in range(50):
            arduino.send(commands['get_temp'])
            tmp = arduino.read()
            while not tmp:
                tmp = arduino.read()
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
            arduino.send(commands[line_parts[0]], *line_parts[1:])
        else:
            arduino.send((commands[line_parts[0]]))
        if cmd == 'heat':
            listen_heat(arduino)
        else:
            tmp = ''
            while not tmp:
                tmp = arduino.read()
                if tmp:
                    print(tmp)





if __name__ == '__main__':
    mega = Arduino("COM8")
    interactive_connection(mega)
