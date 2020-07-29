#include "mainwindow.h"
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
#if 1
    setCentralWidget(createLayoutWidget());
#else
    QWidget *central_widget = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout(central_widget);

 QWidget *window = new QWidget;
    QPushButton *button1 = new QPushButton("One");
    QPushButton *button2 = new QPushButton("Two");
    QPushButton *button3 = new QPushButton("Three");
    QPushButton *button4 = new QPushButton("Four");
    QPushButton *button5 = new QPushButton("Five");

//    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(button1);
    layout->addWidget(button2);
    layout->addWidget(button3);
    layout->addWidget(button4);
    layout->addWidget(button5);
//    setLayout(layout);
setCentralWidget(central_widget);

//    setLayout();
#endif
}

QWidget* MainWindow::createLayoutWidget()
{
    QWidget *central_widget = new QWidget;
    QGridLayout* lay = new QGridLayout(central_widget);

    display = new QLineEdit("XX");
     display->setReadOnly(false);
  //   display->setAlignment(Qt::AlignRight);
     display->setMaxLength(3);
     QFont font = display->font();
     font.setPointSize(font.pointSize() + 8);     
     display->setFont(font);

//    QLabel* right_label = new QLabel("RIGHT");
//    right_label->setAlignment(Qt::AlignCenter);

    QPushButton* loadButton = new QPushButton("Load", this);
    connect(loadButton, SIGNAL(clicked()), this, SLOT(loadButtonClicked()));

    lay->addWidget(loadButton, 0, 0);
    lay->addWidget(display, 1, 0);

    inputImageLabel = new QLabel ("In");
    outputImageLabel = new QLabel ("Out");

    QPixmap* pic = new QPixmap ("./givenfiles/Bottle.png");    
    QPixmap picScaled = pic->scaled(imageWidth, imageHeight);

    inputImageLabel->setPixmap(picScaled);
    inputImageLabel->setMaximumSize(imageWidth, imageHeight);

    outputImageLabel->setPixmap(picScaled);
    outputImageLabel->setMaximumSize(imageWidth, imageHeight);

    lay->addWidget(inputImageLabel, 0, 1);
    lay->addWidget(outputImageLabel, 1, 1);

    return central_widget;
}


void MainWindow::loadButtonClicked()
{
    QPixmap* pic = new QPixmap ("./givenfiles/Barbie.png");    
    QPixmap picScaled = pic->scaled(imageWidth, imageHeight);
    inputImageLabel->setPixmap(picScaled);
}


MainWindow::~MainWindow()
{
}

