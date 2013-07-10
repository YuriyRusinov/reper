#include <QApplication>

#include "dyndoc_synch_form.h"
#include "kkssito.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    KKSSito *sito = KKSSito::init (true);
    if(!sito)
        return 1;

    if(!sito->db())
        return 1;

    dyndoc_synch_form * f = new dyndoc_synch_form(sito->db());
    f->exec();
    delete f;

    return 0;
}
