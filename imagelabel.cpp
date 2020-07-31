#include <QLabel>
#include <QDebug>
#include <QResizeEvent>

#include "imagelabel.h"

ImageLabel::ImageLabel() :
    QLabel(nullptr)
{
   setScaledContents(false);
   setMinimumSize(32, 32);
}

void ImageLabel::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    rescale();
}

QSize ImageLabel::sizeHint() const
{
    int labelWidth = width();
    return QSize(labelWidth, heightForWidth(labelWidth));
}

int ImageLabel::heightForWidth(int width) const
{
    if ((image == nullptr) || (image->isNull()))
    {
        return height();
    }

    return static_cast<int>(static_cast<qreal>(image->height() * width) / static_cast<qreal>(image->width()));
}

void ImageLabel::rescale()
{
    if ((image != nullptr) && (! image->isNull()))
    {
        QImage scaled = image->scaled(this->size(), Qt::KeepAspectRatio);
        QPixmap picture = QPixmap::fromImage(scaled);
        setPixmap(picture);
    }
}

void ImageLabel::setImage(QImage* newImage)
{
    if ((newImage != nullptr) && (! newImage->isNull()))
    {
        image = newImage;
        rescale();
    }
}
