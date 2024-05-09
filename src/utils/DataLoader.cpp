#include "DataLoader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

namespace utils
{
ParticlesData loadParticlesData(const std::string& fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << fileName << std::endl;
        return {};
    }

    ParticlesData particles;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        float x, y, z, mass, v_x, v_y, v_z;
        char ch; // to read the commas and parentheses
        if (!(iss >> mass >> ch >> x >> ch >> y >> ch >> z >> ch >> v_x >> ch >> v_y >> ch >> v_z)) {
            std::cerr << "Error while reading file: " << fileName << std::endl;
            return {};
        }
        particles.positions.push_back({x, y, z});
        particles.masses.push_back(mass);
        particles.velocities.push_back({v_x, v_y, v_z});
    }

    return particles;
}
} // namespace utils
