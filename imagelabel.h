#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

class ImageLabel : public QWidget
{
    Q_OBJECT

public:
    explicit ImageLabel(QWidget* parent = 0);
    const QPixmap pixmap() const;

public slots:
    void setPixmap(const QPixmap& pixmap);

protected:
    void resizeEvent(QResizeEvent* event);

private slots:
    void resizeImage();

private:
    QLabel* label;
};
