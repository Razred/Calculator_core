#include "../include/runner.hpp"
#include "../include/logger.hpp"

int Runner::run(int argc, const char* argv[], std::ostream &out) {
    checker.checkInputArgs(argc, argv);

    const std::string json_path = argv[1];
    Logger::instance().info("Starting Runner with file: %s", json_path.c_str());

    Request request = parser.parse(json_path);
    checker.checkRequest(request);

    std::optional<int64_t> second =
    request.has_second ? std::optional<int64_t>(request.second) : std::nullopt;

    auto cache = database.findInCache(request.first, second, request.op_str[0]);

    Response response{};
    if (cache.has_value()) {
        response.result_i64 = cache->result.value_or(0);
        response.status = cache->status;
    }
    else {
        response = calculator.calculate(request);
        std::optional<int64_t> res = (response.status == 0) ? std::optional<int64_t>(response.result_i64) : std::nullopt;
        database.saveOperation(request.first, second, request.op_str[0], res, response.status);
    }

    printer.print(request, response, out);
    Logger::instance().info("Calculation completed successfully");
    return 0;
}