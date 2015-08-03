#include <QApplication>

#include "syncqueueviewerform.h"
#include "kkscoreapplication.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    KKSCoreApplication *coreApp = KKSCoreApplication::init (true);
    if(!coreApp)
        return 1;

    if(!coreApp->db())
        return 1;

    SyncQueueViewerForm * f = new SyncQueueViewerForm(coreApp->db());
    f->exec();
    delete f;

    return 0;
}
