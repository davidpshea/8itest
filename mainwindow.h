#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <QWidget>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
class QLabel;
class QWidget;
class ImageLabel;
class QSlider;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QWidget* createLayoutWidget();

    void updateUIWithNewImages();
    void loadInputImage();
    void loadBackgroundImage();
    void removeBackgroundFromImage();

    void saveImage(const QImage* image, const QString& title);
    QImage* rotateImage(QImage* image, const float angle = 90.0f);

    QLabel* inputImageLabel;
    QLabel* outputImageLabel;

    QImage* inputImage = nullptr;
    QImage* outputImage = nullptr;
    QImage* backgroundImage = nullptr;

    QString inputImageFilename  = nullptr;
    QString backdropImageFilename  = nullptr;

    QSlider* imageZoomLevel;
    QSlider* thresholdSlider;

    int imageWidth;

private slots:
    void loadImageButtonClicked();
    void loadBackgroundImageButtonClicked();
    void saveImageButtonClicked();
    void rotateButtonClicked();
    void saveForgroundButtonClicked();
    void thresholdSliderChanged(int newValue);
    void imageZoomLevelSliderChanged(int newValue);
};

#endif // MAINWINDOW_H
