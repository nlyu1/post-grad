cmake_minimum_required(VERSION 3.15)

# set the project name and version
project(ActionManager VERSION 1.0)

# specify the C++ standard
add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_20)

# add compiler warning flags just when building this project via
# the BUILD_INTERFACE genex
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

# Add fmt library for string formatting
include(FetchContent)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG        10.1.1  # Use a specific version
)
FetchContent_MakeAvailable(fmt)

# Create a library from the ActionManager implementation
add_library(action_manager_lib action_manager.cc action_manager.h)
target_link_libraries(action_manager_lib PUBLIC compiler_flags fmt::fmt)

# Create the main executable and link it to the library
add_executable(main main.cc)
target_link_libraries(main PRIVATE action_manager_lib)

### Testing ###
enable_testing() 
# Test 1
# add_test(NAME Runs COMMAND main)