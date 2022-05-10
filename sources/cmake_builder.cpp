// Copyright 2021 Your Name <your_email>

#include <cmake_builder.hpp>
#include <future>

int building_fun(const std::string build_type, const int timeout) {
    bp::ipstream out;
    bp::group proc_group;

    bp::child build_proc(bp::search_path("cmake"), "-H. -B_builds"
           " -DCMAKE_INSTALL_PREFIX=_install -DCMAKE_BUILD_TYPE=" + build_type,
           bp::std_out > out, proc_group);

    if (timeout > 0) {
        if (!(proc_group.wait_for(std::chrono::seconds(timeout)) &&
              build_proc.wait_for(std::chrono::seconds(timeout)))) {
            proc_group.terminate();
            build_proc.terminate();
            return 1;
        }
    } else {
        proc_group.wait();
        build_proc.wait();
    }
    if (build_proc.exit_code())
        return build_proc.exit_code();

    bp::child build_proc2(bp::search_path("cmake"), "--build _build",
                          bp::std_out > out, proc_group);

    if (timeout > 0) {
        if (!(proc_group.wait_for(std::chrono::seconds(timeout)) &&
             build_proc2.wait_for(std::chrono::seconds(timeout)))) {
            proc_group.terminate();
            build_proc2.terminate();
            return 1;
        }
    } else {
        proc_group.wait();
        build_proc.wait();
    }

    if (build_proc2.exit_code())
      return build_proc2.exit_code();

    return 0;
}
