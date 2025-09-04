import os
import numpy as np
import struct

def read_bin_data(path, Nx, Ny, type=np.float64):
    data = np.fromfile(path, dtype=type).reshape(Ny, Nx)
    return data

def read_fieldgrid(filename, dtype=np.float64):
    with open(filename, "rb") as f:
        # читаем размеры
        Nx = struct.unpack("Q", f.read(8))[0]  # size_t = 8 байт (обычно на 64-бит)
        Ny = struct.unpack("Q", f.read(8))[0]

        size = Nx * Ny

        # читаем данные Ex, Ey, Ez (по порядку записи)
        Ex = np.frombuffer(f.read(size * np.dtype(dtype).itemsize), dtype=dtype).reshape(Ny, Nx)
        Ey = np.frombuffer(f.read(size * np.dtype(dtype).itemsize), dtype=dtype).reshape(Ny, Nx)
        Ez = np.frombuffer(f.read(size * np.dtype(dtype).itemsize), dtype=dtype).reshape(Ny, Nx)

    return Nx, Ny, Ex, Ey, Ez

def read_txt(path):
    data = np.loadtxt(path)
    return data