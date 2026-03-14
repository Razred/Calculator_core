#include "../include/printer.hpp"
#include "../include/errors.hpp"

void Printer::print(const Request &request, const Response &response, std::ostream &out) {
    if (!request.has_second) { 
        out << request.first << request.op_str << " = " << response.result_i64 << std::endl;
        return;
    }

    out << request.first << " " <<  request.op_str << " " << request.second << " = " << response.result_i64 << std::endl;
}