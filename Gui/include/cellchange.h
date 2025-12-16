#pragma once

#include <set>
#include <vector>

using namespace std;

struct State {
    int digit;
    set<int> centerMarks;
    set<int> cornerMarks;
};

struct CellChange {
    int x;
    int y;
    State oldState;
    State newState;
};
