#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QLabel>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class ImageLabel : public QLabel
{
    Q_OBJECT

public:
    ImageLabel();

    void setImage(QImage* newImage);

protected:
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual int heightForWidth(int width) const override;
    virtual QSize sizeHint() const override;

private:
    void rescale();

    QImage* image = nullptr;
};
#endif
