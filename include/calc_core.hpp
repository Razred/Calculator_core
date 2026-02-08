#pragma once

#include <stdint.h>

#include "operations.hpp"

namespace calc_core {

enum class OperationType { None = 0, Add, Sub, Mul, Div, Pow, Fact };

struct CalcContext {
    int64_t first;
    int64_t second;
    bool    has_first;
    bool    has_second;

    OperationType op;

    operations::OperationalStatus op_status;

    int64_t  result_i64;
    uint64_t result_u64;
    bool     result_is_u64;
};

int run(int argc, char *const argv[]);

void printHelp(const char *prog);

bool parseArgs(int argc, char *const argv[], CalcContext &calc_ctx);

bool checkArgs(CalcContext &calc_ctx);

bool calculate(CalcContext &calc_ctx);

void printResult(const CalcContext &calc_ctx);

}  //namespace calc_core