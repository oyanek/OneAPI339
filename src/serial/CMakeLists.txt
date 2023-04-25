set(CMAKE_CXX_COMPILER "icpx")
set(COMPILE_FLAGS "-fsycl")
set(LINK_FLAGS "-fsycl")

add_executable(wcs word_cloud_serial.cpp)
target_link_libraries(wcs CLI11::CLI11)

target_compile_options(wcs PRIVATE ${COMPILE_FLAGS})
target_link_options(wcs PRIVATE ${LINK_FLAGS})