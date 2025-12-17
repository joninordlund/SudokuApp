#pragma once

#include <cstdint>
using namespace std;

struct State {
    int digit;
    bool isGiven;
    uint16_t centerMarks;
    uint16_t cornerMarks;
};

struct CellChange {
    int x;
    int y;
    State oldState;
    State newState;
};
