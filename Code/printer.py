import math
from connections import Commands


class Tower:

    def __init__(self, car_x=-290, car_y=290, car_z=290, car_e=-290, step_length=10, std_v =40, std_v_ext=0.1):
        """
        :param car_x:  начальная позиция каретки(шагового двигателя) в мм
        :param car_y:
        :param car_z:
        :param car_e:
        :param step_length: примерная длина отрезка для разбиения пути в мм (реальная расчитывается)
        :param std_v: скорость движения по умолчанию мм/с
        :param std_v_extr: скорость выдавливания экструдера по умолчанию (рассчитывается если дана в команде) в мм/с
        """
        self._PLATFORM_PARAMETER = 70.0
        self._TABLE_WIDTH = 230.0
        self._TOWER_HEIGHT = 405.0
        self._EXTRUDER_HEIGHT = 100.0
        self._ONE_STEP = 0.1
        self._ONE_EXTR_STEP = 0.01
        self._TH2 = self._TOWER_HEIGHT ** 2
        self._PPsqrt2 = self._PLATFORM_PARAMETER / math.sqrt(2)

        x, y, z = self.pos_from_cars(car_x, car_y, car_z, car_e)    # Получаем позицию сопла
        self.x = x
        self.y = y
        self.z = z
        self.step_length = step_length
        self.vel = std_v    # Если не дано в команде, то берётся предыдущая
        self.vel_ext = std_v_ext
        self.ext_pos = 0    # Начальная позиция движка экструдера

    def cars_from_pos(self, x, y, z):
        """
        Расчитывает координаты X кареток относительно нуля из координат положения сопла
        :param x:
        :param y:
        :param z:
        :return: в миллиметрах
        """
        root1 = math.sqrt(self._TH2 - (-self._TABLE_WIDTH / 2 - (y - self._PPsqrt2)) ** 2 - (z - self._EXTRUDER_HEIGHT) ** 2)
        root2 = math.sqrt(self._TH2 - (self._TABLE_WIDTH / 2 - (y + self._PPsqrt2)) ** 2 - (z - self._EXTRUDER_HEIGHT) ** 2)
        return (x - self._PPsqrt2 - root1,
                x + self._PPsqrt2 + root1,
                x + self._PPsqrt2 + root2,
                x - self._PPsqrt2 - root2)

    def pos_from_cars(self, car_x, car_y, car_z, car_e):
        """
        Расчитывает координаты положения сопла из координат X кареток относительно нуля
        :param car_x: в мм
        :param car_y: в мм
        :param car_z: в мм
        :param car_e: в мм
        :return:
        """
        x = (car_x + car_y) / 2
        delta_xy = math.fabs(car_x - car_y) / 2
        delta_ze = math.fabs(car_z - car_e) / 2
        y = ((delta_xy - self._PPsqrt2)**2 - (delta_ze - self._PPsqrt2)**2) /\
            (4 * self._PPsqrt2 - 2 * self._TABLE_WIDTH)
        z = self._EXTRUDER_HEIGHT + math.sqrt(
            self._TH2 - (delta_xy - self._PPsqrt2)**2 - (-self._TABLE_WIDTH / 2 - y + self._PPsqrt2)**2)
        return x, y, z

    def move_to(self, command):
        """
        :param command:
        :return:
        """

        self.vel = command['F'] / 60 if command.get('F') is not None else self.vel

        _, _, Z = self.pos_from_cars(car_x=-290, car_y=290, car_z=290, car_e=-290)
        # деление отрезка из считанной строки на интервалы
        dx = command['X'] - self.x if command.get('X') is not None else 0
        dy = command['Y'] - self.y if command.get('Y') is not None else 0
        dz = (Z - command['Z']) - self.z if command.get('Z') is not None else 0

        num_del = round(math.sqrt(dy**2 + dz**2) / self.step_length)
        if num_del == 0:
            num_del = 1  # если округл. число микро шагов = 0, то значит, у нас будет 1 шаг

        full_length = math.sqrt(dx**2 + dy**2 + dz**2)

        if full_length == 0:    # g1 без движения не обрабатывается или если движемся в точку в которой уже стоим
            return

        real_step_length = full_length / num_del

        mini_dx = dx / num_del
        mini_dy = dy / num_del
        mini_dz = dz / num_del

        # рассчёт шагов для экструдера

        time_for_step = real_step_length / self.vel

        ext_micro_step = 0

        if command['cmd'] == 'G1':
            if command.get('E') is None:
                ext_micro_step = 0
            else:
                ext_micro_step = (command['E'] - self.ext_pos) / num_del
                self.vel_ext = ext_micro_step / time_for_step

        for i in range(num_del):

            target_x = self.x + mini_dx
            target_y = self.y + mini_dy
            target_z = self.z + mini_dz
            ext_target = self.ext_pos + ext_micro_step

            car1_x, car1_y, car1_z, car1_e = [j for j in map(lambda length: length / self._ONE_STEP ,
                                                             self.cars_from_pos(self.x, self.y, self.z))]
            car2_x, car2_y, car2_z, car2_e = [j for j in map(lambda length: length / self._ONE_STEP ,
                                                             self.cars_from_pos(target_x, target_y, target_z))]

            car_speed_x = (car2_x - car1_x) / time_for_step
            car_speed_y = (car2_y - car1_y) / time_for_step
            car_speed_z = (car2_z - car1_z) / time_for_step
            car_speed_e = (car2_e - car1_e) / time_for_step

            # формируем данные для передачи

            car2_x = round(car2_x)
            car2_y = round(car2_y)
            car2_z = round(car2_z)
            car2_e = round(car2_e)

            car_speed_x = round(math.fabs(car_speed_x), 2)
            car_speed_y = round(math.fabs(car_speed_y), 2)
            car_speed_z = round(math.fabs(car_speed_z), 2)
            car_speed_e = round(math.fabs(car_speed_e), 2)

            data_pack = ()
            if command['cmd'] == 'G1':
                data_pack = (Commands['g1'], car2_x, car2_y, car2_z, car2_e,
                             car_speed_x, car_speed_y, car_speed_z, car_speed_e,
                             round(ext_target / self._ONE_STEP), round(math.fabs(self.vel_ext / self._ONE_STEP), 2))

            elif command['cmd'] == 'G0':
                data_pack = (Commands['g0'], car2_x, car2_y, car2_z, car2_e,
                             car_speed_x, car_speed_y, car_speed_z, car_speed_e)

            yield data_pack

            self.x = target_x
            self.y = target_y
            self.z = target_z

            self.ext_pos = ext_target

