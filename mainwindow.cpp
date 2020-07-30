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

    imageWidth = 640;

    imageZoomLevel = new QSlider(Qt::Orientation::Horizontal);
    imageZoomLevel->setValue(imageWidth);
    imageZoomLevel->setMinimum(64);
    imageZoomLevel->setMaximum(512);
//    imageZoomLevel->setTracking(true);
    imageZoomLevel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(imageZoomLevel, SIGNAL(sliderReleased()), this, SLOT(imageZoomLevelSliderReleased()));
    sidebar->addWidget(imageZoomLevel);

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
    int thresholds[3] = {
        threshold[0]->value(),
        threshold[1]->value(),
        threshold[2]->value()
    };

    outputImage = removeBackground(inputImage, backgroundImage, thresholds);
}

void MainWindow::loadButtonClicked()
{
    // Ask user for a file to load
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Select Input Image"), "./givenimages", tr("Image Files (*.png *.jpg *.bmp)"));

    if (! fileName.isNull())
    {
        // load the selected image
        QImage* loadedImage = new QImage(fileName);
        inputImage = DebayerImageRGGB(loadedImage);

        // reset background to match loaded image
        loadBackgroundImage();

        removeBackgroundFromImage();
#if 0
        // give us an initial processed image too
        int thresholds[3] = {
            threshold[0]->value(),
            threshold[1]->value(),
            threshold[2]->value()
        };

        outputImage = removeBackground(inputImage, backgroundImage, thresholds);
#endif
        updateUIWithNewImages();
    }
}

void MainWindow::loadBackgroundImage()
{
    QImage* backdropImage = new QImage("./givenfiles/CleanPlate.png");
    backgroundImage = DebayerImageRGGB(backdropImage);
}

void MainWindow::saveImage(const QImage* image, const QString& title)
{
    if (image != nullptr)
    {
        QString fileName = QFileDialog::getSaveFileName(this,
            title, ".", tr("Image Files (*.png *.jpg *.bmp)"));

        if (! fileName.isNull())
        {
            image->save(fileName);
        }
    }
}

QImage* MainWindow::rotateImage(QImage* image, const float angle)
{
    if (image == nullptr)
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
    if (inputImage != nullptr)
    {
        // Scale original large image into something to display
        QImage scaled = inputImage->scaledToWidth(imageWidth);
        QPixmap picture = QPixmap::fromImage(scaled);
        inputImageLabel->setPixmap(picture);
        inputImageLabel->setMaximumSize(imageWidth, scaled.height());
    }

    if (outputImage != nullptr)
    {
        // Do same for processed image
        QImage scaled = outputImage->scaledToWidth(imageWidth);
        QPixmap picture = QPixmap::fromImage(scaled);
        outputImageLabel->setPixmap(picture);
        outputImageLabel->setMaximumSize(imageWidth, scaled.height());
    }
}

void MainWindow::thresholdSliderReleased(int newValue)
{
    removeBackgroundFromImage();
//    outputImage = removeBackground(inputImage, backgroundImage, newValue);
    updateUIWithNewImages();
}

void MainWindow::imageZoomLevelSliderReleased()
{
    imageWidth = imageZoomLevel->value();
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
