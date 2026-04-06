#pragma once
#include "model.hpp"
#include "errors.hpp"
#include <nlohmann/json.hpp>

#include <string>
#include <fstream>

class Parser {
public:
    Request parse(const nlohmann::json &json_data);
};
