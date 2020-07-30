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
#include <QTransform>

#include "debayered.h"
#include "removebackground.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(createLayoutWidget());

//loadButtonClicked();
}

QWidget* MainWindow::createLayoutWidget()
{
    QWidget* rootWidget = new QWidget;

    QHBoxLayout* rootLayout = new QHBoxLayout (rootWidget);
    rootLayout->setAlignment(rootLayout, Qt::AlignLeft | Qt::AlignTop);

    // Controls on left
    QVBoxLayout* sidebar = new QVBoxLayout;
    sidebar->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    // Group buttons together
    QGroupBox* buttonGroupBox = new QGroupBox;
    QVBoxLayout* buttonLayout = new QVBoxLayout;

    QPushButton* loadButton = new QPushButton("Load Image...");
    connect(loadButton, SIGNAL(clicked()), this, SLOT(loadImageButtonClicked()));
    loadButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    buttonLayout->addWidget(loadButton);

    QPushButton* loadBackdropButton = new QPushButton("Load Backdrop...");
    connect(loadBackdropButton, SIGNAL(clicked()), this, SLOT(loadBackgroundImageButtonClicked()));
    loadButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    buttonLayout->addWidget(loadBackdropButton);


    QPushButton* saveImageButton = new QPushButton("Save Image...");
    connect(saveImageButton, SIGNAL(clicked()), this, SLOT(saveImageButtonClicked()));
    loadButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    buttonLayout->addWidget(saveImageButton);

    QPushButton* saveForgroundButton = new QPushButton("Save Forground...");
    connect(saveForgroundButton, SIGNAL(clicked()), this, SLOT(saveForgroundButtonClicked()));
    loadButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    buttonLayout->addWidget(saveForgroundButton);

    QPushButton* rotateButton = new QPushButton("Rotate 90");
    connect(rotateButton, SIGNAL(clicked()), this, SLOT(rotateButtonClicked()));
    rotateButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    buttonLayout->addWidget(rotateButton);

    buttonGroupBox->setLayout(buttonLayout);
    sidebar->addWidget(buttonGroupBox);

    QGroupBox* thresholdGroupBox = new QGroupBox(tr("Threshold"));
    QVBoxLayout* thresholdLayout = new QVBoxLayout;

    thresholdSlider = new QSlider(Qt::Orientation::Horizontal);
    thresholdSlider->setMinimum(0);
    thresholdSlider->setMaximum(100);
    thresholdSlider->setValue(10);
    thresholdSlider->setTracking(true);
    thresholdSlider->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    connect(thresholdSlider, SIGNAL(valueChanged(int)), this, SLOT(thresholdSliderChanged(int)));

    thresholdLayout->addWidget(thresholdSlider);

    thresholdGroupBox->setLayout(thresholdLayout);
    sidebar->addWidget(thresholdGroupBox);

    QGroupBox* zoomGroupBox = new QGroupBox(tr("Zoom"));
    QVBoxLayout* zoomLayout = new QVBoxLayout;

    imageWidth = 640;

    imageZoomLevel = new QSlider(Qt::Orientation::Horizontal);
    imageZoomLevel->setValue(imageWidth);
    imageZoomLevel->setMinimum(64);
    imageZoomLevel->setMaximum(512);
    imageZoomLevel->setTracking(true);
    imageZoomLevel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(imageZoomLevel, SIGNAL(valueChanged(int)), this, SLOT(imageZoomLevelSliderChanged(int)));
//    connect(imageZoomLevel, SIGNAL(sliderReleased()), this, SLOT(imageZoomLevelSliderReleased()));
//    sidebar->addWidget(imageZoomLevel);
    zoomLayout->addWidget(imageZoomLevel);

    zoomGroupBox->setLayout(zoomLayout);
    sidebar->addWidget(zoomGroupBox);

    QHBoxLayout* imagesLayout = new QHBoxLayout;
    imagesLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    inputImageLabel = new QLabel;
//    inputImageLabel->setMaximumSize(imageWidth, imageHeight);
    inputImageLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    imagesLayout->addWidget(inputImageLabel);

    outputImageLabel = new QLabel;
//    outputImageLabel->setMaximumSize(imageWidth, imageHeight);
    outputImageLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    imagesLayout->addWidget(outputImageLabel);

    rootLayout->addLayout(sidebar);
    rootLayout->addLayout(imagesLayout);

    return rootWidget;
}

void MainWindow::removeBackgroundFromImage()
{
    outputImage = removeBackground(inputImage, backgroundImage, thresholdSlider->value(), qRgb(32, 32, 32));
}

