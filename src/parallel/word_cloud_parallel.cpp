#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>
#include <CL/sycl.hpp>
#include <CLI/CLI.hpp>


///hash, create window size, CLI options

using namespace cl::sycl;

const std::string DEF_DICT = "/home/u189146/OneAPI339/WOTW.txt";
const size_t DEF_BITS = 12000000;

std::string dict = DEF_DICT;
size_t bits = DEF_BITS;


int main(const int argc, const char *const argv[]){
    CLI::App app{"Parallel word cloud"};
    app.set_help_flag("-h,--help", "we need help too");
    ///choose amout of buffers to use
    CLI11_PARSE(app,argc,argv);

    const int N = 10000000;
    int data1[N], data2[N], data3[N]; 

    buffer<int, 1> buf1(data1, range<1>(N), {property::buffer::use_host_ptr(), property::buffer::cpu_selector_v});
    buffer<int, 1> buf2(data2, range<1>(N), {property::buffer::use_host_ptr(), property::buffer::gpu_selector_v});
    buffer<int, 1> buf3(data3, range<1>(N), {property::buffer::use_host_ptr(), property::buffer::gpu_selector_v});
    
    // three queues, one for each device
    queue q1{cpu_selector_v()};
    queue q2{gpu_selector_v()};
    queue q3{gpu_selector_v()};


    // get start time and submit the second kernel to q1
    auto start1 = std::chrono::high_resolution_clock::now();
    q1.submit([&](handler& h) {
        auto a = buf1.get_access<access::mode::read_write>(h);
        // process the data in parallel using a parallel_for
        h.parallel_for(N, [=](auto i) {
        a[i] = a[i] * 2;
        });
    });
    auto end1 = std::chrono::high_resolution_clock::now();

    // get start time and submit the second kernel to q2
    auto start2 = std::chrono::high_resolution_clock::now();
    q2.submit([&](handler& h) {
        auto a = buf2.get_access<access::mode::read_write>(h);
        // process the data in parallel using a parallel_for
        h.parallel_for(N, [=](auto i) {
        a[i] = a[i] + 1;
        });
    });
    auto end2 = std::chrono::high_resolution_clock::now();


    // get start time and submit the second kernel to q2
    auto start3 = std::chrono::high_resolution_clock::now();
    q3.submit([&](handler& h) {
        // get a reference to the buffer
        auto a = buf3.get_access<access::mode::read_write>(h);
        // process the data in parallel using a parallel_for
        h.parallel_for(N, [=](auto i) {
        a[i] = a[i] + 1;
        });
    });
    auto end3 = std::chrono::high_resolution_clock::now();


    // wait for kernels to finish then retrieve the results
    auto result1 = buf1.get_access<access::mode::read>();
    auto result2 = buf2.get_access<access::mode::read>();
    auto result3 = buf3.get_access<access::mode::read>();

    // get the elapsed time for the kernel execution
    auto elapsed1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);
    auto elapsed2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2);
    auto elapsed3 = std::chrono::duration_cast<std::chrono::milliseconds>(end3 - start3);


    //print time results for buffers, done before printing results
    std::cout << "Kernel 1 execution time: " << elapsed1.count() << " ms" << std::endl;
    std::cout << "Kernel 2 execution time: " << elapsed2.count() << " ms" << std::endl;
    std::cout << "Kernel 3 execution time: " << elapsed3.count() << " ms" << std::endl;

    //print results
    for (int i = 0; i < N; i++) {
        std::cout << result1[i] << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < N; i++) {
        std::cout << result2[i] << " ";
    }

    std::cout << std::endl;
    for (int i = 0; i < N; i++) {
        std::cout << result3[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
