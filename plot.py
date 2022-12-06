import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os
import matplotlib.animation as animation

def get_count_file(dir_path):
    count_file = 0
    for path in os.listdir(dir_path):
        # check if current path is a file
        if os.path.isfile(os.path.join(dir_path, path)):
            count_file += 1

    return count_file

def get_file_arr(dir_path):
    count_file = get_count_file(dir_path)
    time_array = np.arange(0, count_file)
    file_arr = []
    for t in time_array:
        t_file = "\\" + "mesh" + str(t) + "_" + "99" + ".csv"
        file_arr.append(dir_path + t_file)
    frames_arr = np.arange(0, count_file)

    return file_arr, frames_arr


dir_path_MPI = r".\x64\Debug\data_plot"
dir_path_no_MPI = r".\ParticleMethod\data_plot"
fig = plt.figure(figsize=(10, 5))
file_arr_MPI, frames_arr_MPI = get_file_arr(dir_path_MPI)
file_arr_no_MPI, frames_arr_no_MPI = get_file_arr(dir_path_no_MPI)

def animate_MPI(i):
    fig.clear()
    df_mesh = pd.read_csv(
        file_arr_MPI[i],
        header=None,
    )
    plt.xlim(-5, 25)
    plt.ylim(-5, 15)
    plt.grid()
    s = plt.scatter(df_mesh.iloc[:, 0], df_mesh.iloc[:, 1])

def animate_no_MPI(i):
    fig.clear()
    df_mesh = pd.read_csv(
        file_arr_no_MPI[i],
        header=None,
    )
    plt.xlim(-5, 25)
    plt.ylim(-5, 15)
    plt.grid()
    s = plt.scatter(df_mesh.iloc[:, 0], df_mesh.iloc[:, 1])


ani_MPI = animation.FuncAnimation(
    fig, animate_MPI, interval=1, frames=frames_arr_MPI, save_count=None, cache_frame_data=False
)
ani_MPI.save("animation_MPI.gif", writer="pillow")

ani_no_MPI = animation.FuncAnimation(
    fig, animate_no_MPI, interval=1, frames=frames_arr_no_MPI, save_count=None, cache_frame_data=False
)
ani_no_MPI.save("animation_no_MPI.gif", writer="pillow")


