#include "paint.h"
#include "QMouseEvent"
#include "QPainter"

Paint::Paint(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    modified = false;
    painting = false;
    penWidth = 7;
    penColor = Qt::black;
}

bool Paint::openImage(const QString &fileName){
    QImage loadedImage;
    if(!loadedImage.load(fileName)){
        return false;
    }
    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    image = loadedImage;
    modified = false;
    update();
    return true;
}

bool Paint::saveImage(const QString &fileName, const char *fileFormat){
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());
    if (visibleImage.save(fileName, fileFormat)){
        modified = false;
        return true;
    } else {
        return false;
    }
}

void Paint::setPenColor(const QColor &newColor){
    penColor = newColor;
}

void Paint::setPenWidth(int newWidth){
    penWidth = newWidth;
}

void Paint::clearImage(){
    image.fill(qRgb(255,255,255));
    modified = true;
    update();
}

void Paint::chooseLine()
{
    type = "Line";
    emit figure();
    //type = 2;
}

void Paint::chooseCurve()
{
    type = "Curve";
    emit figure();
    //type = 1;
}

void Paint::chooseRect()
{
    type = "Rect";
    emit figure();
    //type = 3;
}

void Paint::chooseEllipse()
{
    type = "Ellipse";
    emit figure();
    //type = 4;
}

void Paint::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        lastPoint = event->pos();
        painting = true;
    }
}

void Paint::mouseMoveEvent(QMouseEvent *event){
    if((event->buttons() & Qt::LeftButton) && painting){
        if (type == "Curve")
            drawLineTo(event->pos());
    }
}

void Paint::mouseReleaseEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton && painting){
        if(type == "Line" || type == "Curve")
            drawLineTo((event)->pos());
        if(type == "Rect")
            drawRectangleTo(event->pos());
        if(type == "Ellipse")
            drawEllipseTo(event->pos());
        painting = false;
    }
}

void Paint::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QRect updatableRect = event->rect();
    painter.drawImage(updatableRect, image, updatableRect);
}

void Paint::resizeEvent(QResizeEvent *event){
    if (width() > image.width() || height() > image.height()){
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void Paint::resizeImage(QImage *image, const QSize &newSize){
    if(image->size() == newSize){
        return;
    }
    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255,255,255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0,0), *image);
    *image = newImage;
}

void Paint::drawLineTo(const QPoint &endPoint){
    QPainter painter(&image);
    painter.setPen(QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    modified = true;
    int rad = (penWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}

void Paint::drawRectangleTo(const QPoint &endPoint)
{
    QPainter painter(&image);
    painter.setPen(QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    QRectF rect(lastPoint, endPoint);
    painter.drawRect(rect);
    modified = true;
    int rad = (penWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}

void Paint::drawEllipseTo(const QPoint &endPoint)
{
    QPainter painter(&image);
    painter.setPen(QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    QRectF rect(lastPoint, endPoint);
    painter.drawEllipse(rect);
    modified = true;
    int rad = (penWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}


