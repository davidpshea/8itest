#include "mainwindow.h"
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QSlider>
#include <QFileDialog>
#include <QDebug>


#include "debayered.h"
#include "imagelabel.h"
#include "removebackground.h"

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

    #if 0
    display = new QLineEdit("XX");
     display->setReadOnly(false);
  //   display->setAlignment(Qt::AlignRight);
     display->setMaxLength(3);
     QFont font = display->font();
     font.setPointSize(font.pointSize() + 8);     
     display->setFont(font);
    #endif

//    QLabel* right_label = new QLabel("RIGHT");
//    right_label->setAlignment(Qt::AlignCenter);

    QPushButton* loadButton = new QPushButton("Load", this);
    connect(loadButton, SIGNAL(clicked()), this, SLOT(loadButtonClicked()));
//    QSize size = loadButton->
    loadButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    lay->addWidget(loadButton, 0, 0);
//    lay->addWidget(display, 1, 0);

    threshold = new QSlider(Qt::Orientation::Horizontal);
    threshold->setMinimum(0);
    threshold->setMaximum(100);
    threshold->setValue(10);
    threshold->setTracking(true);
    threshold->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(threshold, SIGNAL(valueChanged(int)), this, SLOT(thresholdSliderReleased(int)));



    lay->addWidget(threshold, 1, 0);

    imageZoomLevel = new QSlider(Qt::Orientation::Horizontal);
    imageZoomLevel->setValue(20);
    imageZoomLevel->setMinimum(5);
    imageZoomLevel->setMaximum(30);
    imageZoomLevel->setTracking(true);
    imageZoomLevel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(imageZoomLevel, SIGNAL(sliderReleased()), this, SLOT(imageZoomLevelSliderReleased()));
    lay->addWidget(imageZoomLevel, 2, 0);

    inputImageLabel = new QLabel;
    outputImageLabel = new QLabel;

    QImage* backdropImage = new QImage("./givenfiles/CleanPlate.png");
    backgroundImage = DebayerImageRGGB(backdropImage);
 //   QPixmap picScaled = pic->scaled(imageWidth, imageHeight);

  //  inputImageLabel->setPixmap(picScaled);
    inputImageLabel->setMaximumSize(imageWidth, imageHeight);

//    inputImageLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);


    //outputImageLabel->setPixmap(picScaled);
    outputImageLabel->setMaximumSize(imageWidth, imageHeight);

    lay->addWidget(inputImageLabel, 0, 1);
    lay->addWidget(outputImageLabel, 1, 1);


    return central_widget;
}


void MainWindow::loadButtonClicked()
{
    // Ask user for a file to load
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Select Input Image"), ".", tr("Image Files (*.png *.jpg *.bmp)"));

    if (! fileName.isNull())
    {
        QImage* loadedImage = new QImage(fileName);
        inputImage = DebayerImageRGGB(loadedImage);

        updateUIWithNewImages();
    }
}

void MainWindow::updateUIWithNewImages()
{
//        qInfo() << "updateUIWithNewImages...";

    if (inputImage != nullptr)
    {
//        qInfo() << "inputImage...";

        QPixmap picture = QPixmap::fromImage(* inputImage);
        QPixmap pictureScaled = picture.scaled(imageWidth, imageHeight);
        inputImageLabel->setPixmap(pictureScaled);
        inputImageLabel->setMaximumSize(imageWidth, imageHeight);
    }

    if (outputImage != nullptr)
    {
//        qInfo() << "outputImage...";

        QPixmap outputPicture = QPixmap::fromImage(* outputImage);
        QPixmap outputPictureScaled = outputPicture.scaled(imageWidth, imageHeight);
        outputImageLabel->setPixmap(outputPictureScaled);
        outputImageLabel->setMaximumSize(imageWidth, imageHeight);
    }
}
void MainWindow::ProcessImage()
{
    qInfo() << "Process...";

//    outputImage = removeBackground(inputImage, backgroundImage);

//    updateUIWithNewImages();
}
 
void MainWindow::thresholdSliderReleased(int newValue)
{
    qInfo() << "thresholdSliderReleased";
    qInfo() << threshold->value();

    outputImage = removeBackground(inputImage, backgroundImage, threshold->value());

    updateUIWithNewImages();

}

void MainWindow::imageZoomLevelSliderReleased()
{
    float zoomLevel = static_cast<float>(imageZoomLevel->value()) / 100.0f;

    imageWidth = static_cast<int>(640.0f * 2.0f *  zoomLevel);
    imageHeight = static_cast<int>(480.0f * 2.0f * zoomLevel);

    qInfo() << imageZoomLevel->value() << zoomLevel << imageWidth << imageHeight;

    updateUIWithNewImages();
}

MainWindow::~MainWindow()
{
}
