#include "mainwindow.h"
#include <QLineEdit>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    display = new QLineEdit("0");

     display->setReadOnly(true);
     display->setAlignment(Qt::AlignRight);
     display->setMaxLength(15);

     QFont font = display->font();
     font.setPointSize(font.pointSize() + 8);
     display->setFont(font);

//     auto l = getLayout();
     QGridLayout *mainLayout = new QGridLayout;
     mainLayout->addWidget(display, 0, 0, 1, 6);
     setLayout(mainLayout);


//    setWindowTitle(tr("Image blend"));
}

MainWindow::~MainWindow()
{
}

