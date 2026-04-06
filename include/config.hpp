#pragma once

#include <string>

struct DbConfig {
    std::string conn_str = "host=localhost dbname=calc user=calc_user password=calc_pass";
};

struct ServerConfig {
    std::string client_addr = "tcp://127.0.0.1:5555";
};

struct AppConfig {
    DbConfig db;
    ServerConfig server;
};