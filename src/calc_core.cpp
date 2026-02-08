#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "../include/calc_core.hpp"

namespace calc_core {

namespace {

OperationType parseOperation(const char *op) {
    if (!op) return OperationType::None;
    if ((strcmp(op, "add") && strcmp(op, "+")) == 0) return OperationType::Add;
    if ((strcmp(op, "sub") && strcmp(op, "-")) == 0) return OperationType::Sub;
    if ((strcmp(op, "mul") && strcmp(op, "*")) == 0) return OperationType::Mul;
    if ((strcmp(op, "div") && strcmp(op, "/")) == 0) return OperationType::Div;
    if ((strcmp(op, "pow") && strcmp(op, "^")) == 0) return OperationType::Pow;
    if ((strcmp(op, "fact") && strcmp(op, "!")) == 0) return OperationType::Fact;

    return OperationType::None;
}

int parseInt64(const char *value, int64_t &out_value) {
    if (value == nullptr) {
        printf("Incorrect value: null pointer");
        return 1;
    }

    errno     = 0;
    char *end = nullptr;

    const long tmp = strtol(value, &end, 10);

    if (errno != 0) return 1;
    if (end == value || *end != '\0') return 1;

    out_value = static_cast<int64_t>(tmp);
    return 0;
}

}  //namespace

void printHelp(const char *prog) {
    printf("Usage:\n");
    printf("  %s -a A [-b B] -o OP\n", prog);
    printf("Options:\n");
    printf("  -h, --help           Show this help\n");
    printf("  -o, --op OP          Operation: add(+), sub(-), mul(*), div(/), pow(^), fact(!)\n");
    printf("  -a, --a A            First integer\n");
    printf("  -b, --b B            Second integer (required for binary ops)\n");
    printf("Examples:\n");
    printf("  %s -a 2 -b 3 -o +\n", prog);
    printf("  %s -a 5 --op fact\n", prog);
}

bool parseArgs(int argc, char *const argv[], CalcContext &calc_ctx) {
    static struct option long_options[] = {
        {"help",       no_argument, 0, 'h'},
        {   "a", required_argument, 0, 'a'},
        {   "b", required_argument, 0, 'b'},
        {  "op", required_argument, 0, 'o'},
        {     0,                 0, 0,   0}
    };

    int c;
    while ((c = getopt_long(argc, argv, "ha:b:o:", long_options, nullptr)) != -1) {
        switch (c) {
            case 'h': printHelp(argv[0]); exit(0);
            case 'a':
                if (parseInt64(optarg, calc_ctx.first) != 0) { return false; }
                calc_ctx.has_first = true;
                break;
            case 'b':
                if (parseInt64(optarg, calc_ctx.second) != 0) { return false; }
                calc_ctx.has_second = true;
                break;
            case 'o': calc_ctx.op = parseOperation(optarg); break;
            default: return false;
        }
    }

    if (!calc_ctx.has_first || calc_ctx.op == OperationType::None) { return false; }

    return true;
}

bool checkArgs(CalcContext &calc_ctx) {
    using namespace operations;

    if (calc_ctx.op != OperationType::Fact) {
        if (!calc_ctx.has_second) {
            calc_ctx.op_status = OperationalStatus::ValidationError;
            return false;
        }
        if (calc_ctx.op == OperationType::Div && calc_ctx.second == 0) {
            calc_ctx.op_status = OperationalStatus::DivisionByZero;
            return false;
        }
        if (calc_ctx.op == OperationType::Pow && calc_ctx.second < 0) {
            calc_ctx.op_status = OperationalStatus::ValidationError;
            return false;
        }
    }
    else {
        if (calc_ctx.first < 0) {
            calc_ctx.op_status = OperationalStatus::ValidationError;
            return false;
        }
    }

    return true;
}

bool calculate(CalcContext &calc_ctx) {
    calc_ctx.result_is_u64 = false;

    bool result = false;

    switch (calc_ctx.op) {
        case OperationType::Add: result = operations::add(calc_ctx.first, calc_ctx.second, calc_ctx.result_i64, calc_ctx.op_status); break;
        case OperationType::Sub: result = operations::sub(calc_ctx.first, calc_ctx.second, calc_ctx.result_i64, calc_ctx.op_status); break;
        case OperationType::Mul: result = operations::mul(calc_ctx.first, calc_ctx.second, calc_ctx.result_i64, calc_ctx.op_status); break;
        case OperationType::Div: result = operations::div(calc_ctx.first, calc_ctx.second, calc_ctx.result_i64, calc_ctx.op_status); break;
        case OperationType::Pow: result = operations::pow(calc_ctx.first, calc_ctx.second, calc_ctx.result_i64, calc_ctx.op_status); break;
        case OperationType::Fact:
            result                 = operations::fact(static_cast<uint64_t>(calc_ctx.first), calc_ctx.result_u64, calc_ctx.op_status);
            calc_ctx.result_is_u64 = true;
            break;
        default: calc_ctx.op_status = operations::OperationalStatus::UnsupportedOperation; return false;
    }

    return result;
}

void printResult(const CalcContext &calc_ctx) {
    if (calc_ctx.op_status != operations::OperationalStatus::AllRight) {
        fprintf(stderr, "Error: %d\n", (int)calc_ctx.op_status);
        return;
    }

    if (calc_ctx.result_is_u64) { printf("Result: %llu\n", (unsigned long long)calc_ctx.result_u64); }
    else                        { printf("Result: %lld\n", (long long)calc_ctx.result_i64); }
}

int run(int argc, char* const argv[]) {
    CalcContext calc_ctx;
    memset(&calc_ctx, 0, sizeof(calc_ctx));
    calc_ctx.op = OperationType::None;
    calc_ctx.op_status = operations::OperationalStatus::AllRight;

    if (!parseArgs(argc, argv, calc_ctx)) { printResult(calc_ctx); return 1; }
    if (!checkArgs(calc_ctx))             { printResult(calc_ctx); return 1; }
    if (!calculate(calc_ctx))             { printResult(calc_ctx); return 1; }

    printResult(calc_ctx);
    return 0;
}

}  //namespace calc_core
