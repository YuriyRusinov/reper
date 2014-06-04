
#include "KKSGISWidget.h"

#include <QtDebug>
#include <QDomDocument>
#include <QDir>
#include <QMessageBox>
#include <QFile>
#include <QLayout>
#include <QMenuBar>
#include <QStatusBar>
#include <QUrl>


KKSGISWidget::KKSGISWidget(bool withSubWindows, bool withAddons, QWidget* parent, Qt::WFlags fl)
    : KKSGISWidgetBase(withSubWindows, withAddons, parent, fl)
{
}

KKSGISWidget::~KKSGISWidget()
{
}
