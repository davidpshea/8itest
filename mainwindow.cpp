#include "mainwindow.h"
//#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QSlider>
#include <QFileDialog>
#include <QDebug>
#include <QGroupBox>
//#include <QFormLayout>
#include <QTransform>
#include <QStyleOption>
#include <QToolTip>
#include <QStyleOptionSlider>

#include "debayered.h"
#include "removebackground.h"
#include "imagelabel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(createLayoutWidget());
}

QWidget* MainWindow::createLayoutWidget()
{
    QWidget* rootWidget = new QWidget;

    QHBoxLayout* rootLayout = new QHBoxLayout (rootWidget);
    rootLayout->setAlignment(rootLayout, Qt::AlignLeft | Qt::AlignTop);
//    rootLayout->setStretch(0, 0);

    // Controls on left
    QVBoxLayout* sidebar = new QVBoxLayout;
    rootLayout->addLayout(sidebar);
    sidebar->setAlignment(Qt::AlignLeft | Qt::AlignTop);
//    sidebar->setStretch(0, 0);

    // Group buttons together
    QGroupBox* buttonGroupBox = new QGroupBox;
    QVBoxLayout* buttonLayout = new QVBoxLayout;
    buttonLayout->setAlignment(Qt::AlignRight | Qt::AlignTop);
//    buttonLayout->setStretch(0, 0);

    QPushButton* loadButton = new QPushButton("Load Image...");
    connect(loadButton, SIGNAL(clicked()), this, SLOT(loadImageButtonClicked()));
    loadButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);

    buttonLayout->addWidget(loadButton, 0);

    QPushButton* loadBackdropButton = new QPushButton("Load Backdrop...");
    connect(loadBackdropButton, SIGNAL(clicked()), this, SLOT(loadBackgroundImageButtonClicked()));
    loadButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    buttonLayout->addWidget(loadBackdropButton, 0);

    QPushButton* saveImageButton = new QPushButton("Save Image...");
    connect(saveImageButton, SIGNAL(clicked()), this, SLOT(saveImageButtonClicked()));
    loadButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    buttonLayout->addWidget(saveImageButton, 0);

    QPushButton* saveForgroundButton = new QPushButton("Save Forground...");
    connect(saveForgroundButton, SIGNAL(clicked()), this, SLOT(saveForgroundButtonClicked()));
    loadButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    buttonLayout->addWidget(saveForgroundButton, 0);

    QPushButton* rotateButton = new QPushButton("Rotate 90");
    connect(rotateButton, SIGNAL(clicked()), this, SLOT(rotateButtonClicked()));
    rotateButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    buttonLayout->addWidget(rotateButton, 0);

//    buttonGroupBox->setLayout(buttonLayout);
//    sidebar->addWidget(buttonGroupBox, 0);
    sidebar->addLayout(buttonLayout);

    QGroupBox* thresholdGroupBox = new QGroupBox(tr("Threshold"));
    QVBoxLayout* thresholdLayout = new QVBoxLayout;

    thresholdSlider = new QSlider(Qt::Orientation::Horizontal);
    thresholdSlider->setMinimum(0);
    thresholdSlider->setMaximum(100);
    thresholdSlider->setValue(10);
    thresholdSlider->setTracking(true);
    thresholdSlider->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    connect(thresholdSlider, SIGNAL(valueChanged(int)), this, SLOT(thresholdSliderChanged(int)));

    thresholdLayout->addWidget(thresholdSlider);

    thresholdGroupBox->setLayout(thresholdLayout);
    sidebar->addWidget(thresholdGroupBox);

    QGroupBox* zoomGroupBox = new QGroupBox(tr("Zoom"));
    QVBoxLayout* zoomLayout = new QVBoxLayout;

    imageZoomLevelSlider = new QSlider(Qt::Orientation::Horizontal);
    imageZoomLevelSlider->setMinimum(128);
    imageZoomLevelSlider->setMaximum(512);
    imageZoomLevelSlider->setTracking(true);
    imageZoomLevelSlider->setValue(imageWidth);
    imageZoomLevelSlider->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    connect(imageZoomLevelSlider, SIGNAL(valueChanged(int)), this, SLOT(imageZoomLevelSliderChanged(int)));
    zoomLayout->addWidget(imageZoomLevelSlider);

    zoomGroupBox->setLayout(zoomLayout);
    sidebar->addWidget(zoomGroupBox);

    QHBoxLayout* imagesLayout = new QHBoxLayout;
    rootLayout->addLayout(imagesLayout);
    imagesLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    inputImageLabel = new ImageLabel;
    inputImageLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    imagesLayout->addWidget(inputImageLabel);

    outputImageLabel = new ImageLabel;
    outputImageLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    imagesLayout->addWidget(outputImageLabel);

    return rootWidget;
}

