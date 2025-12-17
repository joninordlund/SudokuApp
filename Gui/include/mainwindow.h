#pragma once

#include <QWidget>
#include <QPushButton>

#include "toggleswitch.h"
#include "grid.h"
#include "sudokuimage.h"

class MainWindow : public QWidget
{
    Q_OBJECT
  public:
    explicit MainWindow(QWidget *parent = nullptr);

  private slots:
    void updateUndoRedoButtons(bool canUndo, bool canRedo);
    void updateCountLabel(int count, int maxCount);

  private:

    QPushButton *m_solveBtn;
    QPushButton *m_showSolBtn;
    QPushButton* m_clearBtn;
    QPushButton* m_randomBtn;
    QPushButton* m_undoBtn;
    QPushButton* m_redoBtn;
    ToggleSwitch* m_editModeToggle;
    Grid* m_grid;
    SudokuImage* m_image;
    QLabel* m_solCountLbl;
};
