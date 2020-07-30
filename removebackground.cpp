#include <cstdlib>

#include <QImage>
#include <QPixmap>
#include <QDebug>

QImage* removeBackground(const QImage* inputImage, const QImage* backgroundImage, int threshold[3])
{
    int width = inputImage->width();
    int height = inputImage->height();

    qInfo() << "removeBackground";
    qInfo() << width << height << inputImage->depth();

    QImage* outputImage = new QImage(width, height, QImage::Format_RGB32);

    for (int y = 0; y < height; y++)
    {
        const QRgb* inputScanLine = (QRgb*) inputImage->constScanLine(y);
        const QRgb* backgroundScanLine = (QRgb*) backgroundImage->constScanLine(y);

        QRgb* outputRow = (QRgb*) outputImage->constScanLine(y);

        for (int x = 0; x < width; x++)
        {
            QRgb inputPixel = inputScanLine[x];
            QRgb backgroundPixel = backgroundScanLine[x];

            int backgroundRed   = qRed(backgroundPixel);
            int backgroundGreen = qGreen(backgroundPixel);
            int backgroundBlue  = qBlue(backgroundPixel);

            int inputRed   = qRed(inputPixel);
            int inputGreen = qGreen(inputPixel);
            int inputBlue  = qBlue(inputPixel);

            if (
                (abs(backgroundRed - inputRed) <= threshold[0]) &&
                (abs(backgroundGreen - inputGreen) <= threshold[1]) &&
                (abs(backgroundBlue - inputBlue) <= threshold[2])
            )
            {
                outputRow[x] = qRgb(255, 0, 0);
            }
            else
            {
                outputRow[x] = inputPixel;
            }
        }
    }

    return outputImage;
}
