#include <algorithm>
#include <chrono>
#include <iostream>

#include <sycl/sycl.hpp>
#include <CLI/CLI.hpp>

int main(const int argc, const char *const argv[]){
    CLI::App app{"Serial word cloud"};
    app.set_help_flag("-h,--help", "we need help too");
    CLI11_PARSE(app,argc,argv);
}