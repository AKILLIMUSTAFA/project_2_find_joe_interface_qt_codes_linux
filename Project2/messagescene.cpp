#include "messagescene.h"

MessageScene::MessageScene(QObject * parent = 0):QTextBrowser(parent)
{
}

void MessageScene::add(Q){
    this->append();
}
