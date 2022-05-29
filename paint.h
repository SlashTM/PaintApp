#ifndef PAINT_H
#define PAINT_H

#include <QWidget>
#include <QColor>
#include <QPoint>
#include <QImage>

class Paint : public QWidget
{
    Q_OBJECT

public:
    Paint(QWidget *parent = 0);
    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);


    bool isModified() const {return modified;}
    QColor getPenColor() const {return penColor;}
    int getPenWidth() const {return penWidth;}
    QString getFigure() const{return type;}

public slots:
    void clearImage();
    void chooseLine();
    void chooseCurve();
    void chooseRect();
    void chooseEllipse();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawLineTo(const QPoint &endPoint);
    void drawStraightLineTo (const QPoint &endPoint);
    void drawRectangleTo(const QPoint &endPoint);
    void drawEllipseTo(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);
    bool modified;
    bool painting;
    QColor penColor;
    QImage image;
    QString type = "Curve";
    //int type = 1;
    QPoint lastPoint;
    int penWidth;

signals:
    void figure();
};

#endif // PAINT_H
