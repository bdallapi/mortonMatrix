#include <gtest/gtest.h>

#include <mortonMatrix/mortonMatrix.hpp>

using namespace mtmt;

struct AMortonCode
    : public testing::TestWithParam<std::pair<uint16_t, uint16_t>> {};

TEST_P(AMortonCode, codeAndDecodeIsConsistent) {
  ASSERT_EQ(mtmt::detail::z2Coord(
                mtmt::detail::coord2Z(GetParam().first, GetParam().second)),
            GetParam());
}

INSTANTIATE_TEST_SUITE_P(SomeCoordinates, AMortonCode,
                         testing::Values(std::pair<uint16_t, uint16_t>(0, 0),
                                         std::pair<uint16_t, uint16_t>(1, 0),
                                         std::pair<uint16_t, uint16_t>(0, 1),
                                         std::pair<uint16_t, uint16_t>(17, 42),
                                         std::pair<uint16_t, uint16_t>(64, 64),
                                         std::pair<uint16_t, uint16_t>(2042,
                                                                       1017)));
struct AFloorLog2
    : public testing::TestWithParam<std::pair<uint16_t, uint16_t>> {};

TEST_P(AFloorLog2, matchesExpectedResult) {
  ASSERT_EQ(mtmt::detail::floorlog2(GetParam().first), GetParam().second);
}

INSTANTIATE_TEST_SUITE_P(SomeKnownResults, AFloorLog2,
                         testing::Values(std::pair<uint16_t, uint16_t>(0, 0),
                                         std::pair<uint16_t, uint16_t>(2, 2),
                                         std::pair<uint16_t, uint16_t>(6, 4),
                                         std::pair<uint16_t, uint16_t>(1042,
                                                                       1024)));

TEST(AMortonMatrix, reorders2by2YieldsExpectedResult) {
  //clang-format off
  std::vector<int> matrix = {0, 1,
                             2, 3};
  //clang-format on
  detail::reorder(matrix.data(), 2, 2);

  ASSERT_EQ(matrix[0], 0);
  ASSERT_EQ(matrix[1], 1);
  ASSERT_EQ(matrix[2], 2);
  ASSERT_EQ(matrix[3], 3);
}

TEST(AMortonMatrix, reorders2by3YieldsExpectedResult) {
  //clang-format off
  std::vector<int> matrix = {0, 1, 2,
                             3, 4, 5};
  //clang-format on
  detail::reorder(matrix.data(), 2, 3);

  ASSERT_EQ(matrix[0], 0);
  ASSERT_EQ(matrix[1], 1);
  ASSERT_EQ(matrix[2], 3);
  ASSERT_EQ(matrix[3], 4);
  ASSERT_EQ(matrix[4], 2);
  ASSERT_EQ(matrix[5], 5);
}

TEST(AMortonMatrix, reorders3by2YieldsExpectedResult) {
  //clang-format off
  std::vector<int> matrix = {0, 1,
                             2, 3,
                             4, 5};
  //clang-format on
  detail::reorder(matrix.data(), 3, 2);

  ASSERT_EQ(matrix[0], 0);
  ASSERT_EQ(matrix[1], 1);
  ASSERT_EQ(matrix[2], 2);
  ASSERT_EQ(matrix[3], 3);
  ASSERT_EQ(matrix[4], 4);
  ASSERT_EQ(matrix[5], 5);
}

TEST(AMortonMatrix, reorders3by3YieldsExpectedResult) {
  //clang-format off
  std::vector<int> matrix = {0, 1, 2,
                             3, 4, 5,
                             6, 7, 8};
  //clang-format on
  detail::reorder(matrix.data(), 3, 3);

  ASSERT_EQ(matrix[0], 0);
  ASSERT_EQ(matrix[1], 1);
  ASSERT_EQ(matrix[2], 3);
  ASSERT_EQ(matrix[3], 4);
  ASSERT_EQ(matrix[4], 2);
  ASSERT_EQ(matrix[5], 5);
  ASSERT_EQ(matrix[6], 6);
  ASSERT_EQ(matrix[7], 7);
  ASSERT_EQ(matrix[8], 8);
}

