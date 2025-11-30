#pragma once

#include <QWidget>
#include <QPushButton>

#include "toggleswitch.h"
#include "grid.h"

class MainWindow : public QWidget
{
    Q_OBJECT
  public:
    explicit MainWindow(QWidget *parent = nullptr);

  private slots:
    // void onEditModeChanged(bool checked);

  private:
    void setupUI();

    QPushButton *m_solveBtn;
    QPushButton *m_showSolBtn;
    QPushButton* m_clearBtn;
    QPushButton* m_randomBtn;
    ToggleSwitch* m_editModeToggle;
    Grid* m_grid;
};
