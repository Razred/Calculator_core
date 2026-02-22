#include "../include/printer.hpp"
#include "../include/errors.hpp"

#include <iostream>

void Printer::print(const Request &request, const Response &response, std::ostream &out) {
    switch (request.op) {
        case OperationType::Add: out << request.first << " + " << request.second << " = " << response.result_i64 << std::endl; return;
        case OperationType::Sub: out << request.first << " - " << request.second << " = " << response.result_i64 << std::endl; return;
        case OperationType::Mul: out << request.first << " * " << request.second << " = " << response.result_i64 << std::endl; return;
        case OperationType::Div: out << request.first << " / " << request.second << " = " << response.result_i64 << std::endl; return;
        case OperationType::Pow: out << request.first << " ^ " << request.second << " = " << response.result_i64 << std::endl; return;
        case OperationType::Fact:
            out << request.first << "!"
                      << " = " << response.result_u64 << std::endl;
            return;
        default: throw Error("Incorect operation type");
    }
}