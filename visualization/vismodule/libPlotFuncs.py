import os
import numpy as np
import matplotlib
matplotlib.use('qtagg')
from matplotlib import rc
import matplotlib.pyplot as plt
from .libReadData import *

rc('text', usetex=True)
rc('font', family='serif')
rc('text.latex', preamble=r"\usepackage[utf8]{inputenc}")
matplotlib.rcParams.update({'font.size': 18})


def make_filename(ind, MaxSize):
    return str(ind).zfill(len(str(MaxSize)))


def plot_2DField(ax, data, title, xlabel="x", ylabel="y", cm="plasma", extent=None, limits=None):
    if limits:
        ax.imshow(data, cmap=cm, extent=extent, vmin=limits[0], vmax=limits[1])
    else:
        ax.imshow(data, cmap=cm, extent=extent)
    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel)
    ax.set_title(title)

def plot_Fields(field_path, TimeStep, MaxTime, axs, 
                titles, xlabel="x", ylabel="y", cm="plasma", extent=None, limits=None):
    F = read_fieldgrid(field_path)

    for i, iF in enumerate(F):
        plot_2DField(axs[i], iF, titles[i], xlabel, ylabel, cm, extent, limits)

def plot_1D(ax, data, xlabel, title):
    ax.plot(data)
    ax.set_xlabel(xlabel)
    ax.set_title(title)



def plot_all(axs, field_dir, TimeStep, MaxTime):
    titles_E = [r'$E_x$', r'$E_y$', r'$E_z$']
    filename = "FieldE" +make_filename(TimeStep, MaxTime)+ ".bin"
    path = os.path.join(field_dir, "2D", "ElectricFields", filename)
    plot_Fields(path, TimeStep, MaxTime, axs[0], titles_E)

    titles_B = [r'$B_x$', r'$B_y$', r'$B_z$']
    filename = "FieldB" +make_filename(TimeStep, MaxTime)+ ".bin"
    path = os.path.join(field_dir, "2D", "MagneticFields", filename)
    plot_Fields(path, TimeStep, MaxTime, axs[1], titles_B)

    x_diag = 10
    y_diag = 5

    filename = "PointFieldE_X" + make_filename(x_diag, len(str(x_diag)))+"_Y" + \
                                    make_filename(y_diag, len(str(y_diag))) + ".txt"
    path = os.path.join(field_dir, "Point", filename)
    data = read_txt(path)
    plot_1D(ax, data, xlabel, title)

    filename = "PointFieldE_X" + make_filename(x_diag, len(str(x_diag)))+"_Y" + \
                                    make_filename(y_diag, len(str(y_diag))) + ".txt"

