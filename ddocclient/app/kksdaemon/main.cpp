/* qt */
#include <QApplication>
#include <Qt>
#include <QObject>

#include "kksdaemon.h"

int main(int argc, char *argv[])
{
#if !defined(Q_WS_WIN)
    // QtService stores service settings in SystemScope, which normally require root privileges.
    // To allow testing this example as non-root, we change the directory of the SystemScope settings file.
    QSettings::setPath(QSettings::NativeFormat, QSettings::SystemScope, QDir::tempPath());
    qWarning("(Example uses dummy settings file: %s/QtSoftware.conf)", QDir::tempPath().toLatin1().constData());
#endif
    KKSDaemon service(argc, argv);
    return service.exec();
}


