/*
 * File:   attr_history.h
 * Author: Alexander Mitin
 *
 * Created on 02 Март 2012 г., 15:04
 */

#ifndef _AttrHistory_H
#define _AttrHistory_H

#include "kksgui_config.h"
#include <QDialog>
#include <QDockWidget>
#include "KKSAttrValuePropsForm.h"

class QLineEdit;

namespace Ui
{
    class attr_history;
};

class AttrHistory : public QDialog
{
public:
    AttrHistory (const KKSList<KKSAttrValue*> & histlist, QWidget *parent=0, Qt::WFlags f=0);
    virtual ~AttrHistory();

private:
    void view(const KKSList<KKSAttrValue*> &);

    //
private:
    Ui::attr_history *UI;
    Q_OBJECT
};

#endif //_AttrHistory_H
