#include "mainwindow.h"
#include "grid.h"
#include "sudokuserializer.h"
#include <QCheckBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent) :
    QWidget{parent}
{
    setFixedSize(1380, 640);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QWidget* centerArea = new QWidget;
    QHBoxLayout* centerLayout = new QHBoxLayout(centerArea);
    centerLayout->setSpacing(20);
    centerLayout->setContentsMargins(0, 0, 0, 0);

    centerLayout->addWidget(createLeftControls());
    centerLayout->addWidget(createSudokuArea());
    centerLayout->addWidget(createImageArea());
    centerLayout->addWidget(createImageControls());

    mainLayout->addWidget(centerArea);
    mainLayout->addLayout(createBottomBar());

    connectSignals();
}

QWidget* MainWindow::createLeftControls()
{
    const int spacing = 10;
    QWidget* controlArea = new QWidget;
    controlArea->setFixedWidth(90);
    QVBoxLayout* layout = new QVBoxLayout(controlArea);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);

    // File section
    QLabel* fileLbl = new QLabel("File");
    fileLbl->setObjectName("sectionLabel");
    QPushButton* loadBtn = new QPushButton("Load");
    QPushButton* saveBtn = new QPushButton("Save");

    layout->addWidget(fileLbl);
    layout->addWidget(loadBtn);
    layout->addWidget(saveBtn);
    layout->addSpacing(spacing);

    // Sudoku section
    QLabel* sudokuLbl = new QLabel("Sudoku");
    sudokuLbl->setObjectName("sectionLabel");
    layout->addWidget(sudokuLbl);

    // Undo/Redo arrows
    QWidget* undoRedoWidget = new QWidget;
    QHBoxLayout* undoRedoLayout = new QHBoxLayout(undoRedoWidget);
    undoRedoLayout->setContentsMargins(0, 0, 0, 0);
    undoRedoLayout->setSpacing(5);

    m_undoBtn = new QPushButton("↶");
    m_redoBtn = new QPushButton("↷");
    // m_undoBtn->setObjectName("colorRed");
    // m_redoBtn->setObjectName("colorRed");
    m_undoBtn->setFixedSize(35, 35);
    m_redoBtn->setFixedSize(35, 35);

    undoRedoLayout->addWidget(m_undoBtn);
    undoRedoLayout->addWidget(m_redoBtn);
    layout->addWidget(undoRedoWidget);

    // Mode toggle
    QWidget* modeWidget = new QWidget;
    QHBoxLayout* modeLayout = new QHBoxLayout(modeWidget);
    modeLayout->setContentsMargins(0, 0, 0, 0);
    modeLayout->setSpacing(8);

    QLabel* modeLabel = new QLabel("Mode:");
    m_editModeToggle = new ToggleSwitch(this);

    modeLayout->addWidget(modeLabel);
    modeLayout->addWidget(m_editModeToggle);
    layout->addWidget(modeWidget);

    // Clear and Random buttons
    m_clearBtn = new QPushButton("Clear");
    m_randomBtn = new QPushButton("Random");
    m_clearBtn->setObjectName("colorRed");
    // m_randomBtn->setObjectName("colorRed");
    layout->addWidget(m_clearBtn);
    layout->addWidget(m_randomBtn);
    layout->addSpacing(spacing);

    // Solutions section
    QLabel* solLbl = new QLabel("Solutions");
    solLbl->setObjectName("sectionLabel");
    m_peekBtn = new QPushButton("Peek");
    m_peekBtn->setObjectName("colorGreen");
    layout->addWidget(solLbl);
    layout->addWidget(m_peekBtn);

    // Lock toggle
    QWidget* lockWidget = new QWidget;
    QHBoxLayout* lockLayout = new QHBoxLayout(lockWidget);
    lockLayout->setContentsMargins(0, 0, 0, 0);
    lockLayout->setSpacing(8);

    QLabel* lockLabel = new QLabel("Lock:");
    m_lockToggle = new ToggleSwitch(this);

    lockLayout->addWidget(lockLabel);
    lockLayout->addWidget(m_lockToggle);
    layout->addWidget(lockWidget);

    // Navigation arrows
    QWidget* arrowWidget = new QWidget;
    QHBoxLayout* arrowLayout = new QHBoxLayout(arrowWidget);
    arrowLayout->setContentsMargins(0, 0, 0, 0);
    arrowLayout->setSpacing(5);

    QPushButton* leftArrowBtn = new QPushButton("←");
    QPushButton* rightArrowBtn = new QPushButton("→");
    leftArrowBtn->setFixedSize(35, 35);
    rightArrowBtn->setFixedSize(35, 35);

    arrowLayout->addWidget(leftArrowBtn);
    arrowLayout->addWidget(rightArrowBtn);
    layout->addWidget(arrowWidget);

    // Puzzle count
    m_puzzleCountLabel = new CountLabel;
    m_puzzleCountLabel->setCount(100);
    layout->addWidget(m_puzzleCountLabel);

    layout->addStretch();

    // Store buttons for signal connection
    m_loadBtn = loadBtn;
    m_saveBtn = saveBtn;

    return controlArea;
}

