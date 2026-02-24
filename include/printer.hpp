#pragma once
#include "model.hpp"

#include <ostream>

class Printer {
public:
    void print(const Request &request, const Response &response, std::ostream &out);
};