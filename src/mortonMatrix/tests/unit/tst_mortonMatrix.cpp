#include <cstdlib>
#include <cassert>
#include <mortonMatrix/mortonMatrix.hpp>

using namespace mtmt;

int main()
{
    std::vector<int> matrix = {0, 1, 2, 3};
    reorder(matrix.data(), 2, 2);

    assert(matrix[coord2Z(0, 0)] == 0);
    assert(matrix[coord2Z(0, 1)] == 1);
    assert(matrix[coord2Z(1, 0)] == 2);
    assert(matrix[coord2Z(1, 1)] == 3);

    return EXIT_SUCCESS;
}
