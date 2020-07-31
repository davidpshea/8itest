#include <QLabel>
#include <QDebug>
#include <QResizeEvent>

#include "imagelabel.h"

ImageLabel::ImageLabel() :
    QLabel(nullptr)
{
   setScaledContents(true);
}

#ifdef IN_PROGRESS
void ImageLabel::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    qInfo() << "RESIZE img";

    int width = event->size().width();
    qInfo() << event->size().width() << event->size().height();
    setImageWidth(width);
//    resizeImage();
}
#endif

void ImageLabel::setImageWidth(int newWidth)
{
    imageWidth = newWidth;
    rescale(imageWidth);
}

void ImageLabel::rescale(int newWidth)
{
    if ((image != nullptr) && (! image->isNull()))
    {
        QImage scaled = image->scaledToWidth(newWidth);
        QPixmap picture = QPixmap::fromImage(scaled);
        setPixmap(picture);
        setMaximumSize(newWidth, scaled.height());
    }
}

void ImageLabel::setImage(QImage* newImage)
{
    if ((newImage != nullptr) && (! newImage->isNull()))
    {
        image = newImage;
        rescale(imageWidth);
    }
}
