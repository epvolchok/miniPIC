import os
import numpy as np
import matplotlib
matplotlib.use('qtagg')
from matplotlib import rc
import matplotlib.pyplot as plt
from matplotlib import gridspec
from multiprocessing import Pool

rc('text', usetex=True)
rc('font', family='serif')
rc('text.latex', preamble=r"\usepackage[utf8]{inputenc}")
matplotlib.rcParams.update({'font.size': 14})

from main_plot import plot_fig

def create_imag(field_dir, Graph_path, Params, TimeStep):
    print(TimeStep)
    Nx, Ny, MaxTime = Params
    width = len(str(MaxTime))
    Graph_name = f'FigFields{TimeStep:0{width}d}.png'

    temp_path = os.path.join(Graph_path, Graph_name)
    if os.path.exists(temp_path):
        return
    
    fig, axs = plt.subplots(4, 3, figsize=(17, 15))
    plot_fig(fig, axs, field_dir, Params, TimeStep)
    plt.savefig(temp_path, format='png', dpi=200)
    #plt.show()
    plt.close(fig)
    plt.clf()
    plt.close('all')

def main():
    data_dir = os.path.normpath(os.path.join("..", os.getcwd(), "data"))
    field_dir = os.path.join(data_dir, "Fields")
    Nx, Ny = 20, 10
    MaxTime = 50

    Params = [Nx, Ny, MaxTime]

    path = os.path.join(field_dir, "2D", "ElectricFields")
    files = sorted(os.listdir(path))
    print(files)

    LastTimeStep = int(files[-1][-(len(str(MaxTime))+4):-4])
    print(LastTimeStep)

    Graph_path = os.path.join(data_dir, "Anime")
    os.makedirs(Graph_path, exist_ok=True)
    Params = [Nx, Ny, MaxTime]
    arg_list = [(field_dir, Graph_path, Params, TimeStep) for TimeStep in range(LastTimeStep+1)]

    with Pool(processes=8) as pool:
        pool.starmap(create_imag, arg_list)

if __name__ == '__main__':
    
    main()