cmake_minimum_required(VERSION 3.28.3)

# Set project name and version. This also sets
# Tutorial_VERSION_MAJOR and Tutorial_VERSION_MINOR.
project(Tutorial VERSION 1.0)

# Set the C++ standard. This should be done before adding targets.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# The tutorial uses configure_file to pass version info to the source.
# This will create 'use_list.h' directly inside your build directory.
configure_file(include/use_list.h.in use_list.h)

# Add the executable target for your use_list program.
add_executable(use_list src/use_list.cc)
# Add the build directory to the include paths for the 'use_list' target.
# This is necessary so the compiler can find the generated 'use_list.h'
# which will be at <build_dir>/include/use_list.h
target_include_directories(use_list PUBLIC
  "${PROJECT_BINARY_DIR}"
)


add_executable(mysqrt src/MathFunctions/mysqrt.cpp)
target_include_directories(mysqrt PUBLIC
  "${PROJECT_BINARY_DIR}"
)