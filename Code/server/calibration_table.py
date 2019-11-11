import numpy as np


class TableCalibrate:

    def __init__(self, port, tower, touch_probe):
        self.port = port
        self.tower = tower
        self.Tp = touch_probe
        self.A = np.eye(3)
        self.table_zero = np.zeros(3)
        self.table_coords = np.zeros(3)
        self.last_pos = np.zeros(3)

    def special_move_to(self, **args):  # в абсолютных координатах принтера
        command = {'cmd': 'G0'}
        command.update(args)
        for commands_for_mega in self.tower.move_to(command):
            self.port.send(*commands_for_mega)
            self.port.get()

    def get_point(self, dx, dy, dz):
        x = self.tower.x
        y = self.tower.y
        z = self.tower.z
        while True:
            x = x + dx if dx != 0 else x
            y = y + dy if dy != 0 else y
            z = z + dz if dz != 0 else z
            self.special_move_to(X=x, Y=y, Z=z)
            if self.Tp.isTouched():
                self.special_move_to(X=x-dx, Y=y-dy, Z=z-dz)
                break
        re_par = self.Tp.get_point()     # возвращает точку касания

        return re_par

    """
        Предполагается, что датчик касания смещен в плостости ХУ и выше по Z.
        point передает координаты, как будто бы в точке качания сопло.
    """

    def transform_matrix_creation(self):
        print(self.Tp.isTouched)
        self.Tp.up_prob()
        print(self.Tp.isTouched)
        self.special_move_to(Z=420)
        self.table_zero = self.get_point(0, 0, 0.1)
        self.special_move_to(Z=420)
        self.special_move_to(X=200)
        point1 = self.get_point(0, 0, 0.1)
        self.special_move_to(Z=420)
        self.special_move_to(X=-10, Y=-30)
        point2 = self.get_point(0, 0, 0.1)
        self.special_move_to(Z=420)
        self.Tp.down_prob()

        axis_x = point1 - self.table_zero
        axis_y = point2 - self.table_zero
        axis_y = axis_y - (
                (axis_y @ axis_x) / (axis_x @ axis_x)) * axis_x  # ортогонализация axis_y  по отношению к axis_x
        axis_x = axis_x * (1 / np.linalg.norm(axis_x))
        axis_y = axis_y * (1 / np.linalg.norm(axis_y))

        axis_z = np.cross(axis_x, axis_y)

        self.A[0, 0] = axis_x[0]
        self.A[1, 0] = axis_x[1]
        self.A[2, 0] = axis_x[2]
        self.A[0, 1] = axis_y[0]
        self.A[1, 1] = axis_y[1]
        self.A[2, 1] = axis_y[2]
        self.A[0, 2] = axis_z[0]
        self.A[1, 2] = axis_z[1]
        self.A[2, 2] = axis_z[2]

        self.special_move_to(X=self.table_zero[0], Y=self.table_zero[1], Z=self.table_zero[2])
        self.last_pos = np.zeros(3)

    def transform_command(self, command):
        if command.get('X') is not None:
            self.last_pos[0] = command['X']
            self.table_coords[0] = command['X']
        else:
            self.table_coords[0] = self.last_pos[0]

        if command.get('Y') is not None:
            self.last_pos[1] = command['Y']
            self.table_coords[1] = command['Y']
        else:
            self.table_coords[1] = self.last_pos[1]

        if command.get('Z') is not None:
            self.last_pos[2] = command['Z']
            self.table_coords[2] = command['Z']
        else:
            self.table_coords[2] = self.last_pos[2]

        changed_command = self.table_zero + self.A @ self.table_coords

        command['X'] = changed_command[0]
        command['Y'] = changed_command[1]
        command['Z'] = changed_command[2]

    def invert_transform(self, absolute_coord):
        inv_A = np.linalg.inv(self.A)
        table_coord = inv_A.dot(absolute_coord-self.table_zero)
        return table_coord
