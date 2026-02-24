#pragma once
#include <cstdint>
#include <string>

enum class OperationType { None = 0, Add, Sub, Mul, Div, Pow, Fact };

struct Request {
    int64_t       first{};
    int64_t       second{};
    bool          has_second{false};
    OperationType op{};
    std::string op_str{};
};

struct Response {
    int64_t  result_i64{};
    uint64_t result_u64{};
    bool     result_is_u64{false};
};
