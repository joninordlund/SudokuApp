#include <vector>
#include <QApplication>
#include <QGridLayout>
#include <QPushButton>
#include <QFrame>
#include "mainwindow.h"

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


/*
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);


    QWidget w;

    w.setFixedSize(1100, 640); // Esimerkiksi 1080 (2*540) + marginaali + 100 korkeutta napeille

    QVBoxLayout *mainLayout = new QVBoxLayout(&w);
    // mainLayout->setContentsMargins(10, 10, 10, 10); // Päämarginaalit
    // mainLayout->setSpacing(20);                     // Tilaa ylä- ja ala-alueen välissä

    // --- 1. Ylä-alue: Kaksi neliötä rinnakkain (QHBoxLayout) ---
    QWidget *topArea = new QWidget;
    QHBoxLayout *squaresLayout = new QHBoxLayout(topArea);
    squaresLayout->setSpacing(20);                 // Tilaa neliöiden välissä
    squaresLayout->setContentsMargins(0, 0, 0, 0); // Ei ylimääräisiä marginaaleja

    QWidget *square1 = new QWidget;
    square1->setFixedSize(540, 540);


    // **HUOM:** Tähän kohtaan sijoittaisit alkuperäisen Sudoku-koodisi:
    SudokuSolver solver;
    auto sols = solver.getSolutions(sudoku);

    Grid *grid = new Grid(sols[0], square1);
    QHBoxLayout *frameLayout = new QHBoxLayout(square1);
    frameLayout->addWidget(grid);

    squaresLayout->addWidget(square1);

    // Luodaan toinen 540x540 neliö
    QWidget *square2 = new QWidget;
    square2->setFixedSize(540, 540);
    // square2->setFrameShape(QFrame::Box);
    // square2->setLineWidth(3);

    squaresLayout->addWidget(square2);

    mainLayout->addWidget(topArea); // Lisätään kahden neliön asettelu pää-asetteluun

    // --- 2. Ala-alue: Horisontaalinen painikeasettelu (QHBoxLayout) ---
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setContentsMargins(0, 0, 0, 0);

    // Esimerkkipainikkeet
    QPushButton *btn = new QPushButton("Tallenna");
    btn->setStyleSheet(
        "QPushButton {"
        "    background-color: #4CAF50;"  // Vihreä tausta
        "    color: white;"                // Valkoinen teksti
        "    border: none;"
        "    border-radius: 5px;"          // Pyöristetyt kulmat
        "    padding: 10px 20px;"
        "    font-size: 16px;"
        "}"
        "QPushButton:hover {"              // Hover-efekti
        "    background-color: #45a049;"
        "}"
        "QPushButton:pressed {"            // Painettu-tila
        "    background-color: #3d8b40;"
        "}"
        );
    ToggleSwitch *toggle = new ToggleSwitch;
    buttonLayout->addWidget(new QPushButton("Laske Uudelleen"));
    buttonLayout->addStretch(); // Painaa painikkeet vasemmalle
    buttonLayout->addWidget(btn);
    buttonLayout->addWidget(toggle);

    mainLayout->addLayout(buttonLayout); // Lisätään painikeasettelu pää-asetteluun

    w.show();
    return app.exec();
}*/


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}

