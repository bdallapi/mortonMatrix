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

/// return the nearest power of 2 smaller than n.
inline uint16_t floorlog2(uint16_t n) {
  // special case if n is a power of 2.
  if (n != 0 && !(n & (n - 1))) {
    return n;
  }
  --n;
  n |= n >> 1;
  n |= n >> 2;
  n |= n >> 4;
  n |= n >> 8;
  ++n;
  return n >> 1;
}

inline uint32_t coord2Z(uint16_t i, uint16_t j) {
  return (detail::toEvenBits(i) << 1) | detail::toEvenBits(j);
}

inline std::pair<uint16_t, uint16_t> z2Coord(uint32_t z) {
  return std::make_pair(detail::fromEvenBits(z >> 1), detail::fromEvenBits(z));
}

// reorder a MxN matrix (leading dimension N) in Z-order.
// to support arbitrary M*N matrix (not only power of 2), the matrix is
// partitioned into 3 blocks:
// ----------
// |A    |B |
// |     |  |
// ----------
// |C       |
// ----------
// A: square Na*Na block with Na the greatest power of 2 smaller than min(N, M)
// B: Na*(N-Na) matrix
// C: (M-Na)*N matrix
// The matrix A is ordered following Morton-order.
// The matrices B and C are ordered following the same partitioning recursively.
template <typename T> void reorder(T *start, uint16_t M, uint16_t N) {
  if (!(M * N)) {
    return;
  }

  uint16_t Na = floorlog2(std::min(M, N));

  std::vector<T> blockMatrix(N * M);
  for (uint16_t i = 0; i < Na; ++i) {
    for (uint16_t j = 0; j < Na; ++j) {
      blockMatrix[coord2Z(i, j)] = start[i * N + j];
    }
  }

  for (uint16_t i = 0; i < Na; ++i) {
    for (uint16_t j = Na; j < N; ++j) {
      blockMatrix[Na * Na + i * (N - Na) + j - Na] = start[i * N + j];
    }
  }
  reorder(blockMatrix.data() + Na * Na, Na, N - Na);

  for (uint16_t i = Na; i < M; ++i) {
    for (uint16_t j = 0; j < N; ++j) {
      blockMatrix[Na * Na + Na * (N - Na) + (i - Na) * N + j] =
          start[i * N + j];
    }
  }
  reorder(blockMatrix.data() + Na * Na + Na * (N - Na), M - Na, N);

  std::copy(blockMatrix.begin(), blockMatrix.end(), start);
}

} // namespace detail
} // namespace mtmt
