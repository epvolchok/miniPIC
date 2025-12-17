import os
import numpy as np
import matplotlib
matplotlib.use('qtagg')
from matplotlib import rc
import matplotlib.pyplot as plt
from mpl_toolkits.axes_grid1 import make_axes_locatable
from .libReadData import *

rc('text', usetex=True)
rc('font', family='serif')
rc('text.latex', preamble=r"\usepackage[utf8]{inputenc}")
matplotlib.rcParams.update({'font.size': 14})

folder_dependencies1D = {
    "Electric": ["FieldE", "ElectricFields"],
    "Magnetic": ["FieldB", "MagneticFields"],
    "Current": ["FieldJ", "Current"]
}

def make_filename(ind, MaxSize):
    return str(ind).zfill(len(str(MaxSize)))


def plot_2DField(fig, ax, data, title, xlabel="x", ylabel="y", cm="plasma", extent=None, limits=None):
    if limits:
        im = ax.imshow(data, cmap=cm, extent=extent, vmin=limits[0], vmax=limits[1], aspect='auto')
    else:
        im = ax.imshow(data, cmap=cm, extent=extent, aspect='auto')
    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel)
    ax.set_title(title)
    divider = make_axes_locatable(ax)
    cax = divider.append_axes("right", size="2%", pad=0.1)#свойства colorbar'а
    cbar =fig.colorbar(im, cax=cax, orientation='vertical')

def plot_Fields(field_path, TimeStep, MaxTime, fig, axs, 
                titles, xlabel="x", ylabel="y", cm="plasma", extent=None, limits=None):
    F = read_fieldgrid(field_path)

    for i, iF in enumerate(F):
        plot_2DField(fig, axs[i], iF, titles[i], xlabel, ylabel, cm, extent, limits)

def plot_1D(ax, data, label="", limits=None):
    if limits:
        #print(limits)
        xs = np.linspace(*limits)
        ax.plot(xs, data, label=label)
    else:
        ax.plot(data, label=label)


def plot_slice(ax, field_dir, types, diag_type, diag_coord, components, N, limits, TimeStep, MaxTime, labels):

    for i, field_type in enumerate(types):
        filename = folder_dependencies1D[field_type][0] + f"_{diag_type}" + \
            make_filename(diag_coord, N)+"_Time" + make_filename(TimeStep, MaxTime) + ".txt"
        path = os.path.join(field_dir, "1D", folder_dependencies1D[field_type][1], filename)
        data = read_txt(path).T[components[i]]
        plot_1D(ax, data, label=labels[i],  limits=limits[i])

def plot_2Dmaps(fig, axs, field_dir, types, titles, TimeStep, MaxTime):
    for i, field_type in enumerate(types):
        filename = folder_dependencies1D[field_type][0] +make_filename(TimeStep, MaxTime)+ ".bin"
        path = os.path.join(field_dir, "2D", folder_dependencies1D[field_type][1], filename)
        plot_Fields(path, TimeStep, MaxTime, fig, axs[i], titles[i])

def plot_EBenergy(ax, field_dir, TimeStep):
    filename = "EBFields_energy.txt"
    path = os.path.join(field_dir, "Energy", filename)
    data = read_txt(path).T
    E_en = data[1]
    B_en = data[2]

    ax.plot(E_en[:TimeStep+1], label='electric')
    ax.plot(B_en[:TimeStep+1], label='magnetic')
    ax.plot(E_en[:TimeStep+1]+B_en[:TimeStep+1], label='sum')
    ax.legend(loc="upper left")
    ax.set_title("Fields energy")
    ax.set_ylabel("Energy")
    ax.set_xlabel("time, steps")

def plot_all(fig, axs, field_dir, TimeStep, MaxTime, Nx, Ny):
    types = ["Electric", "Magnetic", "Current"]
    titles_E = [r"$E_x$", r"$E_y$", r"$E_z$"]
    titles_B = [r"$B_x$", r"$B_y$", r"$B_z$"]
    titles_J = [r"$J_x$", r"$J_y$", r"$J_z$"]
    titles = [titles_E, titles_B, titles_J]
    plot_2Dmaps(fig, axs, field_dir, types, titles, TimeStep, MaxTime)


    x_diag = 10
    title = f"Transverse X={x_diag}"
    components = [0, 0, 0] #Ax, Ay, Az
    labels = ["E field", "B field", "Current"]
    limits = [0, Ny, Ny]
    Limits = [limits for i in range(len(types))]
    plot_slice(axs[3][0], field_dir, types, "X", x_diag, components, Nx, Limits, TimeStep, MaxTime, labels)
    axs[3][0].set_title(title)
    axs[3][0].set_xlabel("y")
    axs[3][0].legend(loc="upper right")

    y_diag = 5
    title = f"Long Y={y_diag}"
    components = [0, 0, 0] #Ax, Ay, Az
    labels = ["E field", "B field", "Current"]
    limits = [0, Nx, Nx]
    Limits = [limits for i in range(len(types))]
    plot_slice(axs[3][1], field_dir, types, "Y", y_diag, components, Ny, Limits, TimeStep, MaxTime, labels)
    axs[3][1].set_title(title)
    axs[3][1].set_xlabel("x")
    axs[3][1].legend(loc="upper right")
    
    plot_EBenergy(axs[3][2], field_dir, TimeStep)
