# =========================================================
# VectorGestureLab - Project CMake Configuration
# =========================================================
cmake_minimum_required(VERSION 3.10)
project(VectorGestureLab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------- Find OpenCV ----------
find_package(OpenCV REQUIRED)

# -------- Find SFML 3 ----------
# 注意：SFML 3 的组件名首字母大写！
find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

# -------- Include Directories ----------
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${SFML_INCLUDE_DIRS})
include_directories(include)

# -------- Source Files ----------
add_executable(VectorGestureLab
    src/main.cpp
)

# -------- Link Libraries ----------
target_link_libraries(VectorGestureLab
    ${OpenCV_LIBS}
    SFML::Graphics
    SFML::Window
    SFML::System
)