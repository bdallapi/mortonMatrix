#include <gtest/gtest.h>

#include <mortonMatrix/mortonMatrix.hpp>

using namespace mtmt;

struct AMortonCode : public testing::TestWithParam<std::pair<uint16_t, uint16_t>>
{
};

TEST_P(AMortonCode, codeAndDecodeIsConsistent)
{
    ASSERT_EQ(mtmt::z2Coord(mtmt::coord2Z(GetParam().first, GetParam().second)), GetParam());
}

INSTANTIATE_TEST_SUITE_P(SomeCoordinates, AMortonCode,
                         testing::Values(std::pair<uint16_t, uint16_t>(0, 0),
                                         std::pair<uint16_t, uint16_t>(1, 0),
                                         std::pair<uint16_t, uint16_t>(0, 1),
                                         std::pair<uint16_t, uint16_t>(17, 42),
                                         std::pair<uint16_t, uint16_t>(64, 64),
                                         std::pair<uint16_t, uint16_t>(2042, 1017)
                                         ));

TEST(AMortonMatrix, indexingWorksOn2x2) {
  std::vector<int> matrix = {0, 1, 2, 3};
  reorder(matrix.data(), 2, 2);

  ASSERT_EQ(matrix[coord2Z(0, 0)], 0);
  ASSERT_EQ(matrix[coord2Z(0, 1)], 1);
  ASSERT_EQ(matrix[coord2Z(1, 0)], 2);
  ASSERT_EQ(matrix[coord2Z(1, 1)], 3);
}
