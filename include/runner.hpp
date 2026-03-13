#pragma once
#include "parser.hpp"
#include "checker.hpp"
#include "calculator.hpp"
#include "printer.hpp"
#include "postgres_db.hpp"

#include <iostream>

class Runner {
public:
    Runner() : database(getConnStr()) {}
    int run(int argc, const char* argv[], std::ostream &out = std::cout);

private:
    Parser parser;
    Checker checker;
    Calculator calculator;
    Printer printer;

    postgres::PostgresDB database;

    static std::string getConnStr() {
        const char* conn = std::getenv("POSTGRES_CONN_STR");

        if (!conn) {
            throw std::runtime_error("POSTGRES_CONN_STR is not set");
        }

        return std::string(conn);
    }
};