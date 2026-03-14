#include "runner.hpp"

#include <gtest/gtest.h>
#include <sstream>
#include <fstream>
#include <array>

TEST(RunnerTest, AddCheck) {
    const std::string filename = "test_add.json";
    std::ofstream     file(filename);
    file << R"({
        "first": 2,
        "second": 3,
        "op": "add"
    })";
    file.close();

    std::array<const char*, 2> argv = {"./calc", filename.c_str()};
    int argc = static_cast<int>(argv.size());   

    Runner runner;

    std::ostringstream out;

    int code = runner.run(argc, argv.data(), out);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out.str(), "2 + 3 = 5\n");
}

TEST(RunnerTest, SubCheck) {
    const std::string filename = "test_sub.json";
    std::ofstream     file(filename);
    file << R"({
        "first": 3,
        "second": 2,
        "op": "sub"
    })";
    file.close();

    std::array<const char*, 2> argv = {"./calc", filename.c_str()};
    int argc = static_cast<int>(argv.size());   

    Runner runner;

    std::ostringstream out;

    int code = runner.run(argc, argv.data(), out);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out.str(), "3 - 2 = 1\n");
}

TEST(RunnerTest, MulCheck) {
    const std::string filename = "test_mul.json";
    std::ofstream     file(filename);
    file << R"({
        "first": 4,
        "second": 5,
        "op": "mul"
    })";
    file.close();

    std::array<const char*, 2> argv = {"./calc", filename.c_str()};
    int argc = static_cast<int>(argv.size());   

    Runner runner;

    std::ostringstream out;

    int code = runner.run(argc, argv.data(), out);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out.str(), "4 * 5 = 20\n");
}

TEST(RunnerTest, DivCheck) {
    const std::string filename = "test_div.json";
    std::ofstream     file(filename);
    file << R"({
        "first": 20,
        "second": 4,
        "op": "div"
    })";
    file.close();

    std::array<const char*, 2> argv = {"./calc", filename.c_str()};
    int argc = static_cast<int>(argv.size());   

    Runner runner;

    std::ostringstream out;

    int code = runner.run(argc, argv.data(), out);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out.str(), "20 / 4 = 5\n");
}

TEST(RunnerTest, PowCheck) {
    const std::string filename = "test_pow.json";
    std::ofstream     file(filename);
    file << R"({
        "first": 2,
        "second": 3,
        "op": "pow"
    })";
    file.close();

    std::array<const char*, 2> argv = {"./calc", filename.c_str()};
    int argc = static_cast<int>(argv.size());   

    Runner runner;

    std::ostringstream out;

    int code = runner.run(argc, argv.data(), out);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out.str(), "2 ^ 3 = 8\n");
}


TEST(RunnerTest, FactorialCheck) {
    const std::string filename = "test_fact.json";
    std::ofstream     file(filename);
    file << R"({
        "first": 5,
        "op": "fact"
    })";
    file.close();

    std::array<const char*, 2> argv = {"./calc", filename.c_str()};
    int argc = static_cast<int>(argv.size());   

    Runner runner;

    std::ostringstream out;

    int code = runner.run(argc, argv.data(), out);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out.str(), "5! = 120\n");
}

TEST(RunnerTest, DivisionByZero) {
    const std::string filename = "test_div0.json";
    std::ofstream file(filename);
    file << R"({
        "first": 4,
        "second": 0,
        "op": "div"
    })";
    file.close();

    std::array<const char*, 2> argv = {"./calc", filename.c_str()};
    int argc = static_cast<int>(argv.size());

    Runner runner;
    std::ostringstream out;

    try {
        runner.run(argc, argv.data(), out);
        FAIL() << "Expected DivisionByZero exception";
    }
    catch (const Error& e) {
        EXPECT_EQ(std::string(e.what()), "Division by zero");
    }
}

TEST(RunnerTest, CheckNegativePow) {
    const std::string filename = "test_neg_pow.json";
    std::ofstream     file(filename);
    file << R"({
        "first": 2,
        "second": -1,
        "op": "pow"
    })";
    file.close();

    std::array<const char*, 2> argv = {"./calc", filename.c_str()};
    int argc = static_cast<int>(argv.size());   

    Runner runner;

    std::ostringstream out;

    try {
        runner.run(argc, argv.data(), out);
        FAIL() << "Expected Negative Pow exception";
    }
    catch (const Error& e) {
        EXPECT_EQ(std::string(e.what()), "Cannot pow by negative exponent");
    }
}

TEST(RunnerTest, CheckNegativeFactorial) {
    const std::string filename = "test_neg_fact.json";
    std::ofstream     file(filename);
    file << R"({
        "first": -5,
        "op": "fact"
    })";
    file.close();

    std::array<const char*, 2> argv = {"./calc", filename.c_str()};
    int argc = static_cast<int>(argv.size());   

    Runner runner;

    std::ostringstream out;

    try {
        runner.run(argc, argv.data(), out);
        FAIL() << "Expected Negative factorial exception";
    }
    catch (const Error& e) {
        EXPECT_EQ(std::string(e.what()), "Value 'first' must be >= 0 for factorial");
    }
}

TEST(RunnerTest, MissingSecondParameter) {
    const std::string filename = "test_missing_second.json";
    std::ofstream     file(filename);
    file << R"({
        "first": 5,
        "op": "add"
    })";
    file.close();

    std::array<const char*, 2> argv = {"./calc", filename.c_str()};
    int argc = static_cast<int>(argv.size());   

    Runner runner;

    std::ostringstream out;

    try {
        runner.run(argc, argv.data(), out);
        FAIL() << "Expected Missing second parameter exception";
    }
    catch (const Error& e) {
        EXPECT_EQ(std::string(e.what()), "Cannot calculate because 'second' parameter is missed for binary operation");
    }
}

TEST(RunnerTest, InvalidOperation) {
    const std::string filename = "test_invalid_op.json";
    std::ofstream     file(filename);
    file << R"({
        "first": 5,
        "second": 3,
        "op": "unknown"
    })";
    file.close();

    std::array<const char*, 2> argv = {"./calc", filename.c_str()};
    int argc = static_cast<int>(argv.size());   

    Runner runner;

    std::ostringstream out;

    try {
        runner.run(argc, argv.data(), out);
        FAIL() << "Expected Invalid operation exception";
    }
    catch (const Error& e) {
        EXPECT_EQ(std::string(e.what()), "Not expected value unknown for key 'op'");
    }
}