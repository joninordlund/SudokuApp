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
    // void onEditModeChanged(bool checked);

  private:

    QPushButton *m_solveBtn;
    QPushButton *m_showSolBtn;
    QPushButton* m_clearBtn;
    QPushButton* m_randomBtn;
    ToggleSwitch* m_editModeToggle;
    Grid* m_grid;
    SudokuImage* m_image;
};
