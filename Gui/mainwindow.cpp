#include "mainwindow.h"
#include "grid.h"
#include <QVBoxLayout>

const std::vector<std::vector<int>> sudoku{
    {0, 0, 0, 2, 0, 5, 0, 0, 0},
    {0, 0, 8, 0, 0, 0, 2, 0, 0},
    {0, 5, 0, 8, 0, 9, 0, 1, 0},
    {9, 0, 7, 0, 0, 0, 8, 0, 6},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {6, 0, 2, 0, 0, 0, 3, 0, 9},
    {0, 4, 0, 1, 0, 8, 0, 3, 0},
    {0, 0, 6, 0, 0, 0, 5, 0, 0},
    {0, 0, 0, 5, 0, 4, 0, 0, 0}};

MainWindow::MainWindow(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(1100, 640);
    setupUI();
}

void MainWindow::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QWidget* topArea = new QWidget;
    QHBoxLayout *squaresLayout = new QHBoxLayout(topArea);
    squaresLayout->setSpacing(20);
    squaresLayout->setContentsMargins(0, 0, 0, 0);

    QWidget* leftSquare = new QWidget;
    leftSquare->setFixedSize(540, 540);

    QHBoxLayout* leftLayout = new QHBoxLayout(leftSquare);
    m_grid = new Grid(leftSquare);
    leftLayout->addWidget(m_grid);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    squaresLayout->addWidget(leftSquare);


    // QWidget *rightSquare = new QWidget;
    // rightSquare->setFixedSize(540, 540);
    // m_solutionGrid = new Grid(rightSquare);
    // QHBoxLayout *rightLayout = new QHBoxLayout(rightSquare);
    // rightLayout->addWidget(m_solutionGrid);
    // rightLayout->setContentsMargins(0, 0, 0, 0);
    // squaresLayout->addWidget(rightSquare);

    mainLayout->addWidget(topArea);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setContentsMargins(0, 0, 0, 0);

    m_showSolBtn = new QPushButton("Peek solution");
    m_editModeToggle = new ToggleSwitch(this);
    m_clearBtn = new QPushButton("Clear");
    m_randomBtn = new QPushButton("Random");

    buttonLayout->addWidget(m_showSolBtn);
    buttonLayout->addWidget(m_clearBtn);
    buttonLayout->addWidget(m_randomBtn);

    buttonLayout->addStretch();
    buttonLayout->addWidget(m_editModeToggle);

    mainLayout->addLayout(buttonLayout);

    connect(m_showSolBtn, &QPushButton::pressed, m_grid, &Grid::onShowSolution);
    connect(m_showSolBtn, &QPushButton::released, m_grid, &Grid::onHideSolution);
    connect(m_clearBtn, &QPushButton::clicked, m_grid, &Grid::onClearSolution);
    connect(m_randomBtn, &QPushButton::clicked, m_grid, &Grid::onRandom);

    connect(m_editModeToggle, &ToggleSwitch::toggled, m_grid, &Grid::onEditModeChanged);
}
