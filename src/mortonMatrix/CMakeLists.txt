set(sources include/mortonMatrix/mortonMatrix.hpp src/mortonMatrix.cpp)

add_library(mortonMatrix SHARED ${sources})

target_include_directories(mortonMatrix PUBLIC include)

add_subdirectory(tests)
