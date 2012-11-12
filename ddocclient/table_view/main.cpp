#include "table_view.h"
#include <QtGui>
#include <kkssito.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	KKSSito *sito = KKSSito::init(false);

	if(!sito)
		return 1;

	table_view w;

	w.show();
	return a.exec();
}
