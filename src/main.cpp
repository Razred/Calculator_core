#include "../include/runner.hpp"

int main(int argc, const char* argv[]) {
    try {
        Runner runner;
        return runner.run(argc, argv);
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}