#pragma once
#include "connection.hpp"
#include <unordered_map>
#include <string>
#include <optional>

namespace postgres {

struct OperationRecord {
    int64_t first;
    std::optional<int64_t> second;
    char    operation;
    std::optional<int64_t> result;
    int     status;
};

class PostgresDB {
public:
    explicit PostgresDB(const std::string& conn_str)
        : connection(conn_str)
    {
        initTable();
        warmUpCache();
    }

    std::optional<OperationRecord> findInCache(int64_t first, std::optional<int64_t> second, char op) const;

    void saveOperation(int64_t first, std::optional<int64_t> second, char op, std::optional<int64_t> res, int status);
    void warmUpCache();

private:
    Connection connection;
    std::unordered_map<std::string, OperationRecord> cache;

    void initTable();

    static std::string makeKey(int64_t first, std::optional<int64_t> second, char op);
};

}