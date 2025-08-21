import os
import numpy as np
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

    fig, (ax_2D, ax_1D) = plt.subplots(nrows=2, ncols=1, figsize=(8,8))
    filename = "test.bin"
    path = os.path.join(os.getcwd(), "data", "results", filename)
    data2D = read_bin_data(path, Nx, Ny)
    print(data2D.shape)
    plot_2D(ax_2D, data2D, 'x', 'y', 'Data')

    filename = "test_line.txt"
    path = os.path.join(os.getcwd(), "data", "results", filename)
    data1D = read_txt(path)
    plot_1D(ax_1D, data1D, 'x', 'Slice along y')

    plt.tight_layout()
    plt.show()

if __name__ == '__main__':
    
    main()