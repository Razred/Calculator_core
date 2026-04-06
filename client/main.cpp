#include <zmq.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <string>

int main() {
    zmq::context_t context{1};
    zmq::socket_t socket{context, zmq::socket_type::req};

    socket.connect("tcp://127.0.0.1:5555");

    std::cout << "Connected to server\n";

    while (true) {
        int first;
        int second;
        std::string op;

        std::cout << "\nEnter operation (+, -, *, /, pow, fact) or 'exit': ";
        std::cin >> op;

        if (op == "exit") break;

        std::cout << "Enter first: ";
        std::cin >> first;

        nlohmann::json req;

        if (op == "fact" || op == "!") {
            req = {
                {"first", first},
                {"op", "fact"}
            };
        } else {
            std::cout << "Enter second: ";
            std::cin >> second;

            req = {
                {"first", first},
                {"second", second},
                {"op", op}
            };
        }

        std::string req_str = req.dump();

        socket.send(zmq::buffer(req_str), zmq::send_flags::none);

        zmq::message_t reply;
        socket.recv(reply, zmq::recv_flags::none);

        std::string reply_str(
            static_cast<char*>(reply.data()),
            reply.size()
        );

        std::cout << "Response: " << reply_str << "\n";
        try {
            auto json = nlohmann::json::parse(reply_str);

            if (json["status"] == 0) {
                std::cout << "Result = " << json["result"] << "\n";
            } else {
                std::cout << "Error status = " << json["status"] << "\n";
            }
        } catch (...) {
            std::cout << "Failed to parse response\n";
        }
    }

    return 0;
}