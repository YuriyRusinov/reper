/* 
 * File:   KKSBinaryWidget.h
 * Author: yuriyrusinov
 *
 * Created on 26 Ноябрь 2014 г., 18:47
 */

#ifndef KKSBINARYWIDGET_H
#define	KKSBINARYWIDGET_H

#include <QLabel>
#include <QVariant>
#include "KKSAttrWidget.h"

#include "kksgui_config.h"

class KKSAttrValue;

class _GUI_EXPORT KKSBinaryWidget : public QLabel, public KKSAttrWidget
{
public:
    KKSBinaryWidget(const KKSAttrValue*attr, KKSIndAttrClass isSys, QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~KKSBinaryWidget();

public slots:
    void openFile (void);
    void setData (const QByteArray& ba);

signals:
    void valueChanged (qint64 id, KKSIndAttrClass isSys, QVariant val);

private:
    //
    // Variables
    //
    QByteArray binaryData;
   
private:
    Q_OBJECT
};

#endif	/* KKSBINARYWIDGET_H */

