#include "drawboard.h"
#include "filelist.h"
#include "mainwindow.h"

#include <QLabel>
#include <QSplitter>
#include <QVBoxLayout>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto fileList = new FileList();
    auto drawingBoard = new DrawingBoard();

    auto splitter = new QSplitter;

    QWidget* filesSide = new QWidget;
    QVBoxLayout* filesLayout = new QVBoxLayout();
    filesLayout->addWidget(new QLabel(tr("Select file here:")));
    filesLayout->addWidget(fileList);
    filesSide->setLayout(filesLayout);
    filesSide->setMinimumWidth(200);
    filesSide->setMaximumWidth(600);

    splitter->addWidget(filesSide);
    splitter->addWidget(drawingBoard);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    QStatusBar* statusBar = new QStatusBar();
    statusBar->setFixedHeight(20);
    mainLayout->addWidget(splitter);
    mainLayout->addWidget(statusBar);
    QWidget* mainWidget = new QWidget();
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);

    connect(fileList, &FileList::itemSelectionChanged,
            [fileList, drawingBoard, statusBar]() {
        auto selected = fileList->selectedItems();
        if (!selected.isEmpty()) {
            const QString file_name = selected.first()->text();
            drawingBoard->setFile( file_name );
            statusBar->showMessage(QString("File is loaded: %1").arg(file_name));
        }
    });

    statusBar->showMessage("Please select SSD file ...");
}

