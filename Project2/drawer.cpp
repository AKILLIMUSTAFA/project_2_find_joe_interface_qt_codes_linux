#include "drawer.h"
#include <QGraphicsSceneMouseEvent>

Drawer::Drawer(QPointF realSize, QPoint sceneSize, QObject * parent)
    :QGraphicsScene(parent), sceneSize(sceneSize)
{
    setRealWidth(realSize.x());
    setRealHeight(realSize.y());
}

void Drawer::setRealWidth(qreal width){
    realSize.setX(width);
    ratio.setX(getSceneWidth()/width);
}

void Drawer::setRealHeight(qreal height){
    realSize.setY(height);
    ratio.setY(getSceneHeight()/height);
}

void Drawer::mouseReleaseEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
    if(mouseEvent->button() == Qt::LeftButton && !finishPath){
        if(closeToStartPoint){
            closeToStartPoint = false;
            finishPath = true;
        }
        else{
            QPointF point = mouseEvent->scenePos();
            point = collision(point.x(), point.y());
            path.append(point);
        }
    }
    else if(mouseEvent->button() == Qt::RightButton){
        clear();
    }

    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void Drawer::clearPath(){
    path.clear();
    closeToStartPoint = false;
    finishPath = false;
    draw();
}

void Drawer::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(!finishPath && path.size() > 1){
        qreal dx = mouseEvent->scenePos().x() - path.at(0).x();
        qreal dy = mouseEvent->scenePos().y() - path.at(0).y();
        if(qSqrt(dx*dx+dy*dy) < MAX_RADIUS)
            closeToStartPoint = true;
        else
            closeToStartPoint = false;
    }
    setMouseX(mouseEvent->scenePos().x());
    setMouseY(mouseEvent->scenePos().y());

    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void Drawer::draw(){
    this->clear();
    this->addRect(0, 0, sceneSize.x(), sceneSize.y());

    if(!path.empty()){
        QPen linePen(Qt::black);
        linePen.setWidth(POINT_RADIUS);
        linePen.setJoinStyle(Qt::RoundJoin);
        QPen pointPen(Qt::red);
        QBrush pointBrush(Qt::red);
        QPen startPointPen(Qt::green);
        QBrush startPointBrush(Qt::green);

        for(int i=1;i<path.size();++i)
            this->addLine(QLineF(path.at(i-1), path.at(i)), linePen);

        if(finishPath)
            this->addLine(QLineF(path.at(path.size()-1), path.at(0)), linePen);

        for(int i=1;i<path.size();++i)
            this->addEllipse(path.at(i).x()-POINT_RADIUS, path.at(i).y()-POINT_RADIUS, 2*POINT_RADIUS, 2*POINT_RADIUS, pointPen, pointBrush);

        if(closeToStartPoint)
            this->addEllipse(path.at(0).x()-MAX_RADIUS, path.at(0).y()-MAX_RADIUS, 2*MAX_RADIUS, 2*MAX_RADIUS, startPointPen, startPointBrush);
        else
            this->addEllipse(path.at(0).x()-POINT_RADIUS, path.at(0).y()-POINT_RADIUS, 2*POINT_RADIUS, 2*POINT_RADIUS, startPointPen, startPointBrush);
    }

    QString coordinates = QString("X: ") + QString::number(positionToCoordinateX(getMouseX())) + QString("  Y: ") + QString::number(positionToCoordinateY(getMouseY()));
    this->addText(coordinates);
}

void Drawer::draw(qreal x, qreal y){
    draw();
    QPen cameraPen(Qt::blue);
    QBrush cameraBrush(Qt::blue);

    this->addEllipse(coordinateToPositionX(x)-POINT_RADIUS, coordinateToPositionY(y)-POINT_RADIUS, 2*POINT_RADIUS, 2*POINT_RADIUS, cameraPen, cameraBrush);
}

bool Drawer::getFinishPath()
{
    return finishPath;
}

int Drawer::getMouseX()
{
    return mouseX;
}

int Drawer::getMouseY()
{
    return mouseY;
}

void Drawer::setMouseX(int x)
{
    mouseX = x;
}

void Drawer::setMouseY(int y)
{
    mouseY = y;
}

QList<QPointF> Drawer::getPathList()
{
    QList<QPointF> list;
    for(int i=0;i<path.size();++i)
        list.append(positionToCoordinate(path.at(i)));

    return list;
}

qreal Drawer::collisionX(qreal x)
{
    if(x-MAX_RADIUS < 0)
        return MAX_RADIUS;

    if(x+MAX_RADIUS > getSceneWidth())
        return getSceneWidth() - MAX_RADIUS;

    return x;
}

qreal Drawer::collisionY(qreal y)
{
    if(y-MAX_RADIUS < 0)
        return MAX_RADIUS;

    if(y+MAX_RADIUS > getSceneHeight())
        return getSceneHeight() - MAX_RADIUS;

    return y;
}

QPointF Drawer::collision(QPointF &point)
{
    return collision(point.x(), point.y());
}

QPointF Drawer::collision(qreal x, qreal y)
{
    return QPointF(collisionX(x), collisionY(y));
}

qreal Drawer::positionToCoordinateX(qreal x)
{
    return x/ratio.x();
}

qreal Drawer::positionToCoordinateY(qreal y)
{
    return y/ratio.y();
}

qreal Drawer::coordinateToPositionX(int x)
{
    return x*ratio.x();
}

qreal Drawer::coordinateToPositionY(int y)
{
    return y*ratio.y();
}

QPointF Drawer::positionToCoordinate(const QPointF &point){
    return QPointF(positionToCoordinateX(point.x()), positionToCoordinateY(point.y()));
}
