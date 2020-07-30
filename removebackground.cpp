#include <cstdlib>

#include <QImage>
#include <QPixmap>
#include <QDebug>

QImage* removeBackground(const QImage* inputImage, const QImage* backgroundImage, int threshold, const QRgb& backgroundColour)
{
    if ((inputImage == nullptr) || (backgroundImage == nullptr) ||
        inputImage->isNull() || backgroundImage->isNull()
    )
    {
        return nullptr;
    }

    const int width = inputImage->width();
    const int height = inputImage->height();

    QImage* outputImage = new QImage(width, height, QImage::Format_RGB32);
    if ((outputImage == nullptr) || (outputImage->isNull()))
    {
        return nullptr;
    }

    for (int y = 0; y < height; y++)
    {
        const QRgb* inputScanLine = (QRgb*) inputImage->constScanLine(y);
        const QRgb* backgroundScanLine = (QRgb*) backgroundImage->constScanLine(y);

        QRgb* outputRow = (QRgb*) outputImage->constScanLine(y);

        for (int x = 0; x < width; x++)
        {
            QRgb inputPixel = inputScanLine[x];
            QRgb backgroundPixel = backgroundScanLine[x];

            const int backgroundRed   = qRed(backgroundPixel);
            const int backgroundGreen = qGreen(backgroundPixel);
            const int backgroundBlue  = qBlue(backgroundPixel);

            const int inputRed   = qRed(inputPixel);
            const int inputGreen = qGreen(inputPixel);
            const int inputBlue  = qBlue(inputPixel);

            outputRow[x] = (
                (abs(backgroundRed - inputRed) <= threshold) &&
                (abs(backgroundGreen - inputGreen) <= threshold) &&
                (abs(backgroundBlue - inputBlue) <= threshold)
            ) ? backgroundColour : inputPixel;


#if 0
            if (
                (abs(backgroundRed - inputRed) <= threshold) &&
                (abs(backgroundGreen - inputGreen) <= threshold) &&
                (abs(backgroundBlue - inputBlue) <= threshold)
            )
            {
                outputRow[x] = backgroundColour;//qRgb(255, 0, 0);
            }
            else
            {
                outputRow[x] = inputPixel;
            }
#endif
        }
    }

    return outputImage;
}
