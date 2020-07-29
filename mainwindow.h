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
    void ProcessImage();

//    QLineEdit* display;

    QLabel* inputImageLabel;
    QLabel* outputImageLabel;

    QImage* inputImage = nullptr;
    QImage* outputImage = nullptr;

    QImage* backgroundImage = nullptr;

    QSlider* imageZoomLevel;
    QSlider* threshold;

    int imageWidth = 320; //640;
    int imageHeight = 200;

private slots:
    void loadButtonClicked();
    void thresholdSliderReleased(int newValue);
    void imageZoomLevelSliderReleased();
};
#endif // MAINWINDOW_H
