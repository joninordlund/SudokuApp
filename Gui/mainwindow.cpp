#include "mainwindow.h"
#include "grid.h"
#include <QVBoxLayout>


MainWindow::MainWindow(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(1140, 640);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QWidget* centerArea = new QWidget;
    QHBoxLayout *centerLayout = new QHBoxLayout(centerArea);
    centerLayout->setSpacing(20);
    centerLayout->setContentsMargins(0, 0, 0, 0);

    QWidget* leftSquare = new QWidget;
    leftSquare->setFixedSize(540, 540);

    QHBoxLayout* leftLayout = new QHBoxLayout(leftSquare);
    m_grid = new Grid(leftSquare);
    leftLayout->addWidget(m_grid);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->addWidget(leftSquare);


    QFrame *rightSquare = new QFrame;
    rightSquare->setFixedSize(540, 540);
    rightSquare->setFrameShape(QFrame::Box);
    rightSquare->setLineWidth(3);

    QHBoxLayout *rightLayout = new QHBoxLayout(rightSquare);
    m_image = new SudokuImage(rightSquare);
    rightLayout->addWidget(m_image);
    rightLayout->setContentsMargins(8, 8, 8, 8);
    centerLayout->addWidget(rightSquare);

    mainLayout->addWidget(centerArea);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setContentsMargins(0, 0, 0, 0);

    m_showSolBtn = new QPushButton("Peek solution");
    m_editModeToggle = new ToggleSwitch(this);
    m_clearBtn = new QPushButton("Clear");
    m_randomBtn = new QPushButton("Random");
    m_undoBtn = new QPushButton("undo");
    m_redoBtn = new QPushButton("redo");
    m_solCountLbl = new QLabel("Solution count: ");

    buttonLayout->addWidget(m_solCountLbl);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_showSolBtn);
    buttonLayout->addWidget(m_clearBtn);
    buttonLayout->addWidget(m_randomBtn);
    buttonLayout->addWidget(m_undoBtn);
    buttonLayout->addWidget(m_redoBtn);

    buttonLayout->addStretch();
    buttonLayout->addWidget(m_editModeToggle);

    mainLayout->addLayout(buttonLayout);

    connect(m_image, &SudokuImage::newSudoku, m_grid, &Grid::newSudoku);
    connect(m_showSolBtn, &QPushButton::pressed, m_grid, &Grid::onShowSolution);
    connect(m_showSolBtn, &QPushButton::released, m_grid, &Grid::onHideSolution);
    connect(m_clearBtn, &QPushButton::clicked, m_grid, &Grid::onClearSolution);
    connect(m_randomBtn, &QPushButton::clicked, m_grid, &Grid::onRandom);
    connect(m_undoBtn, &QPushButton::clicked, m_grid, &Grid::onUndo);
    connect(m_redoBtn, &QPushButton::clicked, m_grid, &Grid::onRedo);

    connect(m_editModeToggle, &ToggleSwitch::toggled, m_grid, &Grid::onEditModeChanged);
    connect(m_grid->getHistory(), &History::historyStateChanged, this, &MainWindow::updateUndoRedoButtons);
    connect(m_grid, &Grid::solutionCountChanged, this, &MainWindow::updateCountLabel);
}

void MainWindow::updateUndoRedoButtons(bool canUndo, bool canRedo)
{
    m_undoBtn->setEnabled(canUndo);
    m_redoBtn->setEnabled(canRedo);
}

void MainWindow::updateCountLabel(int count, int maxCount)
{
    QString str;
    if (count >= maxCount)
    {
        QTextStream(&str) << "Solution count: " << maxCount << "+";
    }
    else
    {
        QTextStream(&str) << "Solution count: " << count;
    }
    m_solCountLbl->setText(str);
}
