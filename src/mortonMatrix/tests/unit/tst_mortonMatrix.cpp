#include <gtest/gtest.h>

#include <mortonMatrix/mortonMatrix.hpp>

using namespace mtmt;

TEST(AMortonMatrix, indexingWorksOn2x2) {
  std::vector<int> matrix = {0, 1, 2, 3};
  reorder(matrix.data(), 2, 2);

  ASSERT_EQ(matrix[coord2Z(0, 0)], 0);
  ASSERT_EQ(matrix[coord2Z(0, 1)], 1);
  ASSERT_EQ(matrix[coord2Z(1, 0)], 2);
  ASSERT_EQ(matrix[coord2Z(1, 1)], 3);
}
