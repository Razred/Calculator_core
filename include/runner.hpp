#pragma once
#include "parser.hpp"
#include "checker.hpp"
#include "calculator.hpp"
#include "printer.hpp"

#include <iostream>

class Runner {
public:
    int run(int argc, const char* argv[], std::ostream &out = std::cout);

private:
    Parser parser;
    Checker checker;
    Calculator calculator;
    Printer printer;
};