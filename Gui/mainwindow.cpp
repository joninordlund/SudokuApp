#include "mainwindow.h"
#include "grid.h"
#include "sudokuserializer.h"
#include <QApplication>
#include <QCheckBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QPdfWriter>
#include <QStyle>
#include <QVBoxLayout>

const int areaSize = 630;
// const int areaSize = 540;

MainWindow::MainWindow(QWidget* parent) :
    QWidget{parent}, m_difficulty(1), m_grid(nullptr)
{
    setFixedSize(1560, 740);
    // setFixedSize(1380, 640);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QWidget* centerArea = new QWidget;
    // centerArea->setObjectName("center");
    // centerArea->setStyleSheet("");
    QHBoxLayout* centerLayout = new QHBoxLayout(centerArea);
    centerLayout->setSpacing(20);
    centerLayout->setContentsMargins(0, 0, 0, 0);

    centerLayout->addWidget(createLeftControls());
    centerLayout->addWidget(createSudokuArea());
    centerLayout->addWidget(createImageArea());
    centerLayout->addWidget(createImageControls());

    mainLayout->addWidget(centerArea);
    mainLayout->addLayout(createBottomBar());

    onEditModeToggled(m_editModeToggle->isChecked());

    connectSignals();
    qApp->installEventFilter(this);
    updateUI(false);
}

QWidget* MainWindow::createLeftControls()
{
    const int spacing = 7;
    QWidget* controlArea = new QWidget;
    controlArea->setFixedWidth(96);
    QVBoxLayout* layout = new QVBoxLayout(controlArea);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);

    // File section
    QLabel* fileLbl = new QLabel("File");
    fileLbl->setObjectName("sectionLabel");
    m_loadBtn = new QPushButton("Load");
    m_saveBtn = new QPushButton("Save");
    m_toPDFBtn = new QPushButton("To PDF");

    layout->addWidget(fileLbl);
    layout->addWidget(m_loadBtn);
    layout->addWidget(m_saveBtn);
    layout->addWidget(m_toPDFBtn);
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

    m_undoBtn = new QPushButton();
    m_undoBtn->setIcon(QIcon(":/icons/icons/undo.svg"));
    m_undoBtn->setIconSize(QSize(24, 24));
    m_undoBtn->setFixedSize(35, 35);
    m_undoBtn->setObjectName("undo");
    m_redoBtn = new QPushButton();
    m_redoBtn->setIcon(QIcon(":/icons/icons/redo.svg"));
    m_redoBtn->setFixedSize(35, 35);
    m_redoBtn->setIconSize(QSize(24, 24));
    m_redoBtn->setObjectName("redo");
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
    m_generateBtn = new QPushButton("Generate");
    // QWidget* starWidget = new QWidget;
    QHBoxLayout* starLayout = setupStars();

    starLayout->setContentsMargins(0, 0, 0, 0);

    // m_clearBtn->setObjectName("colorRed");
    // m_randomBtn->setObjectName("colorRed");
    layout->addWidget(m_clearBtn);
    layout->addWidget(m_randomBtn);

    QVBoxLayout* genGroup = new QVBoxLayout();
    genGroup->setSpacing(2);
    genGroup->addWidget(m_generateBtn);
    genGroup->addLayout(starLayout);
    layout->addLayout(genGroup);
    updateStars(m_difficulty);
    // layout->addSpacing(spacing);

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

    m_leftBrowseBtn = new QPushButton();
    m_leftBrowseBtn->setIcon(QIcon(":/icons/icons/chevron_left.svg"));
    m_leftBrowseBtn->setIconSize(QSize(24, 24));
    m_leftBrowseBtn->setFixedSize(35, 35);
    m_leftBrowseBtn->setObjectName("leftBrowse");
    m_rightBrowseBtn = new QPushButton();
    m_rightBrowseBtn->setIcon(QIcon(":/icons/icons/chevron_right.svg"));
    m_rightBrowseBtn->setIconSize(QSize(24, 24));
    m_rightBrowseBtn->setFixedSize(35, 35);
    m_rightBrowseBtn->setObjectName("rightBrowse");

    arrowLayout->addWidget(m_leftBrowseBtn);
    arrowLayout->addWidget(m_rightBrowseBtn);
    m_solIndexLabel = new QLabel("0 / 0");
    m_solIndexLabel->setAlignment(Qt::AlignCenter);
    m_solIndexLabel->setStyleSheet("color: #2c3e50; font-weight: bold; font-size: 14px;");
    layout->addWidget(arrowWidget);
    layout->addWidget(m_solIndexLabel);
    layout->addStretch();

    return controlArea;
}

