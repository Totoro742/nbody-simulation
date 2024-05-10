import sys
import random

n = len(sys.argv)

if n != 9:
    print("Usage: python data_gen.py n_bodies min_mass max_mass max_x max_y max_z max_velocity_component file_name")
    sys.exit(1)

n_bodies = int(sys.argv[1])
min_mass = float(sys.argv[2])
max_mass = float(sys.argv[3])
max_x = float(sys.argv[4])
max_y = float(sys.argv[5])
max_z = float(sys.argv[6])
max_velocity = float(sys.argv[7])
filename = sys.argv[8]

with open(filename, 'w') as f:
    for i in range(n_bodies):
        mass = random.uniform(min_mass, max_mass)
        x = random.uniform(0, max_x)
        y = random.uniform(0, max_y)
        z = random.uniform(0, max_z)
        v = (random.uniform(-max_velocity, max_velocity),
             random.uniform(-max_velocity, max_velocity),
             random.uniform(-max_velocity, max_velocity))

        f.write(f'{mass},{x},{y},{z},{v[0]},{v[1]},{v[2]}\n')
