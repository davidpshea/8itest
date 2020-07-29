#include <QLabel>

#include "imagelabel.h"

ImageLabel::ImageLabel(QWidget* parent) :
    QWidget(parent)
{
    label = new QLabel(this);
    label->setScaledContents(true);
    label->setFixedSize(100, 100);
}

void ImageLabel::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    resizeImage();
}

const QPixmap ImageLabel::pixmap() const
{
    return label->pixmap(Qt::ReturnByValue);
}

void ImageLabel::setPixmap(const QPixmap& pixmap)
{
    label->setPixmap(pixmap);
//    resizeImage();
}

void ImageLabel::resizeImage()
{
    QSize pixSize = pixmap().size();
    pixSize.scale(size(), Qt::KeepAspectRatio);
    label->setFixedSize(pixSize);
}