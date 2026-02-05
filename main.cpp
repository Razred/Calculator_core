#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "operations.hpp"

static void printHelp(const char* prog) {
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

static int parseInt(const char *value, int32_t &out) {
    if (value == nullptr) {
        printf("Incorrect value: null pointer");
        return 1;
    }

    errno = 0;
    char* end = nullptr;

    const long tmp = strtol(value, &end, 10);

    if (end == value || *end != '\0') {
        printf("Incorrect value: not a number (%s)", value);
        return 1;
    }

    if (tmp < INT32_MIN || tmp > INT32_MAX) {
        printf("Incorrect value: out of range (%s)", value);
        return 1;
    }

    out = static_cast<int32_t>(tmp);
    return 0;
}




int main(int argc, char * const argv[]) {
    const char *op = nullptr;
    int32_t a = 0, b = 0;
    bool has_a = false, has_b = false;

    static struct option long_options[] = {
        { "help", no_argument, 0, 'h' },
        { "a", required_argument, 0, 'a' },
        { "b", required_argument, 0, 'b' },
        { "op", required_argument, 0, 'o' },
        { 0, 0, 0, 0 }
    };

    int c;
    while ((c = getopt_long(argc, argv, "ha:b:o:", long_options, nullptr)) != -1) {
        switch (c) {
            case 'h':
                printHelp(argv[0]);
                return 0; 
            case 'a':
                if (parseInt(optarg, a) != 0) return 1;
                has_a = true;
                break;
            case 'b':
                if (parseInt(optarg, b) != 0) return 1;
                has_b = true;
                break;
            case 'o':
                op = optarg;
                break;
            case '?':
            default:
                return 1;
        }
    }

    if (!op) {
        fprintf(stderr, "Missing operation (-o/ --op)\n");
        return 1;
    }

    if (!has_a) {
        fprintf(stderr, "Missing -a\n");
        return 1;
    }

    int is_fact = ((strcmp(op, "fact") && strcmp(op, "!"))  == 0);
    if (!is_fact && !has_b) {
        fprintf(stderr, "Operation '%s' requires -b\n", op);
        return 1;
    }

    int32_t result = 0;

    if ((strcmp(op, "add") && strcmp(op, "+")) == 0) {
        result = add(a, b);
    }
    else if ((strcmp(op, "sub") && strcmp(op, "-")) == 0) {
        result = sub(a, b);
    }
    else if ((strcmp(op, "mul") && strcmp(op, "*")) == 0) {
        result = mul(a, b);
    }
    else if ((strcmp(op, "del") && strcmp(op, "/")) == 0) {
        result = del(a, b);
    }
    else if ((strcmp(op, "pow") && strcmp(op, "^")) == 0) {
        result = pow(a, b);
    }
    else if ((strcmp(op, "fact") && strcmp(op, "!")) == 0) {
        result = fact(a);
    }
    else {
         fprintf(stderr, "Operation '%s' don't support\n", op);
         return 0;
    }
    
    printf("Result of operation: %i %s %i = %i\n", a, op, b, result);

    return 0;
}