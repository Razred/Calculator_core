#include "../include/postgres_db.hpp"
#include "postgres_db.hpp"

namespace postgres {

std::string PostgresDB::makeKey(int64_t a, std::optional<int64_t> b, char op) { 
    if (!b.has_value()) return std::to_string(a) + op;
    if (op == '+' || op == '*') return std::to_string(std::min(a, b.value())) + op + std::to_string(std::max(a, b.value()));

    return std::to_string(a) + op + std::to_string(b.value());
}

void PostgresDB::initTable() {
    std::string sql = R"(
        CREATE TABLE IF NOT EXISTS operations (
            id        SERIAL PRIMARY KEY,
            first     BIGINT NOT NULL,
            second    BIGINT,
            operation CHAR(1) NOT NULL,
            result    BIGINT,
            status    INTEGER NOT NULL DEFAULT 0
        );
    )";

    connection.execute(sql);
}

std::optional<OperationRecord> PostgresDB::findInCache(int64_t first, std::optional<int64_t> second, char op) const {
    std::string key = makeKey(first, second, op);

    auto it = cache.find(key);
    if (it != cache.end()) return it->second;
    return std::nullopt; 
}

void PostgresDB::saveOperation(int64_t first, std::optional<int64_t> second, char op, std::optional<int64_t> res, int status) {
    std::string sql = R"(
        INSERT INTO operations (first, second, operation, result, status)
        VALUES ($1, $2, $3, $4, $5)
    )";
    
    std::string first_str = std::to_string(first);
    std::optional<std::string> second_str = (second.has_value()) ? std::optional<std::string>(std::to_string(second.value())) : std::nullopt;
    std::string op_str{op};
    std::optional<std::string> res_str = (res.has_value()) ? std::optional<std::string>(std::to_string(res.value())) : std::nullopt;
    std::string status_str = std::to_string(status);

    const char *paramValues[5] = {
        first_str.c_str(), 
        second_str.has_value()   ? second_str->c_str()   : nullptr,
        op_str.c_str(), 
        res_str.has_value() ? res_str->c_str() : nullptr,
        status_str.c_str()
    };

    connection.executeWithParams(sql, 5, paramValues);

    std::string key = makeKey(first, second, op);
    cache[key] = {first, second, op, res, status};

    sleep(3);
}

void PostgresDB::warmUpCache() {
    std::string sql = R"(SELECT first, second, operation, result, status FROM operations)";

    Result result(connection.execute(sql));

    for (size_t i = 0; i < result.rows(); ++i) {
        int64_t first = std::stoll(result.get(i, 0));
        std::optional<int64_t> second = (result.isNull(i, 1)) ? std::nullopt : std::optional<int64_t>(std::stoll(result.get(i, 1)));
        char op = result.get(i, 2)[0];
        std::optional<int64_t> res = (result.isNull(i, 3)) ? std::nullopt : std::optional<int64_t>(std::stoll(result.get(i, 3)));
        int status = std::stoi(result.get(i, 4));

        std::string key = makeKey(first, second, op);
        cache[key] = {first, second, op, res, status};
    }
}

}