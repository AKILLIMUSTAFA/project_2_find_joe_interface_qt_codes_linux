#include "simulationscene.h"

SimulationScene::SimulationScene(QPointF realSize, QPoint sceneSize, QTextBrowser * messageScene, QObject * parent)
    :QGraphicsScene(parent), sceneSize(sceneSize), show(false), rotation(0)
{
    const QString imagePath = QCoreApplication::applicationDirPath() + "/stickman.PNG";

    setRealWidth(realSize.x());
    setRealHeight(realSize.y());
    stickmanImage = NULL;
    QPixmap * pixmap = new QPixmap(imagePath);
    if(pixmap->isNull())
        messageScene->append(QString("Image cannot be loaded from \"") + imagePath + QString("\"."));
    else{
        stickmanImage = new QGraphicsPixmapItem(*pixmap);
    }
}

void SimulationScene::draw()
{
    if(removeStickman){
        this->removeItem(stickmanImage);
        removeStickman = false;
    }

    this->clear();
    this->addRect(0, 0, getSceneWidth(), getSceneHeight());

    if(show){
        if(stickmanImage == NULL){
            const qreal RADIUS = 10;
            this->addEllipse(coordinate.x()*ratio.x()-RADIUS, coordinate.y()*ratio.y()-RADIUS, 2*RADIUS, 2*RADIUS);
        }
        else{
            this->addItem(stickmanImage);
            stickmanImage->setTransformOriginPoint(stickmanImage->boundingRect().width()/2.0, stickmanImage->boundingRect().height()/2.0);
            stickmanImage->setPos(coordinate.x()*ratio.x()-stickmanImage->boundingRect().width()/2.0, coordinate.y()*ratio.y()-stickmanImage->boundingRect().height()/2.0);
            stickmanImage->setRotation(getRotation());
            removeStickman = true;
        }
    }
    this->addText(QString("X: ") + QString::number(getCoordinateX()) + QString(" Y: ") + QString::number(getCoordinateY()));
}

void SimulationScene::draw(qreal x, qreal y)
{
    route.append(QPointF(x,y));

    if(removeStickman){
        this->removeItem(stickmanImage);
        removeStickman = false;
    }

    this->clear();
    this->addRect(0, 0, getSceneWidth(), getSceneHeight());

    if(route.size() > 1){
        QPen dashPen(Qt::black);
        dashPen.setStyle(Qt::DashLine);
        dashPen.setJoinStyle(Qt::RoundJoin);

        for(int i=0;i<route.size()-1;++i)
            addLine(route.at(i).x()*ratio.x(), route.at(i).y()*ratio.y(), route.at(i+1).x()*ratio.x(), route.at(i+1).y()*ratio.y(), dashPen);
    }

    QPen rectPen(Qt::red);
    rectPen.setWidth(3);
    rectPen.setJoinStyle(Qt::RoundJoin);

    this->addRect((x - CAMERAWIDTH/2.0)*ratio.x(), (y - CAMERAHEIGHT/2.0)*ratio.y(), CAMERAWIDTH*ratio.x(), CAMERAHEIGHT*ratio.y(), rectPen);

    if(show){
        if(stickmanImage == NULL){
            const qreal RADIUS = 10;
            this->addEllipse(coordinate.x()*ratio.x()-RADIUS, coordinate.y()*ratio.y()-RADIUS, 2*RADIUS, 2*RADIUS);
        }
        else{
            this->addItem(stickmanImage);
            stickmanImage->setTransformOriginPoint(stickmanImage->boundingRect().width()/2.0, stickmanImage->boundingRect().height()/2.0);
            stickmanImage->setPos(coordinate.x()*ratio.x()-stickmanImage->boundingRect().width()/2.0, coordinate.y()*ratio.y()-stickmanImage->boundingRect().height()/2.0);
            stickmanImage->setRotation(getRotation());
            removeStickman = true;
        }
    }
    this->addText(QString("X: ") + QString::number(x) + QString("  Y: ") + QString::number(y));
}

void SimulationScene::setRealWidth(qreal width){
    realSize.setX(width);
    ratio.setX(getSceneWidth()/width);
}

void SimulationScene::setRealHeight(qreal height){
    realSize.setY(height);
    ratio.setY(getSceneHeight()/height);
}

void SimulationScene::setRotation(qreal rotation){
    this->rotation = rotation;
}

qreal SimulationScene::getCoordinateX(){
    return coordinate.x();
}

qreal SimulationScene::getCoordinateY(){
    return coordinate.y();
}

void SimulationScene::removeCoordinate(){
    show = false;
    route.clear();
}

void SimulationScene::setCoordinate(qreal x, qreal y){
    coordinate.setX(x);
    coordinate.setY(y);
    show = true;
}
