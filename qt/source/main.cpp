#include <QDebug>
#include <QThread>
#include <QString>



void hello(QString name)
{
    qDebug() << "Hello" << name << "from" << QThread::currentThread();
}

int main(int argc, char **argv)
{
   printf("Compiled with Qt Version %s\n", QT_VERSION_STR);
    hello("µMIC.200");
}

