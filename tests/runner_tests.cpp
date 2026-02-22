#include <gtest/gtest.h>
#include <sstream>
#include <fstream>

#include "runner.hpp"

TEST(RunnerTest, AddCheck) {
    const std::string filename = "test_add.json";
    std::ofstream     file(filename);
    file << R"({
        "first": 2,
        "second": 3,
        "op": "add"
    })";
    file.close();

    Runner runner;

    std::ostringstream out;
    std::ostringstream err;

    int code = runner.run(filename, out, err);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out.str(), "2 + 3 = 5\n");
    EXPECT_TRUE(err.str().empty());
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

    Runner             runner;
    std::ostringstream out, err;

    int code = runner.run(filename, out, err);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out.str(), "3 - 2 = 1\n");
    EXPECT_TRUE(err.str().empty());
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

    Runner             runner;
    std::ostringstream out, err;

    int code = runner.run(filename, out, err);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out.str(), "4 * 5 = 20\n");
    EXPECT_TRUE(err.str().empty());
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

    Runner             runner;
    std::ostringstream out, err;

    int code = runner.run(filename, out, err);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out.str(), "20 / 4 = 5\n");
    EXPECT_TRUE(err.str().empty());
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

    Runner             runner;
    std::ostringstream out, err;

    int code = runner.run(filename, out, err);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out.str(), "2 ^ 3 = 8\n");
    EXPECT_TRUE(err.str().empty());
}


TEST(RunnerTest, FactorialCheck) {
    const std::string filename = "test_fact.json";
    std::ofstream     file(filename);
    file << R"({
        "first": 5,
        "op": "fact"
    })";
    file.close();

    Runner runner;

    std::ostringstream out;
    std::ostringstream err;

    int code = runner.run(filename, out, err);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out.str(), "5! = 120\n");
    EXPECT_TRUE(err.str().empty());
}

TEST(RunnerTest, DivisionByZero) {
    const std::string filename = "test_div0.json";
    std::ofstream     file(filename);
    file << R"({
        "first": 4,
        "second": 0,
        "op": "div"
    })";
    file.close();

    Runner runner;

    std::ostringstream out;
    std::ostringstream err;

    int code = runner.run(filename, out, err);

    EXPECT_NE(code, 0);
    EXPECT_TRUE(out.str().empty());
    EXPECT_FALSE(err.str().empty());
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

    Runner             runner;
    std::ostringstream out, err;

    int code = runner.run(filename, out, err);

    EXPECT_NE(code, 0);
    EXPECT_TRUE(out.str().empty());
    EXPECT_FALSE(err.str().empty());
}

TEST(RunnerTest, CheckNegativeFactorial) {
    const std::string filename = "test_neg_fact.json";
    std::ofstream     file(filename);
    file << R"({
        "first": -5,
        "op": "fact"
    })";
    file.close();

    Runner             runner;
    std::ostringstream out, err;

    int code = runner.run(filename, out, err);

    EXPECT_NE(code, 0);
    EXPECT_TRUE(out.str().empty());
    EXPECT_FALSE(err.str().empty());
}

TEST(RunnerTest, MissingSecondParameter) {
    const std::string filename = "test_missing_second.json";
    std::ofstream     file(filename);
    file << R"({
        "first": 5,
        "op": "add"
    })";
    file.close();

    Runner             runner;
    std::ostringstream out, err;

    int code = runner.run(filename, out, err);

    EXPECT_NE(code, 0);
    EXPECT_TRUE(out.str().empty());
    EXPECT_FALSE(err.str().empty());
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

    Runner             runner;
    std::ostringstream out, err;

    int code = runner.run(filename, out, err);

    EXPECT_NE(code, 0);
    EXPECT_TRUE(out.str().empty());
    EXPECT_FALSE(err.str().empty());
}