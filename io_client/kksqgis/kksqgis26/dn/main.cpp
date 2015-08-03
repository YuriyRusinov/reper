//#pragma comment(lib,"c:/OSGeo4W/lib/gdal_i.lib")
#include <QtGui/QApplication>
#include <QTextCodec>
#include "dnspecbath.h"

#include <QString>
#include <QFileDialog>
#include <QFileInfo>


int main(int argc, char *argv[])
{
  QTextCodec::setCodecForTr(QTextCodec::codecForName("system"));
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("system"));
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("system"));

  QApplication a(argc, argv);
  DNSpecBath w;
  w.show();
  return a.exec();
}
