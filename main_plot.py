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

from vismodule import *


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