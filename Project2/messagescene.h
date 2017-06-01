#ifndef MESSAGESCENE_H
#define MESSAGESCENE_H
#include<QTextBrowser>

class MessageScene:public QTextBrowser
{
public:
    MessageScene(QObject * parent = 0);
    void add();
};

#endif // MESSAGESCENE_H
