#include "../include/runner.hpp"
#include "iostream"

int main(int argc, char *const argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./calc input.json\n";
        return 1;
    }

    Runner runner;
    return runner.run(argv[1]);
}