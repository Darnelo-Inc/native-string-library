#include <QtGui/QApplication>
#include "myform.h"
#include <QTextCodec>


int main(int argc, char *argv[])
{
    QTextCodec* cyrCodec = QTextCodec::codecForName("Windows-1251");
    QTextCodec::setCodecForTr(cyrCodec);
    QTextCodec::setCodecForCStrings(cyrCodec);
    QApplication a(argc, argv);
    MyForm w;
    w.show();
    return a.exec();
}
