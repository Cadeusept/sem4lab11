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
          ("timeout", po::value<int>(), "set timeout (sec)");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 1;
    }

    int timeout = -1;
    boost::posix_time::ptime begin_time =
        boost::posix_time::microsec_clock::local_time();

    if (vm.count("timeout"))
        timeout = vm["timeout"].as<int>();

    auto building = async::spawn([&vm] {
        if (vm.count("config") &&
            std::string("Release release RELEASE").find(
              vm["config"].as<std::string>()) != std::string::npos)
            building_fun("Release", timeout);
        else
            building_fun("Debug", timeout);

        std::cout << "Build finished" << std::endl;
    });

    //TODO: доделать установку и упаковку
    if (vm.count("install"))
        auto install = async::spawn();

    if (vm.count("pack"))
        auto pack = async::spawn();

    if (timeout >= 0) {

    }

    std::cout << "Execution time: "
              << boost::posix_time::microsec_clock::local_time() - begin_time
              << std::endl;

    return 0;
}