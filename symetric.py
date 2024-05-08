import math

with open("symetric.csv", "w") as file:
    mass = 1000
    z = 0
    vz = 0
    vscaler = 0.0003
    for n in range(10):
        phi = n/10 * 2 * math.pi
        x = math.cos(phi)
        y = math.sin(phi)
        vx = -y * vscaler
        vy = x * vscaler

        file.write(f"{x},{y},{z},{mass},({vx}, {vy}, {vz})\n")
