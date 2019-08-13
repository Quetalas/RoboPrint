import random
import time
from connections import Commands


def test_echo(port, max_value, msg_len):
    """
    :param port:
    :param max_value:
    :param msg_len: длина сообщений не считая команду
    :return: (отправлено, получено, затраченное время)
    """
    msg_len_1 = msg_len // 2
    msg = [Commands['echo']] + \
          [random.randint(0, max_value)] * random.randint(1, msg_len_1) + \
          [random.random() * max_value] * random.randint(1, msg_len - msg_len_1)

    start = time.time()

    port.send(*msg)
    tmp = port.get()
    while not tmp:
        tmp = port.get()

    end = time.time()
    return msg, tmp, end - start


def run_tests(port, num_tests=10):

    start = time.time()
    times = []
    counter = 0
    errors = []
    for i in range(num_tests):
        print('\r', i, end='')
        msg, answ, period = test_echo(port, max_value=100000000, msg_len=4)

        msg = [i for i in map(lambda i: str(i), msg)]

        times.append(period)
        if msg != answ:
            counter += 1
            errors.append((msg, answ))

    end = time.time()
    avarage = sum(times) / len(times)
    print('Количество испытаний: ', num_tests)
    print("Среднее время на отправку-приём сообщения", avarage)
    print("Максимальное время отправки-приёма сообщения: ", max(times))
    print("Минимальное время отправки-приёма сообщения: ", min(times))
    print("Количество ошибок отправки-приёма: ", counter)
    if counter > 0:
        print('Ошибки:', errors[: len(errors) if len(errors) < 10 else 10])
    print('Время на тест: ', end - start)