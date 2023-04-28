#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>
#include <CL/sycl.hpp>
#include <CLI/CLI.hpp>
#include <vector>


///hash, create window size, CLI options

using namespace std;

const std::string DEF_DICT = "/home/u189146/OneAPI339/WOTW.txt";
const size_t DEF_BITS = 12000000;
const int DEF_WIN_SIZE = 20000;

std::string dict = DEF_DICT;
size_t bits = DEF_BITS;
int win_size = DEF_WIN_SIZE;

vector<vector<size_t>> buffers;

void hash_count(string word){
    hash<string> h;
    size_t hash = h(word);
    hash_words[hash % bits]++;
}


int main(const int argc, const char *const argv[]){
    CLI::App app{"Parallel word cloud"};
    app.set_help_flag("-h,--help", "we need help too");
    ///choose amout of buffers to use
    CLI11_PARSE(app,argc,argv);


  //load dictionary 
  ifstream ifs(dict);
      if (!ifs) {
          cerr << "Failed to open dictionary file: [" << dict << "] (suggest changing code to reflect exact path on your machine)" << endl;
          return 1;
      }
//     const int N = 10000000;
//     int data1[N], data2[N], data3[N]; 

//     buffer<int, 1> buf1(data1, range<1>(N), {property::buffer::use_host_ptr(), property::buffer::cpu_selector_v});
//     buffer<int, 1> buf2(data2, range<1>(N), {property::buffer::use_host_ptr(), property::buffer::gpu_selector_v});
//     buffer<int, 1> buf3(data3, range<1>(N), {property::buffer::use_host_ptr(), property::buffer::gpu_selector_v});
    
//     // three queues, one for each device
//     queue q1{cpu_selector_v()};
//     queue q2{gpu_selector_v()};
//     queue q3{gpu_selector_v()};


//     // get start time and submit the first kernel to q1
//     auto startTotalTime = std::chrono::high_resolution_clock::now();
//     q1.submit([&](handler& h) {
//         auto a = buf1.get_access<access::mode::read_write>(h);
//         // process the data in parallel using a parallel_for
//         h.parallel_for(N, [=](auto i) {
//         a[i] = a[i] * 2;
//         });
//     });
    
//     // get start time and submit the second kernel to q2
//     auto start2 = std::chrono::high_resolution_clock::now();
//     q2.submit([&](handler& h) {
//         auto a = buf2.get_access<access::mode::read_write>(h);
//         // process the data in parallel using a parallel_for
//         h.parallel_for(N, [=](auto i) {
//         a[i] = a[i] + 1;
//         });
//     });
//     auto end2 = std::chrono::high_resolution_clock::now();


//     // get start time and submit the second kernel to q3
//     auto start3 = std::chrono::high_resolution_clock::now();
//     q3.submit([&](handler& h) {
//         // get a reference to the buffer
//         auto a = buf3.get_access<access::mode::read_write>(h);
//         // process the data in parallel using a parallel_for
//         h.parallel_for(N, [=](auto i) {
//         a[i] = a[i] - 1;
//         });
//     });
//     auto end3 = std::chrono::high_resolution_clock::now();

//     auto endTotalTime = std::chrono::high_resolution_clock::now();

//     // wait for kernels to finish then retrieve the results
//     auto result1 = buf1.get_access<access::mode::read>();
//     auto result2 = buf2.get_access<access::mode::read>();
//     auto result3 = buf3.get_access<access::mode::read>();

//     // get the elapsed time for the kernel execution
//     auto elapsedQTotalTime = std::chrono::duration_cast<std::chrono::milliseconds>(totalTime - endTotalTime);
//     auto elapsedQ2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2);
//     auto elapsedQ3 = std::chrono::duration_cast<std::chrono::milliseconds>(end3 - start3);

//     q1.wait();
//     q2.wait();
//     q3.wait();
    
//     //print time results for buffers, done before printing results
//     std::cout << "Kernel 1 execution time: " << elapsed1.count() << " ms" << std::endl;
//     std::cout << "Kernel 2 execution time: " << elapsed2.count() << " ms" << std::endl;
//     std::cout << "Kernel 3 execution time: " << elapsed3.count() << " ms" << std::endl;

//     //print results
//     for (int i = 0; i < N; i++) {
//         std::cout << result1[i] << " ";
//     }
//     std::cout << std::endl;
//     for (int i = 0; i < N; i++) {
//         std::cout << result2[i] << " ";
//     }

//     std::cout << std::endl;
//     for (int i = 0; i < N; i++) {
//         std::cout << result3[i] << " ";
//     }
//     std::cout << std::endl;

//     return 0;
// }


///////////////////////////////////////////////////////////////////

size_t VectorAdd2(sycl::queue &q1, sycl::queue &q2, const IntArray &a,
                  const IntArray &b, IntArray &sum, int iter) {
  sycl::range num_items{a.size() / 2};


     //get start time and submit the first kernel to q1
     auto startTotalTime = std::chrono::high_resolution_clock::now()
  
  {
    sycl::buffer a1_buf(a.data(), num_items);
    sycl::buffer b1_buf(b.data(), num_items);
    sycl::buffer sum1_buf(sum.data(), num_items);

    sycl::buffer a2_buf(a.data() + a.size() / 2, num_items);
    sycl::buffer b2_buf(b.data() + a.size() / 2, num_items);
    sycl::buffer sum2_buf(sum.data() + a.size() / 2, num_items);
    for (int i = 0; i < iter; i++) {

      get start time and submit the first kernel to q
      auto start2 = std::chrono::high_resolution_clock::now();
      q.submit([&](sycl::handler& h) {
        // Input accessors
        sycl::accessor a_acc(a1_buf, h, sycl::read_only);
        sycl::accessor b_acc(b1_buf, h, sycl::read_only);
        // Output accessor
        sycl::accessor sum_acc(sum1_buf, h, sycl::write_only, sycl::no_init);

        h.parallel_for(num_items,
                       [=](auto i) { sum_acc[i] = a_acc[i] + b_acc[i]; });
      });

      // do the work on host
      q2.submit([&](sycl::handler& h) {
        // Input accessors
        sycl::accessor a_acc(a2_buf, h, sycl::read_only);
        sycl::accessor b_acc(b2_buf, h, sycl::read_only);
        // Output accessor
        sycl::accessor sum_acc(sum2_buf, h, sycl::write_only, sycl::no_init);

        h.parallel_for(num_items,
                       [=](auto i) { sum_acc[i] = a_acc[i] + b_acc[i]; });
      });
    };


    // On some platforms this explicit flush of queues is needed
    // to ensure the overlap in execution between the CPU and GPU
    // cl_command_queue cq = q1.get();
    // clFlush(cq);
    // cq=q2.get();
    // clFlush(cq);
  }

  q1.wait();
  q2.wait();
  auto end = std::chrono::steady_clock::now();
  std::cout << "Vector add2 completed on device - took "
            << (end - start).count() << " u-secs\n";
  return ((end - start).count());
} // end VectorAdd2
