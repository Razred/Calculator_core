#include <string>

#include "parser.hpp"
#include "checker.hpp"
#include "calculator.hpp"
#include "printer.hpp"


class Runner {
public:
    int run(const std::string &json_path, std::ostream &out, std::ostream &err);

private:
    Parser parser;
    Checker checker;
    Calculator calculator;
    Printer printer;
};