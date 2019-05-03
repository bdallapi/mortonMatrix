#pragma once
#include <algorithm>
#include <tuple>
#include <vector>

namespace mtmt {
uint32_t coord2Z(uint16_t xPos, uint16_t yPos) {
  // taken from http://graphics.stanford.edu/~seander/bithacks.html#InterleaveBMN
  static const uint32_t MASKS[] = {0x55555555, 0x33333333, 0x0F0F0F0F,
                                   0x00FF00FF};
  static const uint32_t SHIFTS[] = {1, 2, 4, 8};

  uint32_t x = xPos; // Interleave lower 16 bits of x and y, so the bits of x
  uint32_t y = yPos; // are in the even positions and bits from y in the odd;

  x = (x | (x << SHIFTS[3])) & MASKS[3];
  x = (x | (x << SHIFTS[2])) & MASKS[2];
  x = (x | (x << SHIFTS[1])) & MASKS[1];
  x = (x | (x << SHIFTS[0])) & MASKS[0];

  y = (y | (y << SHIFTS[3])) & MASKS[3];
  y = (y | (y << SHIFTS[2])) & MASKS[2];
  y = (y | (y << SHIFTS[1])) & MASKS[1];
  y = (y | (y << SHIFTS[0])) & MASKS[0];

  const uint32_t result = x | (y << 1);
  return result;
}

template <typename T> void reorder(T *start, uint16_t M, uint16_t N) {
  std::vector<std::tuple<T, uint16_t, uint16_t>> indexedMatrix(M * N);
  for (size_t i = 0; i < M; ++i) {
    for (size_t j = 0; j < N; ++j) {
      indexedMatrix[i * N + j] = std::make_tuple(start[i * N + j], i, j);
    }
  }
  std::sort(indexedMatrix.begin(), indexedMatrix.end(), [](auto lhs, auto rhs) {
    return coord2Z(std::get<1>(lhs), std::get<2>(lhs)) <
           coord2Z(std::get<1>(rhs), std::get<2>(rhs));
  });

  std::transform(indexedMatrix.begin(), indexedMatrix.end(), start,
                 [](auto tpl) { return std::get<0>(tpl); });
}
}
