#ifndef DEBAYERED_H
#define DEBAYERED_H

QT_BEGIN_NAMESPACE
class QImage;
QT_END_NAMESPACE

QImage* DebayerImageRGGB(const QImage* inputImage);

#endif
