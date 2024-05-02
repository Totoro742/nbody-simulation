#pragma once

#include "ParticlesData.hpp"
#include <string>
#include <vector>

namespace utils
{
ParticlesData loadParticlesData(const std::string& fileName);
}