import os
import numpy as np
import matplotlib
matplotlib.use('qtagg')
from matplotlib import rc
import matplotlib.pyplot as plt
from matplotlib import gridspec

rc('text', usetex=True)
rc('font', family='serif')
rc('text.latex', preamble=r"\usepackage[utf8]{inputenc}")
matplotlib.rcParams.update({'font.size': 18})

import vismodule as vis



def main():
    data_dir = os.path.normpath(os.path.join("..", os.getcwd(), "data"))
    field_dir = os.path.join(data_dir, "Fields")
    Nx, Ny = 10, 20
    MaxTime = 20
    TimeStep=1

    fig, axs = plt.subplots(4, 3, figsize=(16, 9))
    vis.plot_all(axs, field_dir, TimeStep, MaxTime)

    """ filename = "FieldE" +make_filename(TimeStep, MaxTime)+ ".bin"
    path = os.path.join(field_dir, "2D", "ElectricFields", filename)
    E = vis.read_fieldgrid(path)

    titles_E = [r'$E_x$', r'$E_y$', r'$E_z$']
    for i, iE in enumerate(E):
        vis.plot_2D(axs[0, i], iE, titles_E[i]) """

    
    plt.tight_layout()
    plt.show()

    """ path = os.path.join(os.getcwd(), "data", "Fields", "2D", filename)
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
    plt.show() """

if __name__ == '__main__':
    
    main()