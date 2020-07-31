#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <QWidget>
//#include <QLineEdit>

QT_BEGIN_NAMESPACE
class QLabel;
class QWidget;
class ImageLabel;
class QSlider;
class QVBoxLayout;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

//    virtual void resizeEvent(QResizeEvent *ev) override;

private:
    QWidget* createLayoutWidget();

    void updateUIWithNewImages();
//    QImage* loadImage(const QString& filename);
//    void loadInputImage();
//    void loadBackgroundImage();
    QImage* loadImage(const QString& title);
    void removeBackgroundFromImage();

    void saveImage(const QImage* image, const QString& title);
    QImage* rotateImage(QImage* image, const int angle = 90);

    ImageLabel* /*QLabel**/ inputImageLabel;
    ImageLabel* /*QLabel**/ outputImageLabel;

    QImage* inputImage = nullptr;
    QImage* outputImage = nullptr;
    QImage* backgroundImage = nullptr;

//    QString inputImageFilename  = nullptr;
//    QString backdropImageFilename  = nullptr;
    QVBoxLayout* sidebar;

    QSlider* imageZoomLevelSlider;
    QSlider* thresholdSlider;

    int imageWidth = 320;
    int imageRotation = 0;

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
