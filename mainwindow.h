#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <QWidget>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
class QLabel;
class QWidget;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QWidget* createLayoutWidget();

    QLineEdit* display;

    QLabel* inputImageLabel;
    QLabel* outputImageLabel;

    const int imageWidth = 320; //640;
    const int imageHeight = 200; //480;

private slots:
    void loadButtonClicked();

};
#endif // MAINWINDOW_H
