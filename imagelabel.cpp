#include <QLabel>
#include <QDebug>
#include <QResizeEvent>

#include "imagelabel.h"

ImageLabel::ImageLabel() :
    QLabel(nullptr)
{
   setScaledContents(false);
   //setFixedSize(0, 0);
   setMinimumSize(32, 32);
//   setMaximumSize(32, 32);
}

#define IN_PROGRESS
#ifdef IN_PROGRESS
void ImageLabel::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    qInfo() << "RESIZE";
    int width = event->size().width();
    qInfo() << event->size().width() << event->size().height();
//    setImageWidth(width);

    rescale(0);
}
#endif

QSize ImageLabel::sizeHint() const
{
    int w = this->width();
    return QSize( w, heightForWidth(w) );
}

int ImageLabel::heightForWidth( int width ) const
{
    if ((image == nullptr) || (image->isNull()))
    {
        return this->height();
    }

    return ((qreal)image->height() * width) / image->width();
}

void ImageLabel::setImageWidth(int newWidth)
{
//    imageWidth = newWidth;
//    rescale(imageWidth);
}

void ImageLabel::rescale(int newWidth)
{
    if ((image != nullptr) && (! image->isNull()))
    {
//        QImage scaled = image->scaledToWidth(newWidth);

        QImage scaled = image->scaled(this->size(), Qt::KeepAspectRatio); //newWidth);


        QPixmap picture = QPixmap::fromImage(scaled);
        setPixmap(picture);
//        setMaximumSize(newWidth, scaled.height());
    }


}

void ImageLabel::setImage(QImage* newImage)
{
    if ((newImage != nullptr) && (! newImage->isNull()))
    {
        image = newImage;
        rescale(0);
    }
}
