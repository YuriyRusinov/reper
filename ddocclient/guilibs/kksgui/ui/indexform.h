// -*- C++ -*-
/* 
 * File:   indexform.h
 * Author: rusinov
 *
 * Created on 17 Февраль 2012 г., 15:06
 */

#ifndef _INDEXFORM_H
#define	_INDEXFORM_H

#include <QDialog>
#include <KKSMap.h>
#include "kksgui_config.h"

namespace Ui
{
    class index_form;
};

class KKSIndicator;

class _GUI_EXPORT IndexForm : public QDialog
{
public:
    IndexForm (KKSIndicator * ind, const KKSMap<int, KKSIndicator*>& existIndices, QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~IndexForm (void);

    KKSIndicator * getIndex (void) const;
    
private slots:
    void parentChanged (int index);
    void manualStateChanged (int state);
    void apply (void);

private:
    //
    // Functions
    //
    void initForm (void);
private:
    //
    // Variables
    //
    Ui::index_form * UI;
    KKSIndicator * Index;
    KKSMap<int, KKSIndicator*> eIndices;

private:
    Q_OBJECT
};

#endif	/* _INDEXFORM_H */

