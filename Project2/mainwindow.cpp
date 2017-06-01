#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    timer = new QTimer();
    musicPlayer = new QMediaPlayer;
    effectPlayer = new QMediaPlayer;
    musicPlayer->setMedia(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/Mission Impossible.mp3"));
    musicPlayer->setVolume(DEFAULTVOLUME);
    effectPlayer->setVolume(DEFAULTVOLUME);
    simulationScene = new SimulationScene(QPointF(A5WIDTH, A5HEIGHT), QPoint(ui->simulationScene->width(), ui->simulationScene->height()), ui->messageScene, this);
    pathDrawingScene = new Drawer(QPointF(A5WIDTH, A5HEIGHT), QPoint(ui->pathDrawingScene->width(), ui->pathDrawingScene->height()), this);

    soundOnIcon = new QIcon(QCoreApplication::applicationDirPath() + "/soundOn.png");
    soundOffIcon = new QIcon(QCoreApplication::applicationDirPath() + "/soundOff.png");
    ui->muteButton->setIcon(*soundOnIcon);
    ui->muteButton->setIconSize(QSize(ui->muteButton->width(), ui->muteButton->height()));

    ui->volumeSlider->setRange(0, 100);
    ui->volumeSlider->setFixedWidth(100);
    ui->volumeSlider->setValue(DEFAULTVOLUME);

    connect(ui->volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(changeVolume(int)));

    ui->simulationScene->setScene(simulationScene);
    ui->simulationScene->setRenderHint(QPainter::Antialiasing);
    ui->simulationScene->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->simulationScene->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->pathDrawingScene->setScene(pathDrawingScene);
    ui->pathDrawingScene->setRenderHint(QPainter::Antialiasing);
    ui->pathDrawingScene->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->pathDrawingScene->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->messageScene->setTextColor(QColor(Qt::white));
    ui->ipLineEdit->setInputMask("000.000.000.000;_");
    ui->ipLineEdit->setText(ipAddress);
    ui->portLineEdit->setText(QString::number(port));
    //connectToHost();
    timer->start(MS60FPS);
    connect(timer, SIGNAL(timeout()), this, SLOT(communicate()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete simulationScene;
    delete timer;
    delete socket;
}

void MainWindow::changeVolume(int volume){
    if(musicPlayer->volume() == 0)
        mute = 0;

    musicPlayer->setVolume(volume);
    effectPlayer->setVolume(volume);

    if(volume == 0){
        ui->muteButton->setIcon(*soundOffIcon);
        ui->muteButton->setIconSize(QSize(ui->muteButton->width(), ui->muteButton->height()));
    }
    else{
        ui->muteButton->setIcon(*soundOnIcon);
        ui->muteButton->setIconSize(QSize(ui->muteButton->width(), ui->muteButton->height()));
    }
}

void MainWindow::on_startButton_clicked(){
    if(mode == PLAY)
        return;
    elapsedTime = 0;

    if(checkConnection()){
        mode = PLAY;
        sendData.mode = PLAY;
        musicPlayer->play();
    }
}

void MainWindow::on_pauseButton_clicked(){
    if(checkConnection()){
        sendData.mode = PAUSE;
        mode = PAUSE;
        musicPlayer->pause();
    }
}

void MainWindow::on_stopButton_clicked(){
    elapsedTime = 0;
    simulationScene->removeCoordinate();
    if(checkConnection()){
        sendData.mode = STOP;
        mode = STOP;
        musicPlayer->stop();
    }
}

void MainWindow::on_connectButton_clicked(){
    if(connecting)
        return;

    if(checkConnection()){
        ui->messageScene->setTextColor(QColor(Qt::white));
        ui->messageScene->append(QString("Allready connected to host."));
    }
    else{
        socket->close();
        connectToHost();
    }
}

void MainWindow::on_muteButton_clicked(){
    int temp = mute;
    mute = musicPlayer->volume();
    changeVolume(temp);
}

void MainWindow::on_startPathButton_clicked(){
    if(pathDrawingScene->getFinishPath()){
        if(checkConnection()){
            timer->stop();
            ui->messageScene->setTextColor(QColor(Qt::white));
            ui->messageScene->append(QString("Sending path."));
            sendData.mode = PATH;
            writeData();
            socket->waitForBytesWritten();
            QList<QPointF> path = pathDrawingScene->getPathList();
            sendData.mode = path.size()*2;
            writeData();
            for(int i=0;i<path.size();++i){
                sendData.mode = path.at(i).x() + 0.5;
                writeData();
                sendData.mode = path.at(i).y() + 0.5;
                writeData();
            }
            sendData.mode = EMPTY;
            timer->start();
        }
        else{
            ui->messageScene->setTextColor(QColor(Qt::red));
            ui->messageScene->append(QString("Not connected to host."));
        }
    }
    else{
        ui->messageScene->setTextColor(QColor(Qt::red));
        ui->messageScene->append(QString("Path does not have a shape can loop."));
    }
}

void MainWindow::on_clearPathButton_clicked(){
    pathDrawingScene->clearPath();
}

void MainWindow::updateTimer(){
    elapsedTime += MS60FPS;
    ui->timeScene->display(elapsedTime);
    ui->timeScene->show();
}

void MainWindow::connectToHost()
{
    connecting = true;
    ipAddress = ui->ipLineEdit->text();
    port = ui->portLineEdit->text().toInt();
    ui->messageScene->setTextColor(QColor(Qt::white));
    ui->messageScene->append(QString("Trying to connect host..."));
    socket->connectToHost(QHostAddress(/*ui->ipLineEdit->text()*/ipAddress), /**/port);
    if(socket->waitForConnected()){
        ui->messageScene->setTextColor(QColor(Qt::green));
        ui->messageScene->append(QString("Connected to host."));
    }
    else{
        ui->messageScene->setTextColor(QColor(Qt::red));
        ui->messageScene->append(QString("Could not connect to host."));
    }
    connecting = false;
}

void MainWindow::writeData()
{
    socket->write((char*)&sendData, sizeof(sendData));
    socket->waitForBytesWritten();
    sendData.mode = EMPTY;
}

bool MainWindow::checkConnection(){
    return socket->state() == QAbstractSocket::ConnectedState;
}

bool MainWindow::readData()
{
    if(socket->bytesAvailable() >= sizeof(struct HWtoUI))
    {
        socket->read((char*)&receiveData, sizeof(struct HWtoUI));
        return true;

    }

    return false;
}

void MainWindow::communicate(){
    pathDrawingScene->draw();

    if(checkConnection()){
        if(readData()){
            if(mode == PLAY){
                updateTimer();

                if(receiveData.found){
                    mode = PAUSE;
                    musicPlayer->stop();
                    simulationScene->setCoordinate(receiveData.x, receiveData.y);
                    //converting rotation
                    receiveData.rotation += 90;
                    simulationScene->setRotation(receiveData.rotation);
                    ui->messageScene->setTextColor(QColor(Qt::green));
                    ui->messageScene->append(QString("Model has been found. X:")
                                             + QString::number(simulationScene->getCoordinateX())
                                             + QString(" Y:") + QString::number(simulationScene->getCoordinateY())
                                             + QString(" ")
                                             + QString::number(simulationScene->getRotation()) + QString("' ")
                                             + QString::number(elapsedTime) + QString("ms"));
                    simulationScene->draw();
                    effectPlayer->setMedia(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/laugh.mp3"));
                    effectPlayer->play();
                    simulationScene->removeCoordinate();
                }
                else{
                    simulationScene->draw(receiveData.x, receiveData.y);
                }

                receiveData.found = false;
            }
            writeData();
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event){
    socket->close();
}
