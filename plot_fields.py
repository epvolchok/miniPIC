import os
import numpy as np
import struct
import matplotlib
matplotlib.use('qtagg')
from matplotlib import rc
import matplotlib.pyplot as plt

rc('text', usetex=True)
rc('font', family='serif')
rc('text.latex', preamble=r"\usepackage[utf8]{inputenc}")
matplotlib.rcParams.update({'font.size': 18})



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

def plot_2D(ax, data, xlabel, ylabel, title):
    ax.imshow(data, cmap='plasma')
    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel)
    ax.set_title(title)

def plot_1D(ax, data, xlabel, title):
    ax.plot(data)
    ax.set_xlabel(xlabel)
    ax.set_title(title)


def main():
    Nx, Ny = 10, 20

    fig, (ax1, ax2, ax3, ax4) = plt.subplots(nrows=4, ncols=1, figsize=(5,16))
    filename = "FieldsE01.bin"
    path = os.path.join(os.getcwd(), "data", "Fields", "2D", filename)
    Nx, Ny, Jx, Jy, Jz = read_fieldgrid(path, dtype=np.float64)
    print(Jx.shape)
    plot_2D(ax1, Jx, 'x', 'y', '')

    filename = "FieldsE05.bin"
    path = os.path.join(os.getcwd(), "data", "Fields", "2D", filename)
    Nx, Ny, Jx, Jy, Jz = read_fieldgrid(path, dtype=np.float64)
    print(Jx.shape)
    plot_2D(ax2, Jx, 'x', 'y', '')

    filename = "FieldsE10.bin"
    path = os.path.join(os.getcwd(), "data", "Fields", "2D", filename)
    Nx, Ny, Jx, Jy, Jz = read_fieldgrid(path, dtype=np.float64)
    print(Jx.shape)
    plot_2D(ax3, Jx, 'x', 'y', '')

    filename = "FieldsE19.bin"
    path = os.path.join(os.getcwd(), "data", "Fields", "2D", filename)
    Nx, Ny, Jx, Jy, Jz = read_fieldgrid(path, dtype=np.float64)
    print(Jx.shape)
    plot_2D(ax4, Jx, 'x', 'y', '')

    plt.tight_layout()
    plt.show()

if __name__ == '__main__':
    
    main()