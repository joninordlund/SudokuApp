#pragma once

#include "cellchange.h"

class Grid;

class History {
  public:
    History(Grid* grid)
        : m_grid(grid) {};
    void undo();
    void redo();
    void setNewCommand(vector<CellChange>);

  private:
    vector<vector<CellChange>> m_past;
    vector<vector<CellChange>> m_future;
    Grid* m_grid;
};
