#pragma once
#include "model.hpp"
#include "errors.hpp"

#include <string>
#include <fstream>

class Parser {
public:
    Request parse(const std::string &json_path);
};
