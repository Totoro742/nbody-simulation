#include "constants.hpp"
#include "node/master.hpp"
#include "node/worker.hpp"
#include <cstdio>
#include <exception>
#include <string>
#include <upcxx/upcxx.hpp>
#include <vector>

int main(int argc, char** argv)
{
    upcxx::init();

    try {
        const int nodeRank{upcxx::rank_me()};
        if (nodeRank == masterNodeRank) {
            const std::vector<std::string> args{argv, argv + argc};
            node::master::run(args);
        } else {
            node::worker::run();
        }
    } catch (const std::exception& err) {
        std::fprintf(stderr, "handling exception in main:\n  %s\n", err.what());
    } catch (...) {
        std::fprintf(stderr, "unknown exception\n");
    }

    upcxx::finalize();
    return 0;
}
