#pragma once

#include <vector>

namespace node
{
struct NodeConfig {
    int totalNodes{0};
    int nodeRank{0};
    int totalParticles{0};
    int localParticles{0};
    std::vector<int> particlesPerNode{};
};
} // namespace node
