cmake_minimum_required(VERSION 3.10)
project(VectorGestureLab)

# ================================
# 编译选项
# ================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

# ================================
# 查找依赖库
# ================================
find_package(SFML 2.6 COMPONENTS graphics audio window system REQUIRED)
find_package(OpenCV REQUIRED)

# ================================
# 包含头文件目录
# ================================
include_directories(
    include                     # 你的 MyVector.h 所在路径
    ${OpenCV_INCLUDE_DIRS}       # OpenCV 头文件
    ${SFML_INCLUDE_DIR}          # SFML 头文件
)
# ================================
# 源文件
# ================================
add_executable(${PROJECT_NAME}
    src/main.cpp
)

# ================================
# 链接库
# ================================
target_link_libraries(${PROJECT_NAME}
    sfml-graphics
    sfml-audio
    sfml-window
    sfml-system
    ${OpenCV_LIBS}
)

# ================================
# 输出构建信息
# ================================
message(STATUS "✅ Vector Gesture Lab build configuration:")
message(STATUS "   SFML include dir: ${SFML_INCLUDE_DIR}")
message(STATUS "   OpenCV include dir: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "   Binary output: ${CMAKE_BINARY_DIR}")
message(STATUS "   C++ standard: ${CMAKE_CXX_STANDARD}")