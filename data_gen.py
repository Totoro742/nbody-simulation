import sys
import random

n = len(sys.argv)

if n != 9:
    print("Usage: python data_gen.py n_bodies max_x max_y max_z min_mass max_mass max_velocity_component file_name")
    sys.exit(1)

n_bodies = int(sys.argv[1])
max_x = int(sys.argv[2])
max_y = int(sys.argv[3])
max_z = int(sys.argv[4])
min_mass = int(sys.argv[5])
max_mass = int(sys.argv[6])
max_velocity = int(sys.argv[7])
filename = sys.argv[8]

bodies = []
for i in range(n_bodies):
    x = random.randint(0, max_x)
    y = random.randint(0, max_y)
    z = random.randint(0, max_z)
    mass = random.randint(min_mass, max_mass)
    v = (random.randint(-max_velocity, max_velocity),
         random.randint(-max_velocity, max_velocity),
         random.randint(-max_velocity, max_velocity))

    bodies.append((x, y, z, mass, v))

with open(filename, 'w') as f:
    f.write(str(n_bodies) + '\n')
    for body in bodies:
        f.write(str(body[0]) + ',' + str(body[1]) + ',' + str(body[2]) + ',' + str(body[3]) + ',' + str(body[4]) + '\n')
