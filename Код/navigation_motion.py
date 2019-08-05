import serial   # Устанавливается командой pip (Совет: лучше использовать виртуальные среды python)
import math
import time

BAUDRATE_ARDUINO = 250000   # Замените на нужное (250000 - хорошее значение)
PORT_NAME_ARDUINO = "COM7"  # Замените на нужное

NUM_TAGS = 1
BAUDRATES = [115200]
PORT_NAMES = ["COM3"]
IS_OPEN = [False]

ports = []

def tags_coords_processing(tags_coords):
    return (tags_coords[0][0], tags_coords[0][1], tags_coords[0][2])

def get_steps(current_pos, destination):
    return (round(5400 * (destination[0] - current_pos[0]) / (2*math.pi*50)), ) * 4

def navigation_motion(destination):
    """

    :param destination: (x, y, z)
    :return:
    """
    global ports
    with serial.Serial(PORT_NAME_ARDUINO, BAUDRATE_ARDUINO, timeout=60) as arduino:
        arduino.reset_input_buffer()
        arduino.reset_output_buffer()
        try:
            while True:
                ###
                #***************Сюда можно вставить код для вычисления данных***************#
                ###
                # Т.к. данные нужны считывать с радионавигации, то можно либо вставить код для считывания сюда
                # либо считывать в отдельной программе и пренапрвить поток вывода (символ ">" В консоли)
                # либо запустить считывание радионавигации о отправку в ардуино в отдельных потоках Thread
                tags_coords = {}
                for tag in range(NUM_TAGS):
                    if not IS_OPEN[tag]:
                        ports.append(serial.Serial(PORT_NAMES[tag], BAUDRATES[tag], timeout=0.5))
                        IS_OPEN[tag] = True
                    line = ""
                    ser = ports[tag]
                    ser.reset_input_buffer()
                    ser.reset_output_buffer()
                    while len(line) == 0 or line[0] != 'x':
                        if ser.writable():
                            ser.write(b'apg\n')
                        if ser.readable():
                            line = ser.readline().decode("utf-8")
                            # print(line)
                            if len(line) > 0 and line[0] == 'x':
                                coordinates = line.split(" ")
                                x = int(coordinates[0][2:])
                                y = int(coordinates[1][2:])
                                z = int(coordinates[2][2:])
                                tags_coords[tag] = (x, y, z)
                                print("tag {}: {} {} {}".format(tag, x, y, z))
                current_pos = tags_coords_processing(tags_coords)
                if math.sqrt((current_pos[0] - destination[0])**2) < 50:
                    return True
                steps = get_steps(current_pos, destination)
                ###**********************************************************************####
                message = ''
                if arduino.readable():
                    message = arduino.read_until().decode("ascii", errors="ignore").strip()
                    # arduino.reset_input_buffer()
                    if message == "GET":
                        arduino.reset_output_buffer()
                        if arduino.writable():
                            #arduino.write(bytes("SOME TEXT".encode("ascii")))
                            arduino.write(bytes(" {} ".format(-steps[0]).encode("ascii")))
                            arduino.flush()
                            arduino.reset_input_buffer()
                    else:
                        print("Message is ", message)
        finally:
            print("******************")
            for port, i in zip(ports, range(NUM_TAGS)):
                port.close()
                IS_OPEN[i] = False
                print("port", i, "is closed")
            ports = []
            print("******************")
            return False

        return False

if __name__ == '__main__':
    navigation_motion((0, 800, 0))
    print("motion complete")
    time.sleep(3)
    navigation_motion((2000, 800, 0))