import sys
import random

n = len(sys.argv)

if n != 9:
    print("Usage: python data_gen.py n_bodies min_mass max_mass max_x max_y max_z max_velocity_component file_name")
    sys.exit(1)

n_bodies = int(sys.argv[1])
min_mass = int(sys.argv[2])
max_mass = int(sys.argv[3])
max_x = int(sys.argv[4])
max_y = int(sys.argv[5])
max_z = int(sys.argv[6])
max_velocity = int(sys.argv[7])
filename = sys.argv[8]

with open(filename, 'w') as f:
    for i in range(n_bodies):
        mass = random.randint(min_mass, max_mass)
        x = random.randint(0, max_x)
        y = random.randint(0, max_y)
        z = random.randint(0, max_z)
        v = (random.randint(-max_velocity, max_velocity),
             random.randint(-max_velocity, max_velocity),
             random.randint(-max_velocity, max_velocity))

        f.write(f'{mass},{x},{y},{z},{v[0]},{v[1]},{v[2]}\n')
