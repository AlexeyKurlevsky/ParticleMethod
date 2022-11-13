import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os
import matplotlib.animation as animation

dir_path = r".\ParticleMethod\data"
count_file = 0
for path in os.listdir(dir_path):
    # check if current path is a file
    if os.path.isfile(os.path.join(dir_path, path)):
        count_file += 1

time_array = np.arange(0, 20 + 0.01, 0.01)
file_arr = []
fig = plt.figure(figsize=(10, 5))
for t in time_array:
    t_new = round(t, 3)
    t_str = str(t_new).replace(".", "_")
    int_part, frac_part = t_str.split("_")
    t_file = "\\" + "mesh" + int_part + "_" + frac_part[::-1].zfill(2)[::-1] + ".csv"
    file_arr.append(dir_path + t_file)
frames_arr = np.arange(0, count_file, 100)


def animate(i):
    fig.clear()
    df_mesh = pd.read_csv(
        file_arr[i],
        header=None,
    )
    plt.xlim(-5, 25)
    plt.ylim(-5, 15)
    plt.grid()
    s = plt.scatter(df_mesh.iloc[:, 0], df_mesh.iloc[:, 1])


ani = animation.FuncAnimation(
    fig, animate, interval=1, frames=frames_arr, save_count=None, cache_frame_data=False
)
ani.save("animation.gif", writer="pillow")