QWidget* MainWindow::createSudokuArea()
{
    QWidget* leftSquare = new QWidget;
    leftSquare->setFixedSize(540, 540);

    QHBoxLayout* layout = new QHBoxLayout(leftSquare);
    m_grid = new Grid(leftSquare);
    layout->addWidget(m_grid);
    layout->setContentsMargins(0, 0, 0, 0);

    return leftSquare;
}

QWidget* MainWindow::createImageArea()
{
    QFrame* rightSquare = new QFrame;
    rightSquare->setFixedSize(540, 540);
    rightSquare->setFrameShape(QFrame::Box);
    rightSquare->setLineWidth(3);

    QHBoxLayout* layout = new QHBoxLayout(rightSquare);
    m_image = new SudokuImage(rightSquare);
    layout->addWidget(m_image);
    layout->setContentsMargins(8, 8, 8, 8);

    return rightSquare;
}

QWidget* MainWindow::createImageControls()
{
    QWidget* imageControlArea = new QWidget;
    imageControlArea->setFixedWidth(100);
    QVBoxLayout* layout = new QVBoxLayout(imageControlArea);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);

    QLabel* imgLbl = new QLabel("Image");
    imgLbl->setObjectName("sectionLabel");

    m_loadImgBtn = new QPushButton("Load");
    m_removeImgBtn = new QPushButton("Remove");
    m_setImgToGrid = new QPushButton("Set");
    m_setImgToGrid->setObjectName("colorGreen");

    layout->addWidget(imgLbl);
    layout->addWidget(m_loadImgBtn);
    layout->addWidget(m_removeImgBtn);
    layout->addSpacing(20);
    layout->addWidget(m_setImgToGrid);
    layout->addStretch();

    // m_loadImgBtn = loadImgBtn;
    // m_removeImgBtn = removeImgBtn;

    return imageControlArea;
}

QHBoxLayout* MainWindow::createBottomBar()
{
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->setContentsMargins(0, 0, 0, 0);

    m_solCountLbl = new QLabel("Solution count: ");

    QLabel* editModeLabel = new QLabel("Edit mode:");
    m_editModeValueLbl = new QLabel("Solve"); // Tai "Edit clues"

    buttonLayout->addWidget(m_solCountLbl);
    buttonLayout->addSpacing(40);
    buttonLayout->addWidget(editModeLabel);
    buttonLayout->addWidget(m_editModeValueLbl);
    buttonLayout->addStretch();

    return buttonLayout;
}

void MainWindow::connectSignals()
{
    connect(m_image, &SudokuImage::newSudoku, m_grid, &Grid::newSudoku);
    connect(m_peekBtn, &QPushButton::pressed, m_grid, &Grid::onShowSolution);
    connect(m_peekBtn, &QPushButton::released, m_grid, &Grid::onHideSolution);
    connect(m_clearBtn, &QPushButton::clicked, m_grid, &Grid::onClearSolution);
    connect(m_randomBtn, &QPushButton::clicked, m_grid, &Grid::onRandom);
    connect(m_undoBtn, &QPushButton::clicked, m_grid, &Grid::onUndo);
    connect(m_redoBtn, &QPushButton::clicked, m_grid, &Grid::onRedo);
    connect(m_editModeToggle, &ToggleSwitch::toggled, this, &MainWindow::onEditModeToggled);
    connect(m_grid->getHistory(), &History::historyStateChanged, this, &MainWindow::updateUndoRedoButtons);
    connect(m_grid, &Grid::solutionCountChanged, this, &MainWindow::updateCountLabel);
    connect(m_loadBtn, &QPushButton::clicked, this, &MainWindow::onLoadRequested);
    connect(m_saveBtn, &QPushButton::clicked, this, &MainWindow::onSaveRequested);
}

void MainWindow::onEditModeToggled(bool checked)
{
    m_editModeValueLbl->setText(checked ? "Edit clues" : "Solve");
    m_grid->onEditModeChanged(checked);
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

void MainWindow::onSaveRequested()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Save Sudoku",
                                                    "/home/linuxguy/Documents",
                                                    "Sudoku files (*.sdk);;All files (*)");

    if (!fileName.isEmpty())
    {
        if (!fileName.endsWith(".sdk"))
        {
            fileName += ".sdk";
        }
        if (SudokuSerializer::saveToFile(fileName, m_grid->getBoard(), *m_grid->getHistory()))
        {
            qDebug() << "Tallennettu onnistuneesti: " << fileName;
        }
        else
        {
            QMessageBox::critical(this, "Virhe", "Tiedoston tallennus epäonnistui.");
        }
    }
}

void MainWindow::onLoadRequested()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Avaa Sudoku",
                                                    "/home/linuxguy/Documents",
                                                    "Sudoku-tiedostot (*.sdk);;Kaikki tiedostot (*)");

    if (!fileName.isEmpty())
    {
        if (SudokuSerializer::loadFromFile(fileName, m_grid->getBoard(), *m_grid->getHistory()))
        {
            m_grid->updateUI();
            m_grid->update();
            qDebug() << "Ladattu onnistuneesti: " << fileName;
        }
        else
        {
            QMessageBox::critical(this, "Virhe", "Tiedoston lataus epäonnistui.");
        }
    }
}
