#include <QCoreApplication>
#include <QStringList>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList args = QCoreApplication::arguments();

    qWarning() << args;

    return 0; //a.exec();
}
