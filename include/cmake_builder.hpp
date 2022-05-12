// Copyright 2021 Your Name <your_email>

#ifndef INCLUDE_CMAKE_BUILDER_HPP_
#define INCLUDE_CMAKE_BUILDER_HPP_

#include <stdexcept>
#include <async++.h>
#include <boost/process.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>

namespace bp = boost::process;

void create_child(const std::string command, size_t timeout);

void create_child(const std::string command, const time_t &timeout, int &res);

void check_time(bp::child &process, const time_t &timeout);

time_t time_now();

#endif // INCLUDE_CMAKE_BUILDER_HPP_
