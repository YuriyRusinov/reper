/* 
 * File:   KKSBinaryWidget.cpp
 * Author: yuriyrusinov
 * 
 * Created on 26 Ноябрь 2014 г., 18:47
 */
#include <QFileDialog>
#include <QDir>
#include <QDataStream>
#include <QtDebug>

#include <KKSValue.h>
#include "KKSBinaryWidget.h"
#include "KKSAttrValue.h"

KKSBinaryWidget::KKSBinaryWidget(const KKSAttrValue*attr, KKSIndAttrClass isSys, QWidget * parent, Qt::WindowFlags flags)
    : QLabel (parent, flags),
    KKSAttrWidget(attr, isSys),
    binaryData (QByteArray())
{
    KKSValue val = attr->value();
    QVariant V = val.valueVariant();
    binaryData = V.toByteArray();
}

KKSBinaryWidget::~KKSBinaryWidget()
{
}

void KKSBinaryWidget :: openFile (void)
{
    //qDebug () << __PRETTY_FUNCTION__;
    QString filter (tr("All files (*.*)"));
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Data File"),
                                                    QDir::currentPath(),
                                                    filter);
    if (fileName.isEmpty())
        return;

    QFile fName (fileName);
    fName.open (QIODevice::ReadOnly);
    qint64 nBytes = fName.bytesAvailable();
    QDataStream fStr (&fName);
    char * w = new char [nBytes];
    fStr.readRawData(w, nBytes);
    binaryData.clear();
    binaryData.append (w);
    delete [] w;
    //binaryData = fName.readAll();
    setText (binaryData.constData());
    qDebug () << __PRETTY_FUNCTION__ << binaryData.constData() << binaryData.length() << nBytes;
    
    QVariant v = QVariant(binaryData);
    emit valueChanged (m_av->id(), m_isSystem, v);
}

void KKSBinaryWidget :: setData (const QByteArray& ba)
{
    binaryData = ba;
    setText (binaryData.constData());
}