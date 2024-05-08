import sys
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import matplotlib as mpl
import matplotlib.cm as cm
n = len(sys.argv)

if n != 2 and n != 3:
    print("Usage: python data_visualization.py filename [color]")
    sys.exit(1)

filename = sys.argv[1]
color = sys.argv[2] if n == 3 else None

with open(filename, 'r') as f:
    data = f.read().split('\n\n')

masses = np.fromstring(data[0], sep='\n')

positions = data[1:]
positions = [position.split('\n') for position in positions]
positions = [[np.fromstring(p, sep=',') for p in position] for position in positions]
del positions[-1][-1]

positions = np.array(positions)

x_max = np.max(positions[:, :, 0])
x_min = np.min(positions[:, :, 0])
y_max = np.max(positions[:, :, 1])
y_min = np.min(positions[:, :, 1])
z_max = np.max(positions[:, :, 2])
z_min = np.min(positions[:, :, 2])

normalized_masses = (masses - np.min(masses)) / (np.max(masses) - np.min(masses)) + 1

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
if color is not None:
    fig.colorbar(cm.ScalarMappable(cmap=mpl.colormaps['viridis']), ax=ax, label='Mass')

colormap = mpl.colormaps['viridis']


def update_graph(num):
    print(f"step: {num}", end="\r")
    ax.clear()
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    ax.set_title('N-body Simulation')
    ax.set_xlim(x_min, x_max)
    ax.set_ylim(y_min, y_max)
    ax.set_zlim(z_min, z_max)

    for i in range(len(masses)):
        if color is None:
            ax.scatter(positions[num][i][0], positions[num][i][1], positions[num][i][2], s=normalized_masses[i],
                       c='black')
        else:
            ax.scatter(positions[num][i][0], positions[num][i][1], positions[num][i][2],
                       color=colormap(normalized_masses[i]))
    return ax


ani = animation.FuncAnimation(fig, update_graph, frames=len(positions), interval=10)

ani.save('3d_scatter.gif', writer='imagemagick', fps=20)
print("\nsaved")
