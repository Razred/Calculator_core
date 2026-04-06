#include "runner.hpp"
#include "config.hpp"

#include <gtest/gtest.h>
#include <sstream>
#include <nlohmann/json.hpp>

TEST(RunnerTest, AddCheck) {
    DbConfig config{};
    Runner runner(config);

    nlohmann::json req = {
        {"first", 2},
        {"second", 3},
        {"op", "add"}
    };

    nlohmann::json recv;
    std::ostringstream out;

    int code = runner.run(req, recv, out);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out.str(), "2 + 3 = 5\n");
    EXPECT_EQ(recv["status"], 0);
    EXPECT_EQ(recv["result"], 5);
}

TEST(RunnerTest, SubCheck) {
    DbConfig config{};
    Runner runner(config);

    nlohmann::json req = {
        {"first", 3},
        {"second", 2},
        {"op", "sub"}
    };

    nlohmann::json recv;
    std::ostringstream out;

    int code = runner.run(req, recv, out);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out.str(), "3 - 2 = 1\n");
    EXPECT_EQ(recv["status"], 0);
    EXPECT_EQ(recv["result"], 1);
}

TEST(RunnerTest, MulCheck) {
    DbConfig config{};
    Runner runner(config);

    nlohmann::json req = {
        {"first", 4},
        {"second", 5},
        {"op", "mul"}
    };

    nlohmann::json recv;
    std::ostringstream out;

    int code = runner.run(req, recv, out);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out.str(), "4 * 5 = 20\n");
    EXPECT_EQ(recv["status"], 0);
    EXPECT_EQ(recv["result"], 20);
}

TEST(RunnerTest, DivCheck) {
    DbConfig config{};
    Runner runner(config);

    nlohmann::json req = {
        {"first", 20},
        {"second", 4},
        {"op", "div"}
    };

    nlohmann::json recv;
    std::ostringstream out;

    int code = runner.run(req, recv, out);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out.str(), "20 / 4 = 5\n");
    EXPECT_EQ(recv["status"], 0);
    EXPECT_EQ(recv["result"], 5);
}

TEST(RunnerTest, PowCheck) {
    DbConfig config{};
    Runner runner(config);

    nlohmann::json req = {
        {"first", 2},
        {"second", 3},
        {"op", "pow"}
    };

    nlohmann::json recv;
    std::ostringstream out;

    int code = runner.run(req, recv, out);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out.str(), "2 ^ 3 = 8\n");
    EXPECT_EQ(recv["status"], 0);
    EXPECT_EQ(recv["result"], 8);
}

TEST(RunnerTest, FactorialCheck) {
    DbConfig config{};
    Runner runner(config);

    nlohmann::json req = {
        {"first", 5},
        {"op", "fact"}
    };

    nlohmann::json recv;
    std::ostringstream out;

    int code = runner.run(req, recv, out);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out.str(), "5! = 120\n");
    EXPECT_EQ(recv["status"], 0);
    EXPECT_EQ(recv["result"], 120);
}

TEST(RunnerTest, DivisionByZero) {
    DbConfig config{};
    Runner runner(config);

    nlohmann::json req = {
        {"first", 4},
        {"second", 0},
        {"op", "div"}
    };

    nlohmann::json recv;
    std::ostringstream out;

    try {
        runner.run(req, recv, out);
        FAIL() << "Expected DivisionByZero exception";
    } catch (const Error& e) {
        EXPECT_EQ(std::string(e.what()), "Division by zero");
    }
}

TEST(RunnerTest, CheckNegativePow) {
    DbConfig config{};
    Runner runner(config);

    nlohmann::json req = {
        {"first", 2},
        {"second", -1},
        {"op", "pow"}
    };

    nlohmann::json recv;
    std::ostringstream out;

    try {
        runner.run(req, recv, out);
        FAIL() << "Expected Negative Pow exception";
    } catch (const Error& e) {
        EXPECT_EQ(std::string(e.what()), "Cannot pow by negative exponent");
    }
}

TEST(RunnerTest, CheckNegativeFactorial) {
    DbConfig config{};
    Runner runner(config);

    nlohmann::json req = {
        {"first", -5},
        {"op", "fact"}
    };

    nlohmann::json recv;
    std::ostringstream out;

    try {
        runner.run(req, recv, out);
        FAIL() << "Expected Negative factorial exception";
    } catch (const Error& e) {
        EXPECT_EQ(std::string(e.what()), "Value 'first' must be >= 0 for factorial");
    }
}

TEST(RunnerTest, MissingSecondParameter) {
    DbConfig config{};
    Runner runner(config);

    nlohmann::json req = {
        {"first", 5},
        {"op", "add"}
    };

    nlohmann::json recv;
    std::ostringstream out;

    try {
        runner.run(req, recv, out);
        FAIL() << "Expected Missing second parameter exception";
    } catch (const Error& e) {
        EXPECT_EQ(std::string(e.what()),
                  "Cannot calculate because 'second' parameter is missed for binary operation");
    }
}

TEST(RunnerTest, InvalidOperation) {
    DbConfig config{};
    Runner runner(config);

    nlohmann::json req = {
        {"first", 5},
        {"second", 3},
        {"op", "unknown"}
    };

    nlohmann::json recv;
    std::ostringstream out;

    try {
        runner.run(req, recv, out);
        FAIL() << "Expected Invalid operation exception";
    } catch (const Error& e) {
        EXPECT_EQ(std::string(e.what()), "Not expected value unknown for key 'op'");
    }
}