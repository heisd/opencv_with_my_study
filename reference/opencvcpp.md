# 这是使用cmake工具来构造opencv的一个demo
- 构建一个 demo
```zsh
touch CMakeLists.txt
cat > CMakeLists.txt << EOF
cmake_minimum_required(VERSION 3.10)
project(opencv)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 寻找OpenCV的包
find_package(OpenCV REQUIRED)
# 打印cmake找到的opencv的包的版本
message(STATUS "OpenCV version: ${OpenCV_VERSION}")

include_directories(${OpenCV_INCLUDE_DIRS})
message(STATUS "OpenCV include directories: ${OpenCV_INCLUDE_DIRS}")

add_executable(opencv.exe src/opencv.cpp)
target_link_libraries(opencv.exe ${OpenCV_LIBS})
EOF
cmake .
make
./opencv.exe
```


