#pragma once

#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>

#include "countlabel.h"
#include "grid.h"
#include "sudokuimage.h"
#include "toggleswitch.h"

class MainWindow : public QWidget
{
    Q_OBJECT
  public:
    explicit MainWindow(QWidget *parent = nullptr);

  private slots:
    void updateUndoRedoButtons(bool canUndo, bool canRedo);
    void updateCountLabel(int count, int maxCount);

  private:
    QPushButton* m_solveBtn;
    QPushButton* m_showSolBtn;
    QPushButton* m_clearBtn;
    QPushButton* m_randomBtn;
    QPushButton* m_undoBtn;
    QPushButton* m_redoBtn;
    ToggleSwitch* m_lockToggle;
    ToggleSwitch* m_editModeToggle;
    CountLabel* m_puzzleCountLabel;
    Grid* m_grid;
    SudokuImage* m_image;
    QLabel* m_solCountLbl;

  private:
    void onLoadRequested();
    void onSaveRequested();

    QWidget* createLeftControls();
    QWidget* createSudokuArea();
    QWidget* createImageArea();
    QWidget* createImageControls();
    QHBoxLayout* createBottomBar();
    void connectSignals();

  private slots:
    void onEditModeToggled(bool checked);

  private:
    // Lisää nämä member-muuttujat:
    QPushButton* m_peekBtn;
    QPushButton* m_loadBtn;
    QPushButton* m_saveBtn;
    QPushButton* m_loadImgBtn;
    QPushButton* m_removeImgBtn;
    QPushButton* m_setImgToGrid;
    QLabel* m_editModeValueLbl;
};
