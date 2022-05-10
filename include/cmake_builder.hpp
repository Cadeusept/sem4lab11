// Copyright 2021 Your Name <your_email>

#ifndef INCLUDE_EXAMPLE_HPP_
#define INCLUDE_EXAMPLE_HPP_

#include <stdexcept>
#include <async++.h>
#include <boost/process.hpp>

namespace bp = boost::process;

int building_fun(std::string build_type, int timeout);



#endif // INCLUDE_EXAMPLE_HPP_