QWidget* MainWindow::createSudokuArea()
{
    QWidget* leftSquare = new QWidget;
    leftSquare->setFixedSize(areaSize, areaSize);

    QHBoxLayout* layout = new QHBoxLayout(leftSquare);
    m_grid = new Grid(leftSquare);
    layout->addWidget(m_grid);
    layout->setContentsMargins(0, 0, 0, 0);

    return leftSquare;
}

QWidget* MainWindow::createImageArea()
{
    QFrame* rightSquare = new QFrame;
    rightSquare->setFixedSize(areaSize, areaSize);
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
    // m_setImgToGrid->setObjectName("colorGreen");

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

    // m_solCountLbl = new QLabel("Solution count: ");

    QLabel* editModeLabel = new QLabel("Mode:");
    m_editModeValueLbl = new QLabel("Solve");

    // buttonLayout->addWidget(m_solCountLbl);
    // buttonLayout->addSpacing(40);
    buttonLayout->addWidget(editModeLabel);
    buttonLayout->addWidget(m_editModeValueLbl);
    buttonLayout->addStretch();

    return buttonLayout;
}

QHBoxLayout* MainWindow::setupStars()
{
    QHBoxLayout* starLayout = new QHBoxLayout();

    for (int i = 1; i <= 5; i++)
    {
        QToolButton* star = new QToolButton();
        star->setText("\u2605"); // star
        star->setCheckable(true);
        m_starButtons.append(star);
        starLayout->addWidget(star);

        connect(star, &QToolButton::clicked, this, [=]()
                { updateStars(i); });
    }
    return starLayout;
}

void MainWindow::updateStars(int level)
{
    m_difficulty = level;
    for (int i = 0; i < 5; i++)
    {
        if (i < level)
        {
            m_starButtons[i]->setStyleSheet("color: gold; font-size: 20px; border: none;");
        }
        else
        {
            m_starButtons[i]->setStyleSheet("color: gray; font-size: 20px; border: none;");
        }
    }
    if (m_grid)
    {
        m_grid->setDifficulty(level);
    }
}

