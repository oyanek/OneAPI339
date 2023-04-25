set(CMAKE_CXX_COMPILER "icpx")
set(COMPILE_FLAGS "-fsycl")
set(LINK_FLAGS "-fsycl")

add_executable(wcp word_cloud_parallel.cpp)
target_link_libraries(wcp CLI11::CLI11)

target_compile_options(wcp PRIVATE ${COMPILE_FLAGS})
target_link_options(wcp PRIVATE ${LINK_FLAGS})