cmake_minimum_required(VERSION 3.10)
project(Tutorial VERSION 1.0)

# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

# configure a header file to pass some of the CMake settings to the source code
configure_file(TutorialConfig.h.in TutorialConfig.h)

# CMake immediately goes to process the subdirectory CMakeLists.txt. Subdirectory gets its own build directory. This simply brings the variables in scope. 
add_subdirectory(MathFunctions)

# Add primary executable
add_executable(Tutorial tutorial.cc)

# Instead of directly specifying cpp standardds, alternatively make into separate library 
add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_11) 
# CMAKE stop 

# Tells **linker** where to find compiled object code / implementations. 
target_link_libraries(Tutorial PUBLIC MathFunctions compiler_flags)

# Tells the **compiler** where to find things. Headers are the menu, while linker provides food 
target_include_directories(
    Tutorial PUBLIC
    ${PROJECT_SOURCE_DIR}
)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )