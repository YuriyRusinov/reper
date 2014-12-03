/* 
 * File:   KKSBinaryWidget.cpp
 * Author: yuriyrusinov
 * 
 * Created on 26 Ноябрь 2014 г., 18:47
 */

#include "KKSBinaryWidget.h"

KKSBinaryWidget::KKSBinaryWidget(const KKSAttrValue*attr, KKSIndAttrClass isSys, QWidget * parent, Qt::WindowFlags flags)
    : QLabel (parent, flags),
    KKSAttrWidget(attr, isSys),
    binaryData (QByteArray())
{
}

KKSBinaryWidget::~KKSBinaryWidget()
{
}

