add_executable(tests_unit_mortonMatrix unit/tst_mortonMatrix.cpp)

target_link_libraries(tests_unit_mortonMatrix mortonMatrix gtest_main)
