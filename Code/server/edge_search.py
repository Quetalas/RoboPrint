import numpy as np


class EdgeSearch:

    def __init__(self, port, tower, TableCalibrate):
        self.port = port
        self.tower = tower
        self.Tc = TableCalibrate

    def iterrapt_move_to(self, dx=0.0, dy=0.0, dz=0.0):  # в абсолютных координатах принтера (прерывное движение)
        while self.Tc.Tp.isTouched() is False:  # проверить правильность условия касания
            self.tower.relative_move_to(self.port, x=dx, y=dy, z=dz)
        point = self.Tc.Tp.get_point()
        point = self.Tc.invert_transform(point)
        self.tower.relative_move_to(self.port, x=-dx, y=-dy, z=-dz)
        return point

    def sensor_mode(self):
        move_com = self.Tc.A @ np.array([0, 0, 6+self.Tc.Tp.deltaZ])
        self.tower.relative_move_to(self.port, x=move_com[0], y=move_com[1], z=move_com[2])
        self.Tc.Tp.up_prob()
        move_com = self.Tc.A @ np.array([self.Tc.Tp.deltaX, self.Tc.Tp.deltaY, 0])
        self.tower.relative_move_to(self.port, x=move_com[0], y=move_com[1], z=move_com[2])
        move_com = self.Tc.A @ np.array([0, 0, -6])
        self.tower.relative_move_to(self.port, x=move_com[0], y=move_com[1], z=move_com[2])
        # переезжает так, что активный болт датчика становится на место сопла

    def nozzle_mode(self):
        self.Tc.Tp.down_prob()
        move_com = self.Tc.A @ np.array([0, 0, self.tower.START_Z])
        self.Tc.special_move_to(X=move_com[0], Y=move_com[1], Z=move_com[2])
        # выключение датчика. перевод сопла в стартовое положение

    def border_seeker_algorithm(self):
        self.sensor_mode()
        # point = np.zeros(3)
        zero_point = self.Tc.Tp.get_point()
        move_com = self.Tc.A @ np.array([0, 0, 5])      # для относитьельного движения (в + по Z стола на 1)
        self.tower.relative_move_to(self.port, x=move_com[0], y=move_com[1], z=move_com[2])
        # по Х ищем наклон, отъезжаем назад
        move_com_d = self.Tc.A @ np.array([-1, 0, 0])    # для прерывного движения   (в - по Х, шаг 1)
        point = self.iterrapt_move_to(dx=move_com_d[0], dy=move_com_d[1], dz=move_com_d[2])
        move_com = self.Tc.A @ np.array([5, 0, 10])      # для относитьельного движения ( в + по Х смещаемся 1)
        self.tower.relative_move_to(self.port, x=move_com[0], y=move_com[1], z=move_com[2])

        move_com = self.Tc.A @ np.array([-0.1, 0, 0])   # ( в - по Х, шаг 0.1)
        move_com_z = self.Tc.A @ np.array([0, 0, 20])    # для относитьельного движения (в + по Z стола на 1)
        move_com_d = self.Tc.A @ np.array([0, 0, -0.1])  # (в - по Z стола на 0.1)
        print(point[2])
        a = int(input())    # из-за неровности стола и пробуксовки кареток получаемое значение сильно прыгало.
        # добавил чтобы хоть как-то протестить

        # минишаги для точного поска
        while point[2] < a:
            # шаг по X, по Z стола вниз(к 0) до касания, отъехать от стола/точки касания на 1 мм
            self.tower.relative_move_to(self.port, x=move_com[0], y=move_com[1], z=move_com[2])
            point = self.iterrapt_move_to(dx=move_com_d[0], dy=move_com_d[1], dz=move_com_d[2])

            self.tower.relative_move_to(self.port, x=move_com_z[0], y=move_com_z[1], z=move_com_z[2])
            if point[2] >= a:  # 0.1
                move_com = self.Tc.A @ np.array([1, 0, 0])
                self.tower.relative_move_to(self.port, x=move_com[0], y=move_com[1], z=move_com[2])
                break
            else:
                zero_point[0] = point[0]
                zero_point[1] = point[1]
                zero_point[2] = point[2]

        self.nozzle_mode()

        self.Tc.table_zero[0] = zero_point[0]
        self.Tc.table_zero[1] = zero_point[1]
        self.Tc.table_zero[2] = zero_point[2]