void MainWindow::connectSignals()
{
    connect(m_image, &SudokuImage::newSudoku, m_grid, &Grid::newSudoku);
    connect(m_grid, &Grid::solutionCountChanged, this, &MainWindow::updateSolutionCount);
    connect(m_loadImgBtn, &QPushButton::clicked, this, &MainWindow::onLoadImage);
    connect(m_removeImgBtn, &QPushButton::clicked, m_image, &SudokuImage::onRemove);
    connect(m_peekBtn, &QPushButton::pressed, this, &MainWindow::onPeek);
    connect(m_peekBtn, &QPushButton::released, m_grid, &Grid::onHideSolution);
    connect(m_clearBtn, &QPushButton::clicked, m_grid, &Grid::onClearSolution);
    connect(m_randomBtn, &QPushButton::clicked, m_grid, &Grid::onRandom);
    connect(m_undoBtn, &QPushButton::clicked, m_grid, &Grid::onUndo);
    connect(m_redoBtn, &QPushButton::clicked, m_grid, &Grid::onRedo);
    connect(m_leftBrowseBtn, &QPushButton::clicked, m_grid, &Grid::onBrowseSolLeft);
    connect(m_rightBrowseBtn, &QPushButton::clicked, m_grid, &Grid::onBrowseSolRight);
    connect(m_lockToggle, &ToggleSwitch::toggled, this, &MainWindow::onLockToggled);
    connect(m_generateBtn, &QPushButton::clicked, this, &MainWindow::onGenerate);
    connect(m_editModeToggle, &ToggleSwitch::toggled, this, &MainWindow::onEditModeToggled);
    connect(m_toPDFBtn, &QPushButton::clicked, this, &MainWindow::onExportToPdf);
    connect(m_loadBtn, &QPushButton::clicked, this, &MainWindow::onLoadRequested);
    connect(m_saveBtn, &QPushButton::clicked, this, &MainWindow::onSaveRequested);
    // connect(m_lockToggle, &ToggleSwitch::toggled, this, [=](bool locked)
    //         { updateUI(m_editModeToggle->isChecked()); });
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_F10)
        {
            m_image->setDebugView(true);
            return true;
        }
    }
    else if (event->type() == QEvent::KeyRelease)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_F10)
        {
            m_image->setDebugView(false);
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void MainWindow::onExportToPdf()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save PDF", "", "PDF Files (*.pdf)");
    if (fileName.isEmpty())
        return;

    QPdfWriter writer(fileName);
    writer.setResolution(300);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setPageMargins(QMarginsF(30, 30, 30, 30));

    QPainter painter(&writer);

    vector<vector<CellData>> grid = m_grid->getBoard().toCellDataMatrix();
    int side = qMin(writer.width(), writer.height()) * 0.8;
    int cellSize = side / 9;
    int offsetX = (writer.width() - side) / 2;
    int offsetY = (writer.height() - side) / 2;
    int thinPenWidth = qMax(1, cellSize / 30);
    int thickPenWidth = qMax(2, cellSize / 15);
    for (int i = 0; i <= 9; i++)
    {
        int currentWidth = (i % 3 == 0) ? thickPenWidth : thinPenWidth;
        painter.setPen(QPen(Qt::black, currentWidth, Qt::SolidLine, Qt::FlatCap));

        painter.drawLine(offsetX + i * cellSize, offsetY, offsetX + i * cellSize, offsetY + side);
        painter.drawLine(offsetX, offsetY + i * cellSize, offsetX + side, offsetY + i * cellSize);
    }

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (grid[i][j].digit != 0)
            {
                painter.setFont(QFont("Arial", 20, grid[i][j].isGiven ? QFont::Bold : QFont::Normal));
                painter.setPen(grid[i][j].isGiven ? Qt::black : QColor(0, 102, 204));
                QRect rect(offsetX + j * cellSize, offsetY + i * cellSize, cellSize, cellSize);
                painter.drawText(rect, Qt::AlignCenter, QString::number(grid[i][j].digit));
            }
        }
    }
}

void MainWindow::onPeek()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_grid->onShowSolution();
    QApplication::restoreOverrideCursor();
}
void MainWindow::onLoadImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Sudoku Image"), "",
                                                    tr("Images (*.png *.jpg *.jpeg *.bmp)"));

    if (fileName.isEmpty())
    {
        return;
    };
    m_image->load(fileName);
}

void MainWindow::onGenerate()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_grid->onGenerate();
    QApplication::restoreOverrideCursor();
}

void MainWindow::onLockToggled(bool locked)
{
    updateUI(m_editModeToggle->isChecked());
    m_grid->onSolutionLocked(locked);
}

void MainWindow::onEditModeToggled(bool checked)
{
    m_editModeValueLbl->setText(checked ? "Edit clues" : "Solve");
    m_grid->onEditModeChanged(checked);
    m_lockToggle->setChecked(checked && m_lockToggle->isChecked());
    updateUI(checked);
}

void MainWindow::updateSolutionCount(int current, int total)
{
    if (total == 0)
    {
        m_solIndexLabel->setText("0 / 0");
    }
    else
    {
        m_solIndexLabel->setText(QString("%1 / %2").arg(current).arg(total));
    }
}

void MainWindow::updateUI(bool editMode)
{
    bool lockActive = m_lockToggle->isChecked();
    m_leftBrowseBtn->setEnabled(editMode && lockActive);
    m_rightBrowseBtn->setEnabled(editMode && lockActive);

    m_randomBtn->setEnabled(editMode);
    m_lockToggle->setEnabled(editMode);

    if ((!lockActive && editMode) || !editMode)
    {
        m_peekBtn->setText("Peek");
    }
    else
    {
        m_peekBtn->setText("Reset");
    }
}

void MainWindow::updateUndoRedoButtons(bool canUndo, bool canRedo)
{
    m_undoBtn->setEnabled(canUndo);
    m_redoBtn->setEnabled(canRedo);
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
