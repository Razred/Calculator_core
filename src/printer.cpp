#include "../include/printer.hpp"
#include "../include/errors.hpp"

#include <iostream>

void Printer::print(const Request &request, const Response &response) {
    switch (request.op) {
        case OperationType::Add: std::cout << request.first << " + " << request.second << " = " << response.result_i64 << std::endl; return;
        case OperationType::Sub: std::cout << request.first << " - " << request.second << " = " << response.result_i64 << std::endl; return;
        case OperationType::Mul: std::cout << request.first << " * " << request.second << " = " << response.result_i64 << std::endl; return;
        case OperationType::Div: std::cout << request.first << " / " << request.second << " = " << response.result_i64 << std::endl; return;
        case OperationType::Pow: std::cout << request.first << " ^ " << request.second << " = " << response.result_i64 << std::endl; return;
        case OperationType::Fact:
            std::cout << request.first << "!"
                      << " = " << response.result_u64 << std::endl;
            return;
        default: throw Error("Incorect operation type");
    }
}