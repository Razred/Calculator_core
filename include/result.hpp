#pragma once
#include "errors.hpp"

#include <postgresql/libpq-fe.h>
#include <memory>

namespace postgres {

class Result {
public:
    Result(PGresult* result);

    int rows() const { return PQntuples(pg_result.get()); }
    int columns() const { return PQnfields(pg_result.get()); }
    std::string get(int row, int col) const { return std::string(PQgetvalue(pg_result.get(), row, col)); }

    bool isNull(int row, int col) const { return PQgetisnull(pg_result.get(), row, col); }

private:
    std::unique_ptr<PGresult, decltype(&PQclear)> pg_result;
};

inline Result::Result(PGresult *result) : pg_result(result, &PQclear) {
    if (pg_result == nullptr) {
        throw ("Postgresql result is nullptr");
    }

    auto status = PQresultStatus(pg_result.get());

    if (status != ExecStatusType::PGRES_COMMAND_OK && status != ExecStatusType::PGRES_TUPLES_OK) {
        throw Error(PQresultErrorMessage(pg_result.get()));
    }
}

}