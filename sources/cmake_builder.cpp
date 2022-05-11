// Copyright 2021 Your Name <your_email>

#include <cmake_builder.hpp>
#include <future>
#include <iostream>

void create_child(const std::string command, const size_t timeout) {
    bp::ipstream out;
    std::string line;

    bp::child process(command, bp::std_out > out);

    std::thread checkTime(check_time, std::ref(process),
                          std::ref(timeout));

    while (out && std::getline(out, line) && !line.empty())
        std::cerr << line << std::endl;

    checkTime.join();
}

void create_child(const std::string command, const time_t &timeout, int &res) {
  bp::ipstream out;
  std::string line;

  bp::child process(command, bp::std_out > out);

  std::thread checkTime(check_time, std::ref(process),
                        std::ref(timeout));

  while (out && std::getline(out, line) && !line.empty())
    std::cerr << line << std::endl;

  checkTime.join();

  res = process.exit_code();
}

void check_time(bp::child &process, const time_t &timeout) {
    time_t start = time_now();

    while (true) {
        if ((time_now() - start > timeout) && process.running()) {
            std::error_code ec;
            process.terminate(ec);
            std::cout << ec;
            break;
        } else {
            if (!process.running()) {
                break;
            }
        }
    }
}

time_t time_now() {
    return std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now()
    );
}
