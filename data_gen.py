import sys
import random

n = len(sys.argv)

if n != 7:
    print("Usage: python data_gen.py n_bodies max_x max_y min_mass max_mass file_name")
    sys.exit(1)

n_bodies = int(sys.argv[1])
max_x = int(sys.argv[2])
max_y = int(sys.argv[3])
min_mass = int(sys.argv[4])
max_mass = int(sys.argv[5])
filename = sys.argv[6]

bodies = []
for i in range(n_bodies):
    x = random.randint(0, max_x)
    y = random.randint(0, max_y)
    mass = random.randint(min_mass, max_mass)
    bodies.append((x, y, mass))

with open(filename, 'w') as f:
    f.write(str(n_bodies) + '\n')
    for body in bodies:
        f.write(str(body[0]) + ',' + str(body[1]) + ',' + str(body[2]) + '\n')