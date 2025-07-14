# Libraries don't need header files if the sources themselves don't need them nontrivially. 
# For the usecases in e.g. MathFunctions, includes are found directly. 
add_library(MathFunctions MathFunctions.cc)

# target_include_directories are needed for external inclusion
# Alternatively (better practice), attach headers as interfaces 
target_include_directories(MathFunctions INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(MathFunctions PUBLIC compiler_flags)

set(USE_MYMATH ON)

if (USE_MYMATH)
    target_compile_definitions(MathFunctions PRIVATE USE_MYMATH)
    add_library(SqrtLibrary STATIC mysqrt.cc)
    target_link_libraries(MathFunctions PRIVATE SqrtLibrary)
    target_link_libraries(MathFunctions PUBLIC compiler_flags)
endif()

# Install math functions header
install(FILES MathFunctions.h DESTINATION include)