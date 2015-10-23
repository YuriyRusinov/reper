#include <QPainter>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QShowEvent>
#include <QContextMenuEvent>
#include <QWidget>
#include <QCoreApplication>
#include <QMenu>
#include <QAction>
#include <QFontDialog>
#include <QColorDialog>
#include <QPalette>
#include <QtDebug>

#include "radapplication.h"
#include "RadSettings.h"
#include "radMdiArea.h"

RadMdiArea :: RadMdiArea (const QImage& im, QString title, QWidget * parent)
    : QMdiArea (parent),
    bkImage (im),
    rTitle (title),
    pMenu (new QMenu)
{
    this->init ();
}

RadMdiArea :: ~RadMdiArea (void)
{
    delete pMenu;
}

void RadMdiArea :: resizeEvent (QResizeEvent * resizeEvent)
{
    QImage newBackground(resizeEvent->size(), QImage::Format_ARGB32_Premultiplied);
    QPainter p(&newBackground);
    p.fillRect(newBackground.rect(), Qt::gray);

    QImage scaled = bkImage.scaled(resizeEvent->size(), Qt::IgnoreAspectRatio);//Qt::KeepAspectRatio);
    QRect scaledRect = scaled.rect();
    scaledRect.moveCenter(newBackground.rect().center());
    p.drawImage(scaledRect, scaled);
    setBackground(newBackground);

    QMdiArea::resizeEvent(resizeEvent);
}

void RadMdiArea :: paintEvent (QPaintEvent * pEvent)
{
//    Q_UNUSED (pEvent);
    QMdiArea::paintEvent (pEvent);
    QWidget * vPort = this->viewport();
    QPainter painter (vPort);
    painter.setPen (tPen);//Qt::yellow);
    painter.setFont(tFont);//QFont("Arial", 60));
    //painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
    //painter.fillRect(0, 0, 500, 500, QColor(0,220,0));
    painter.drawText (vPort->rect(), Qt::AlignCenter, rTitle);
}

void RadMdiArea :: showEvent (QShowEvent * event)
{
    QCoreApplication * app = QCoreApplication::instance ();
    if (qobject_cast<RadApplication *>(app))
    {
        RadiusSettings * rSettings = qobject_cast<RadApplication *>(app)->getRadSettings ();
        if (rSettings)
        {
            rSettings->beginGroup ("System settings");
            rSettings->writeSettings ("graphics", "pen", tPen.color().name());
            rSettings->writeSettings ("graphics", "font family", tFont.family());
            rSettings->writeSettings ("graphics", "font size", QString::number (tFont.pointSize()));
            rSettings->endGroup();
        }
    }
    QMdiArea::showEvent (event);
}

void RadMdiArea :: init (void)
{
    QCoreApplication * app = QCoreApplication::instance ();
    if (qobject_cast<RadApplication *>(app))
    {
        RadiusSettings * rSettings = qobject_cast<RadApplication *>(app)->getRadSettings ();
        if (rSettings)
        {
            rSettings->beginGroup ("System settings/graphics");
            QString colName = rSettings->getParam ("pen");
            if (colName.isEmpty())
                tPen = QPen (this->palette().color(QPalette::Active, QPalette::Window));//(Qt::yellow);
            else
                tPen = QPen (QColor (colName));
            QString fontFamily = rSettings->getParam ("font family");
            int pointSize = rSettings->getParam ("font size").toInt();
            if (fontFamily.isEmpty() || pointSize <= 0)
                tFont = QFont("Arial", 60);
            else
                tFont = QFont (fontFamily, pointSize);

            rSettings->endGroup();
        }
    }

    QAction * actFont = new QAction (tr("Set title font"), this);
    pMenu->addAction (actFont);
    connect (actFont, SIGNAL (triggered()), this, SLOT (setTitleFont()) );

    QAction * actCol = new QAction (tr("Set title color"), this);
    pMenu->addAction (actCol);
    connect (actCol, SIGNAL (triggered()), this, SLOT (setTitleColor()) );
}

void RadMdiArea :: setTitleFont (void)
{
//    QFontDialog * fDialog = new QFontDialog (tFont, this);
    bool ok;
    QFont font = QFontDialog::getFont (&ok, tFont, this);
    if (!ok)
        return;

    tFont = font;
    QCoreApplication * app = QCoreApplication::instance ();
    if (qobject_cast<RadApplication *>(app))
    {
        RadiusSettings * rSettings = qobject_cast<RadApplication *>(app)->getRadSettings ();
        if (rSettings)
        {
            rSettings->beginGroup ("System settings");
            rSettings->writeSettings ("graphics", "font family", tFont.family());
            rSettings->writeSettings ("graphics", "font size", QString::number (tFont.pointSize()));
            rSettings->endGroup();
        }
    }
}

void RadMdiArea :: setTitleColor (void)
{
    QColor col = QColorDialog::getColor (tPen.color(), this);
    if (!col.isValid())
        return;

    tPen.setColor (col);
    QCoreApplication * app = QCoreApplication::instance ();
    if (qobject_cast<RadApplication *>(app))
    {
        RadiusSettings * rSettings = qobject_cast<RadApplication *>(app)->getRadSettings ();
        if (rSettings)
        {
            rSettings->beginGroup ("System settings");
            rSettings->writeSettings ("graphics", "pen", tPen.color().name());
            rSettings->endGroup();
        }
    }
}

void RadMdiArea :: contextMenuEvent (QContextMenuEvent * event)
{
    pMenu->exec (event->globalPos());
    event->accept();
}
