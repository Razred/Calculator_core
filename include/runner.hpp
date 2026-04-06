#pragma once
#include "parser.hpp"
#include "checker.hpp"
#include "calculator.hpp"
#include "printer.hpp"
#include "postgres_db.hpp"
#include "config.hpp"

#include <iostream>

class Runner {
public:
    explicit Runner(const DbConfig& config)
        : database(config.conn_str) {}

    int run(const nlohmann::json &req, nlohmann::json &recv, std::ostream &out = std::cout);

private:
    Parser parser;
    Checker checker;
    Calculator calculator;
    Printer printer;

    postgres::PostgresDB database;
};