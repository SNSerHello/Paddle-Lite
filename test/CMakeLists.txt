cmake_minimum_required(VERSION 3.10)

project(test_paddlelite_opencl)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /bigobj /MTd")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /bigobj /MTd")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /bigobj /MT")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /bigobj /MT")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
link_directories(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../build.lite.x86.opencl/inference_lite_lib/cxx/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../build.lite.x86.opencl/inference_lite_lib/cxx/lib)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../build.lite.x86.opencl/third_party/mklml/src/extern_mklml/lib)

add_executable(test_paddlelite_opencl test_paddlelite_opencl.cpp)
target_link_libraries(test_paddlelite_opencl libiomp5md.lib libpaddle_api_light_bundled.lib opencv_world455.lib)
