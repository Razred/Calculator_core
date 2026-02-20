#pragma once

#include "model.hpp"

class Calculator {
public:
    Response calculate(const Request &request);

private:
    Response add(int64_t a, int64_t b);
    Response sub(int64_t a, int64_t b);
    Response mul(int64_t a, int64_t b);
    Response div(int64_t a, int64_t b);
    Response pow(int64_t a, int64_t b);
    Response fact(uint64_t a);
};