void MainWindow::loadImageButtonClicked()
{
    // Ask user for a file to load
    inputImageFilename = QFileDialog::getOpenFileName(this,
        tr("Select Input Image"), "./givenimages", tr("Image Files (*.png)")
    );

    if (inputImageFilename.isNull())
    {
        inputImage = nullptr;
    }
    else
    {
        loadInputImage();
    }
/*        // load the selected image
        QImage* loadedImage = new QImage(inputImageFilename);
        inputImage = DebayerImageRGGB(loadedImage);
*/
        // reset background to match loaded image
    loadBackgroundImage();
    removeBackgroundFromImage();
    updateUIWithNewImages();
//    }
}

void MainWindow::loadBackgroundImageButtonClicked()
{
    backdropImageFilename = QFileDialog::getOpenFileName(this,
        tr("Select Clean background Image"), "./givenfiles", tr("Image Files (*.png)"));

    if (backdropImageFilename.isNull())
    {
        backgroundImage = nullptr;
    }
    else
    {
        loadBackgroundImage();
    }

    loadInputImage();
    removeBackgroundFromImage();
    updateUIWithNewImages();

/*
    backdropImage = new QImage(backdropImageFilename);

    if ((backdropImage == nullptr) || (backdropImage->isNull()))
    {
        backgroundImage = nullptr;
        return;
    }

    backgroundImage = DebayerImageRGGB(backdropImage);
*/
}

void MainWindow::loadInputImage()
{
    QImage* image = new QImage(inputImageFilename);

    inputImage = ((image == nullptr) || image->isNull()) ?
        nullptr : DebayerImageRGGB(image);
}

void MainWindow::loadBackgroundImage()
{
    QImage* image = new QImage(backdropImageFilename);

    backgroundImage = ((image == nullptr) || image->isNull()) ?
        nullptr : DebayerImageRGGB(image);
/*
    {
        backgroundImage = nullptr;
    }
    else
    {
        backgroundImage = DebayerImageRGGB(backdropImage);
    }*/
}

void MainWindow::saveImage(const QImage* image, const QString& title)
{
    if ((image != nullptr) && (! image->isNull()))
    {
        QString fileName = QFileDialog::getSaveFileName(this,
            title, ".", tr("Image Files (*.png)"));

        if (! fileName.isNull())
        {
            image->save(fileName);
        }
    }
}

QImage* MainWindow::rotateImage(QImage* image, const float angle)
{
    if ((image == nullptr) || image->isNull())
    {
        return nullptr;
    }

    QTransform transform;
    transform.translate(image->width()/2, image->height()/2);
    transform.rotate(90); 
    QImage transformedImage = image->transformed(transform);

    return new QImage (transformedImage);
}

void MainWindow::rotateButtonClicked()
{
    // rotate all 3 images to keep them in sync
    inputImage = rotateImage(inputImage);
    outputImage = rotateImage(outputImage);
    backgroundImage = rotateImage(backgroundImage);

    updateUIWithNewImages();
}

void MainWindow::saveImageButtonClicked()
{
    saveImage(inputImage, QString("Select Output image file"));
}

void MainWindow::saveForgroundButtonClicked()
{
    saveImage(outputImage, QString("Select Output forground file"));
}

void MainWindow::updateUIWithNewImages()
{
    if ((inputImage != nullptr) && (! inputImage->isNull()))
    {
        // Scale original large image into something to display
        QImage scaled = inputImage->scaledToWidth(imageWidth);
        QPixmap picture = QPixmap::fromImage(scaled);
        inputImageLabel->setPixmap(picture);
        inputImageLabel->setMaximumSize(imageWidth, scaled.height());
    }

    if ((outputImage != nullptr) && (! outputImage->isNull()))
    {
        // Do same for processed image
        QImage scaled = outputImage->scaledToWidth(imageWidth);
        QPixmap picture = QPixmap::fromImage(scaled);
        outputImageLabel->setPixmap(picture);
        outputImageLabel->setMaximumSize(imageWidth, scaled.height());
    }

    adjustSize();
}

void MainWindow::thresholdSliderChanged(int newValue)
{
    removeBackgroundFromImage();
//    outputImage = removeBackground(inputImage, backgroundImage, newValue);
    updateUIWithNewImages();
}

void MainWindow::imageZoomLevelSliderChanged(int newValue)
{
    imageWidth = newValue; //imageZoomLevel->value();
/*
    float zoomLevel = static_cast<float>(imageZoomLevel->value()) / 100.0f;

    imageWidth = static_cast<int>(640.0f * 2.0f *  zoomLevel);
    imageHeight = static_cast<int>(480.0f * 2.0f * zoomLevel);

    qInfo() << imageZoomLevel->value() << zoomLevel << imageWidth << imageHeight;
*/
    updateUIWithNewImages();
}

MainWindow::~MainWindow()
{
}
