#pragma once

#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QWidget>

#include "countlabel.h"
#include "grid.h"
#include "sudokuimage.h"
#include "toggleswitch.h"

#include <QKeyEvent>

class MainWindow : public QWidget
{
    Q_OBJECT
  public:
    explicit MainWindow(QWidget *parent = nullptr);

  protected:
    // void keyPressEvent(QKeyEvent* event) override;
    // void keyReleaseEvent(QKeyEvent* event) override;

    bool eventFilter(QObject* obj, QEvent* event) override;

  private slots:
    void updateUndoRedoButtons(bool canUndo, bool canRedo);
    void onExportToPdf();
    // void updateCountLabel(int count, int maxCount);
    void onGenerate();
    void onEditModeToggled(bool checked);
    void onLoadImage();
    void onPeek();
    // void onLockToggle(bool checked);

    void onLockToggled(bool checked);

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

    QPushButton* m_peekBtn;
    QPushButton* m_leftBrowseBtn;
    QPushButton* m_rightBrowseBtn;
    QLabel* m_solIndexLabel;
    QPushButton* m_loadBtn;
    QPushButton* m_saveBtn;
    QPushButton* m_toPDFBtn;
    QPushButton* m_loadImgBtn;
    QPushButton* m_removeImgBtn;
    QPushButton* m_setImgToGrid;
    QLabel* m_editModeValueLbl;
    QPushButton* m_generateBtn;
    QList<QToolButton*> m_starButtons;
    int m_difficulty;

  private:
    void onLoadRequested();
    void onSaveRequested();

    QWidget* createLeftControls();
    QWidget* createSudokuArea();
    QWidget* createImageArea();
    QWidget* createImageControls();
    QHBoxLayout* createBottomBar();
    void connectSignals();

    QHBoxLayout* setupStars();
    void updateStars(int level);
    void updateUI(bool checked);
    void updateSolutionCount(int current, int total);
};
