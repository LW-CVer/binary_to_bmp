cmake_minimum_required(VERSION 3.14)
project(invasion_detection)

IF (CMAKE_BUILD_TYPE STREQUAL Debug)
    message("Compile in Debug mode.")
    set(CMAKE_CXX_STANDARD 11)
    ADD_DEFINITIONS(-D _DEBUG)
ELSE()
    set(CMAKE_CXX_FLAGS "${CMAKE_XCC_FLAGS} -Wall -std=c++11 -O2")
ENDIF()

# 输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)




# opencv相关
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


add_executable(test ./save_to_binary)

target_link_libraries(test ${OpenCV_LIBS})
