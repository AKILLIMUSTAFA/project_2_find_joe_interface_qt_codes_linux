#ifndef DRAWER_H
#define DRAWER_H

#include <QGraphicsScene>
#include <QList>
#include <QtGui>
#include <QtCore>

class Drawer:public QGraphicsScene
{
public:
    Drawer(QPointF realSize, QPoint sceneSize, QObject * parent = 0);
    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent * mouseEvent );
    void draw();
    void draw(qreal x, qreal y);
    void clearPath();
    bool getFinishPath();
    int getMouseX();
    int getMouseY();
    void setMouseX(int x);
    void setMouseY(int y);
    QList<QPointF> getPathList();

    int getSceneWidth(){ return sceneSize.x(); }
    int getSceneHeight(){ return sceneSize.y(); }

    void setRealWidth(qreal width);
    void setRealHeight(qreal height);

    const qreal POINT_RADIUS = 10;
    const qreal MAX_RADIUS = 20; //MAX_RADIUS >= 2*POINT_RADIUS
private:
    qreal collisionX(qreal x);
    qreal collisionY(qreal y);
    QPointF collision(QPointF &point);
    QPointF collision(qreal x, qreal y);

    qreal coordinateToPositionX(int x);
    qreal coordinateToPositionY(int y);

    qreal positionToCoordinateX(qreal x);
    qreal positionToCoordinateY(qreal y);
    QPointF positionToCoordinate(const QPointF &point);

    int mouseX = 0;
    int mouseY = 0;

    bool pathReady = false;
    bool closeToStartPoint = false;
    bool finishPath = false;

    QList<QPointF> path;
    QPoint sceneSize;
    QPointF realSize;
    QPointF ratio;
};

#endif // DRAWER_H
