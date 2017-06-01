#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTextBrowser>
#include <QTimer>
#include <QTcpSocket>
#include <QtNetwork>
#include <QDataStream>
#include <QHostAddress>
#include <QPushButton>
#include <QMediaPlayer>
#include "simulationscene.h"
#include "communicationdatas.h"
#include "drawer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    const int DEFAULTVOLUME = 50;
    const qreal A5WIDTH = 14.8;
    const qreal A5HEIGHT = 21;
    const int MS60FPS = 17;
    QString ipAddress = QString("192.168.43.22");
    int port = 5566;

private:
    QIcon *soundOnIcon;
    QIcon *soundOffIcon;
    int mute = 0;
    bool connecting = false;
    QMediaPlayer *musicPlayer;
    QMediaPlayer *effectPlayer;
    Ui::MainWindow *ui;
    SimulationScene *simulationScene;
    Drawer *pathDrawingScene;
    int elapsedTime = 0;
    QTimer *timer;
    QTcpSocket *socket;
    UItoHW sendData;
    HWtoUI receiveData;
    QPushButton *startButton;
    QPushButton *stopButton;
    QPushButton *pauseButton;
    int mode = STOP;
    void connectToHost();
    bool checkConnection();
    void writeData();
    bool readData();
private slots:
    void changeVolume(int volume);
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void on_pauseButton_clicked();
    void on_connectButton_clicked();
    void on_muteButton_clicked();
    void on_startPathButton_clicked();
    void on_clearPathButton_clicked();
    void updateTimer();
    void communicate();
protected:
     void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
