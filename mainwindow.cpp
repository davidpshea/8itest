#include "mainwindow.h"
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QSlider>
#include <QFileDialog>
#include <QDebug>
#include <QGroupBox>
#include <QFormLayout>

#include "debayered.h"
#include "imagelabel.h"
#include "removebackground.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(createLayoutWidget());

loadButtonClicked();
}

QWidget* MainWindow::createLayoutWidget()
{
    QWidget* rootWidget = new QWidget;

    QHBoxLayout* rootLayout = new QHBoxLayout (rootWidget);
    rootLayout->setAlignment(rootLayout, Qt::AlignLeft | Qt::AlignTop);

    // Controls on left
    QVBoxLayout* sidebar = new QVBoxLayout;
    sidebar->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    // Group buttons
    QGroupBox* buttonGroupBox = new QGroupBox;
    QVBoxLayout* buttonLayout = new QVBoxLayout;

    QPushButton* loadButton = new QPushButton("Load Image...");
    connect(loadButton, SIGNAL(clicked()), this, SLOT(loadButtonClicked()));
    loadButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    buttonLayout->addWidget(loadButton);

    QPushButton* saveImageButton = new QPushButton("Save Image...");
    connect(saveImageButton, SIGNAL(clicked()), this, SLOT(saveImageButtonClicked()));
    loadButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    buttonLayout->addWidget(saveImageButton);

    QPushButton* saveForgroundButton = new QPushButton("Save Forground...");
    connect(saveForgroundButton, SIGNAL(clicked()), this, SLOT(saveForgroundButtonClicked()));
    loadButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    buttonLayout->addWidget(saveForgroundButton);

    buttonGroupBox->setLayout(buttonLayout);
    sidebar->addWidget(buttonGroupBox);

    QGroupBox* thresholdGroupBox = new QGroupBox(tr("Thresholds"));
    QFormLayout* thresholdLayout = new QFormLayout;

    for (int i = 0; i < 3; i++)
    {
        threshold[i] = new QSlider(Qt::Orientation::Horizontal);
        threshold[i]->setMinimum(0);
        threshold[i]->setMaximum(100);
        threshold[i]->setValue(10);
        threshold[i]->setTracking(true);
        threshold[i]->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        connect(threshold[i], SIGNAL(valueChanged(int)), this, SLOT(thresholdSliderReleased(int)));
    }
    thresholdLayout->addRow(tr("R"), threshold[0]);
    thresholdLayout->addRow(tr("G"), threshold[1]);
    thresholdLayout->addRow(tr("B"), threshold[2]);

    thresholdGroupBox->setLayout(thresholdLayout);
    sidebar->addWidget(thresholdGroupBox);

    QLabel* label = new QLabel("Zoom Level");
    sidebar->addWidget(label);

    imageZoomLevel = new QSlider(Qt::Orientation::Horizontal);
    imageZoomLevel->setValue(20);
    imageZoomLevel->setMinimum(5);
    imageZoomLevel->setMaximum(30);
//    imageZoomLevel->setTracking(true);
    imageZoomLevel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(imageZoomLevel, SIGNAL(sliderReleased()), this, SLOT(imageZoomLevelSliderReleased()));
    sidebar->addWidget(imageZoomLevel);

    QHBoxLayout* imagesLayout = new QHBoxLayout;
    imagesLayout->setAlignment(/*rootLayout,*/ Qt::AlignLeft | Qt::AlignTop);


    inputImageLabel = new QLabel;
    inputImageLabel->setMaximumSize(imageWidth, imageHeight);
    inputImageLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    imagesLayout->addWidget(inputImageLabel);

    outputImageLabel = new QLabel;
    outputImageLabel->setMaximumSize(imageWidth, imageHeight);
    outputImageLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    imagesLayout->addWidget(outputImageLabel);

    rootLayout->addLayout(sidebar);
    rootLayout->addLayout(imagesLayout);

    QImage* backdropImage = new QImage("./givenfiles/CleanPlate.png");
    backgroundImage = DebayerImageRGGB(backdropImage);

    return rootWidget;
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

        outputImage = removeBackground(inputImage, backgroundImage, threshold[0]->value());

        updateUIWithNewImages();
    }
}

void MainWindow::saveImageButtonClicked()
{
}

void MainWindow::saveForgroundButtonClicked()
{
}

void MainWindow::updateUIWithNewImages()
{
    if (inputImage != nullptr)
    {
        QPixmap picture = QPixmap::fromImage(* inputImage);
        QPixmap pictureScaled = picture.scaled(imageWidth, imageHeight);
        inputImageLabel->setPixmap(pictureScaled);
        inputImageLabel->setMaximumSize(imageWidth, imageHeight);
    }

    if (outputImage != nullptr)
    {
        QPixmap outputPicture = QPixmap::fromImage(* outputImage);
        QPixmap outputPictureScaled = outputPicture.scaled(imageWidth, imageHeight);
        outputImageLabel->setPixmap(outputPictureScaled);
        outputImageLabel->setMaximumSize(imageWidth, imageHeight);
    }
}
/*void MainWindow::ProcessImage()
{
//    qInfo() << "Process...";

//    outputImage = removeBackground(inputImage, backgroundImage);

//    updateUIWithNewImages();
}
 */

void MainWindow::thresholdSliderReleased(int newValue)
{
//    qInfo() << "thresholdSliderReleased";
//    qInfo() << newValue;

    outputImage = removeBackground(inputImage, backgroundImage, newValue);
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
