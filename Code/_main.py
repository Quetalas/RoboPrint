import serial
import navigation_motion

BAUDRATE_ARDUINO_MEGA = 250000   # Замените на нужное (250000 - хорошее значение)
PORT_NAME_ARDUINO_MEGA = "COM6"  # Замените на нужное

def communication_mega():
    with serial.Serial(PORT_NAME_ARDUINO_MEGA, BAUDRATE_ARDUINO_MEGA, timeout=60) as mega:
        mega.reset_input_buffer()
        mega.reset_output_buffer()
        while True:
            message = ''
            if mega.readable():
                message = mega.read_until().decode("ascii", errors="ignore").strip()
                # arduino.reset_input_buffer()
                if message == "READY":
                    mega.reset_output_buffer()
                    if mega.writable():
                        # arduino.write(bytes("SOME TEXT".encode("ascii")))
                        mega.write(bytes("ON.".encode("ascii")))
                        mega.flush()
                    mega.reset_input_buffer()
                elif message == "END_PRINT":
                    print("END_PRINT")
                    return True
                else:
                    print("Message is ", message)


if __name__ == '__main__':
    communication_mega()
    #navigation_motion.navigation_motion((500, 0, 0))
    communication_mega()