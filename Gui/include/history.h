#pragma once

#include "cellchange.h"

#include <QObject>

class Grid;

class History : public QObject {

    Q_OBJECT
  public:
    History(Grid* grid)
        : m_grid(grid) {};
    void undo();
    void redo();
    void setNewCommand(vector<CellChange>);
    vector<vector<CellChange>> getUndoStack() const { return m_past; }
    vector<vector<CellChange>> getRedoStack() const { return m_future; }
    void setUndoStack(const vector<vector<CellChange>>& stack) { m_past = stack; }
    void setRedoStack(const vector<vector<CellChange>>& stack) { m_future = stack; }

  signals:
    void historyStateChanged(bool canUndo, bool canRedo);

  private:
    vector<vector<CellChange>> m_past;
    vector<vector<CellChange>> m_future;
    Grid* m_grid;
};
