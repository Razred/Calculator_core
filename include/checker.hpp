#pragma once
#include "model.hpp"

class Checker {
public:
    void checkInputArgs(int argc, const char* argv[]) const;
    void checkRequest(const Request &request) const;
};