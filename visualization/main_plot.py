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
matplotlib.rcParams.update({'font.size': 14})

import vismodule as vis

def plot_fig(fig, axs, field_dir, params, TimeStep):
    Nx, Ny, MaxTime = params

    
    vis.plot_all(fig, axs, field_dir, TimeStep, MaxTime, Nx, Ny)
    
    #MaxTimeDt=round(MaxTime*dt,1)
    plt.figtext(0.5, 0.98,  rf'time = {TimeStep} steps',bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.5),
				color='black',fontsize=18,ha='center')

    
    plt.subplots_adjust(top=0.945,
    bottom=0.047,
    left=0.03,
    right=0.952,
    hspace=0.347,
    wspace=0.197)

def main():
    data_dir = os.path.normpath(os.path.join("..", os.getcwd(), "data"))
    field_dir = os.path.join(data_dir, "Fields")
    Nx, Ny = 20, 10
    MaxTime = 20
    TimeStep=30
    Params = [Nx, Ny, MaxTime]
    fig, axs = plt.subplots(4, 3, figsize=(17, 15))
    plot_fig(fig, axs, field_dir, Params, TimeStep)
    plt.show()



if __name__ == '__main__':
    
    main()