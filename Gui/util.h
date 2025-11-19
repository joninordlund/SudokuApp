#pragma once
#include <vector>

using Matrix = std::vector<std::vector<int>>;
struct Util
{
    static std::vector<Matrix> convertSolutions(const Matrix &grid);
};