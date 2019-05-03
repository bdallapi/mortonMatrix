#pragma once
#include <algorithm>
#include <tuple>
#include <vector>

namespace mtmt {
namespace detail {
constexpr uint32_t MASKS[] = {0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF,
                              0x0000FFFF};
constexpr uint32_t SHIFTS[] = {1, 2, 4, 8};

inline uint16_t fromEvenBits(uint32_t z) {
  z = z & MASKS[0];
  z = (z | (z >> SHIFTS[0])) & MASKS[1];
  z = (z | (z >> SHIFTS[1])) & MASKS[2];
  z = (z | (z >> SHIFTS[2])) & MASKS[3];
  z = (z | (z >> SHIFTS[3])) & MASKS[4];
  return static_cast<uint16_t>(z);
}

inline uint32_t toEvenBits(uint16_t x) {
  // taken from
  // http://graphics.stanford.edu/~seander/bithacks.html#InterleaveBMN
  uint32_t z = x;
  z = (z | (z << SHIFTS[3])) & MASKS[3];
  z = (z | (z << SHIFTS[2])) & MASKS[2];
  z = (z | (z << SHIFTS[1])) & MASKS[1];
  z = (z | (z << SHIFTS[0])) & MASKS[0];
  return z;
}
}

inline uint32_t coord2Z(uint16_t x, uint16_t y) {
  return detail::toEvenBits(x) | (detail::toEvenBits(y) << 1);
}

inline std::pair<uint16_t, uint16_t> z2Coord(uint32_t z) {
  return std::make_pair(detail::fromEvenBits(z), detail::fromEvenBits(z >> 1));
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
