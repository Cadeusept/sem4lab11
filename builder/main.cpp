#include <cmake_builder.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <string>

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    po::options_description desc("Allowed options");
    desc.add_options()
          ("help", "produce help message")
          ("config", po::value<std::string>(), "set "
             "configuration <Release|Debug>, Debug by default")
          ("install", po::value<bool>(), "add install level")
          ("pack", po::value<int>(), "add pack level")
          ("timeout", po::value<int>(), "set timeout (sec)");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 1;
    }

    return 0;
}