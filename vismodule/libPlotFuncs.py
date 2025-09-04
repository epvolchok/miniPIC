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





def plot_2D(ax, data, xlabel, ylabel, title):
    ax.imshow(data, cmap='plasma')
    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel)
    ax.set_title(title)

def plot_1D(ax, data, xlabel, title):
    ax.plot(data)
    ax.set_xlabel(xlabel)
    ax.set_title(title)
