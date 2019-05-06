#include <gtest/gtest.h>

#include <mortonMatrix/mortonMatrix.hpp>

#include <random>
#include <algorithm>

using namespace morton;

struct AMortonCode
    : public testing::TestWithParam<std::pair<uint16_t, uint16_t>> {};

TEST_P(AMortonCode, codeAndDecodeIsConsistent) {
  ASSERT_EQ(detail::z2Coord(
                detail::coord2Z(GetParam().first, GetParam().second)),
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
  ASSERT_EQ(detail::floorlog2(GetParam().first), GetParam().second);
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
  reorder(matrix.data(), 2, 2);

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
  reorder(matrix.data(), 2, 3);

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
  reorder(matrix.data(), 3, 2);

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
  reorder(matrix.data(), 3, 3);

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

struct ARandomMortonMatrix : public testing::TestWithParam<std::pair<uint16_t, uint16_t>>
{
    std::vector<int> matrix;
    std::vector<int> mortonMatrix;

    void SetUp() override
    {
        std::default_random_engine re(0);
        std::uniform_int_distribution<int> dist(0,std::numeric_limits<int>::max());
        
        matrix.resize(GetParam().first * GetParam().second);
        std::generate(matrix.begin(), matrix.end(), [&]{return dist(re);});
        mortonMatrix.assign(matrix.begin(), matrix.end());

        reorder(mortonMatrix.data(), GetParam().first, GetParam().second);
    }
};

bool areIdentical(const int* mat, const int* mortonMat, uint16_t M, uint16_t N)
{
    for(size_t i=0; i < M; ++i)
    {
        for(size_t j=0; j < N; ++j)
        {
            if(mat[i*N + j] != morton::get(mortonMat, M, N, i, j))
            {
                return false;
            }
        }
    }
    return true;
}

TEST_P(ARandomMortonMatrix, areIdentical)
{
    ASSERT_TRUE(areIdentical(matrix.data(), mortonMatrix.data(), GetParam().first, GetParam().second));
}

INSTANTIATE_TEST_SUITE_P(SomeMatrixSizes, ARandomMortonMatrix,
                         testing::Values(std::make_pair(8, 8),
                                         std::make_pair(17, 19),
                                         std::make_pair(64, 96),
                                         std::make_pair(1024, 2048)));
