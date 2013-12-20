#include <QApplication>

#include "syncqueueviewerform.h"
#include "kkssito.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    KKSSito *sito = KKSSito::init (true);
    if(!sito)
        return 1;

    if(!sito->db())
        return 1;

    SyncQueueViewerForm * f = new SyncQueueViewerForm(sito->db());
    f->exec();
    delete f;

    return 0;
}
