#include <cmake_builder.hpp>
#include <boost/program_options.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <string>

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    po::options_description desc("Allowed options");
    desc.add_options()
          ("help", "produce help message")
          ("config", po::value<std::string>(), "set "
             "configuration <Release|Debug>, Debug by default")
          ("install", "add install level")
          ("pack", "add pack level")
          ("timeout", po::value<size_t>(), "set timeout (sec)");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 1;
    }

    std::string command_1 = "cmake -H. -B_build -DCMAKE_INSTALL_PREFIX="
                            "_install -DCMAKE_BUILD_TYPE=";
    std::string command_2 = "cmake --build _build";
    std::string command_3 = "cmake --build _build --target install";
    std::string command_4 = "cmake --build _build --target package";

    size_t timeout = 0;

    if (vm.count("timeout"))
        timeout = vm["timeout"].as<size_t>();

    if (vm.count("config") &&
        std::string("Release release RELEASE").find(
            vm["config"].as<std::string>()) != std::string::npos)
        command_1 +="Release";
    else
        command_1 +="Debug";

    int res_1 = 0;
    int res_2 = 0;
    time_t time_spent = 0;
    //time_t begin_time = time_now();

    auto building_task = async::spawn([&res_1, timeout, &time_spent,
                                       command_1, command_2] () mutable {
        time_t start_1 = time_now();

        create_child(command_1, timeout);
        time_t end_1 = time_now();

        time_spent += end_1 - start_1;

        time_t period_2 = timeout - time_spent;
        create_child(command_2, period_2, res_1);
        time_t end_2 = time_now();

        time_spent += end_2 - end_1;
    });

    if (vm.count("install") && res_1 == 0) {
        auto install_task = building_task.then(
                                              [&res_2, timeout, &time_spent,
                                              command_2, command_3] () mutable {
          time_t period_3 = timeout - time_spent;
          time_t start_3 = time_now();

          create_child(command_3, period_3, res_2);
          time_t end_3 = time_now();

          time_spent += end_3 - start_3;
        });

        if (vm.count("pack") && res_2 == 0) {
          auto pack_task = install_task.then(
                          [command_4, timeout, time_spent] () {
              time_t period_4 = timeout - time_spent;

              create_child(command_4, period_4);
          });
        }
    }

    //std::cout << "Execution time: "
    //          << time_now() - begin_time
    //          << std::endl;

    return 0;
}