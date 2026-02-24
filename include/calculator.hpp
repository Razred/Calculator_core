#pragma once
#include "model.hpp"

class Calculator {
public:
    Response calculate(const Request &request);
};