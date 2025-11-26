#include "mainwindow.h"
#include "sudokusolver.h"
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

// void MainWindow::onShowSolution()
// {
//     SudokuSolver solver;
//     solver.getSolutions(m_current)
// }



void MainWindow::onEditModeChanged(bool checked)
{
    if (checked) {
        m_grid->setEditMode(EDIT_CLUES);
        qDebug() << "Edit clues";
    } else {
        m_grid->setEditMode(SOLVE);
        qDebug() << "Solve";
    }
}

void MainWindow::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // --- Ylä-alue: Kaksi ruudukkoa ---
    QWidget *topArea = new QWidget;
    QHBoxLayout *squaresLayout = new QHBoxLayout(topArea);
    squaresLayout->setSpacing(20);
    squaresLayout->setContentsMargins(0, 0, 0, 0);

    // Vasen ruudukko (tehtävä)
    QWidget *leftSquare = new QWidget;
    leftSquare->setFixedSize(540, 540);

    // SudokuSolver solver;
    // auto sols = solver.getSolutions(sudoku);

    m_grid = new Grid(sudoku, leftSquare);

    QHBoxLayout *leftLayout = new QHBoxLayout(leftSquare);
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

    // --- Ala-alue: Painikkeet ---
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setContentsMargins(0, 0, 0, 0);

    m_showSolBtn = new QPushButton("Show solution");
    m_editModeToggle = new ToggleSwitch(this);
    // m_saveBtn = new QPushButton("Tallenna");
    // m_quitBtn = new QPushButton("Lopeta");

    buttonLayout->addWidget(m_showSolBtn);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_editModeToggle);

    mainLayout->addLayout(buttonLayout);

    connect(m_showSolBtn, &QPushButton::pressed, m_grid, &Grid::onShowSolution);
    connect(m_showSolBtn, &QPushButton::released, m_grid, &Grid::onHideSolution);
    connect(m_editModeToggle, &ToggleSwitch::toggled, this, &MainWindow::onEditModeChanged);

}
