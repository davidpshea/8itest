#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QWidget>
#include <QLabel>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class ImageLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ImageLabel();

    void setImage(QImage* newImage);
    void setImageWidth(int newWidth);

private:
    void rescale(int newWidth);

    QImage* image = nullptr;
    int imageWidth = 0;
};
#endif
