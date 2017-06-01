#ifndef SIMULATIONSCREEN_H
#define SIMULATIONSCREEN_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTextBrowser>
#include <QCoreApplication>

class SimulationScene:public QGraphicsScene
{
public:
    QList<QPointF> route;
    SimulationScene(QPointF realSize, QPoint sceneSize, QTextBrowser * messageScene, QObject * parent = 0);
    void draw();
    void draw(qreal x, qreal y);
    qreal getRealWidth(){ return realSize.x(); }
    qreal getRealHeight(){ return realSize.y(); }
    void setRealWidth(qreal width);
    void setRealHeight(qreal height);
    int getSceneWidth(){ return sceneSize.x(); }
    int getSceneHeight(){ return sceneSize.y(); }
    qreal getRotation(){ return rotation; }
    void setRotation(qreal rotation);
    qreal getCoordinateX();
    qreal getCoordinateY();
    void removeCoordinate();
    void setCoordinate(qreal x, qreal y);
private:
    bool removeStickman = false;
    qreal CAMERAWIDTH = 6.0;
    qreal CAMERAHEIGHT = 7.0;
    QGraphicsPixmapItem * stickmanImage;
    bool show;
    QPoint sceneSize;
    QPointF realSize;
    QPointF ratio;
    qreal rotation;
    QPointF coordinate;
};

#endif // SIMULATIONSCREEN_H