void MainWindow::removeBackgroundFromImage()
{
    outputImage = removeBackground(inputImage, backgroundImage, thresholdSlider->value(), qRgb(32, 32, 32));
}

void MainWindow::loadImageButtonClicked()
{
    // Ask user for a file to load
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Select Input Image"), "./givenimages", tr("Image Files (*.png)")
    );

    if (! filename.isNull())
    {
//        inputImageFilename = filename;
    //    loadInputImage();
        inputImage = loadImage(filename);

        removeBackgroundFromImage();
        updateUIWithNewImages();
    }
/*        // load the selected image
        QImage* loadedImage = new QImage(inputImageFilename);
        inputImage = DebayerImageRGGB(loadedImage);
*/
        // reset background to match loaded image
//    loadBackgroundImage();

    //inputImage = rotateImage(inputImage, imageRotation);


//    }
}

void MainWindow::loadBackgroundImageButtonClicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Select Clean background Image"), "./givenfiles", tr("Image Files (*.png)"));

    if (! filename.isNull())
    {
//        backdropImageFilename = filename;
//        loadBackgroundImage();
        backgroundImage = loadImage(filename);

//        loadInputImage();
        removeBackgroundFromImage();
        updateUIWithNewImages();
    }

//    inputImage = rotateImage(inputImage, imageRotation);


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

QImage* MainWindow::loadImage(const QString& filename)
{
    QImage* image = new QImage(filename);
    QImage* debayeredImage = DebayerImageRGGB(image);
    return /*image = */rotateImage(debayeredImage, imageRotation);

//    return image;
}

#if 0
void MainWindow::loadInputImage()
{
    inputImage = loadImage(inputImageFilename);
/*
    QImage* image = new QImage(inputImageFilename);

    inputImage = ((image == nullptr) || image->isNull()) ?
        nullptr : DebayerImageRGGB(image);
*/
}

void MainWindow::loadBackgroundImage()
{
    backgroundImage = loadImage(backdropImageFilename);
/*
    QImage* image = new QImage(backdropImageFilename);

    backgroundImage = ((image == nullptr) || image->isNull()) ?
        nullptr : DebayerImageRGGB(image);
*/
/*
    {
        backgroundImage = nullptr;
    }
    else
    {
        backgroundImage = DebayerImageRGGB(backdropImage);
    }*/
}
#endif

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

QImage* MainWindow::rotateImage(QImage* image, const int angle)
{
    if ((image == nullptr) || image->isNull())
    {
        return nullptr;
    }

//    qInfo() << "rotateImage";

    QTransform transform;
    transform.translate(image->width()/2, image->height()/2);
    transform.rotate(static_cast<qreal>(angle)); 
    QImage transformedImage = image->transformed(transform);

    return new QImage (transformedImage);
}

void MainWindow::rotateButtonClicked()
{
    imageRotation = (imageRotation + 90) % 360;
    qInfo() << imageRotation;

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
        inputImageLabel->setImageWidth(imageWidth);
        inputImageLabel->setImage(inputImage);
    }

    if ((outputImage != nullptr) && (! outputImage->isNull()))
    {
        // Do same for processed image
        outputImageLabel->setImageWidth(imageWidth);
        outputImageLabel->setImage(outputImage);
    }

    adjustSize();
}

void MainWindow::thresholdSliderChanged(int newValue)
{
    removeBackgroundFromImage();
    updateUIWithNewImages();
}

void MainWindow::imageZoomLevelSliderChanged(int newValue)
{
    imageWidth = newValue;
    updateUIWithNewImages();
}

MainWindow::~MainWindow()
{
}
