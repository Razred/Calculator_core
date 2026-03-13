#pragma once
#include "result.hpp"

namespace postgres {

class Connection {
public:
    explicit Connection(const std::string& conninfo);
    Result execute(const std::string& sql) { return Result(PQexec(conn.get(), sql.c_str())); }

    Result executeWithParams(const std::string& sql, int n_params,  const char **params) { 
        return Result(PQexecParams(conn.get(), sql.c_str(), n_params, nullptr, params, nullptr, nullptr, 0)); 
    }

private:
    std::unique_ptr<PGconn, decltype(&PQfinish)> conn;
};

inline Connection::Connection(const std::string &conninfo) : conn(PQconnectdb(conninfo.c_str()), &PQfinish) {
    auto status = PQstatus(conn.get());

    if (status != CONNECTION_OK) {
        throw Error(PQerrorMessage(conn.get()));
    }

}

}