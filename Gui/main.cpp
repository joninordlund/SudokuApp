#include <vector>
#include <QApplication>
#include <QGridLayout>
#include <QFrame>

#include "grid.h"
#include "util.h"

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

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget w;
    w.setFixedSize(540, 540);

    QHBoxLayout *mainGrid = new QHBoxLayout(&w);
    mainGrid->setSpacing(0);
    mainGrid->setContentsMargins(5, 5, 5, 5);

    QFrame *frame = new QFrame;
    frame->setFrameStyle(QFrame::Box);
    frame->setLineWidth(3);

    auto sols = Util::convertSolutions(sudoku);

    Grid *grid = new Grid(sols[0], frame);
    QHBoxLayout *frameLayout = new QHBoxLayout(frame);
    frameLayout->setSpacing(0);
    frameLayout->setContentsMargins(0, 0, 0, 0);
    frameLayout->addWidget(grid);

    mainGrid->addWidget(frame);

    w.show();
    return app.exec();
}
