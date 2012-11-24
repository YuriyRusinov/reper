#ifndef _KKSSYNC_DIALOG_H
#define _KKSSYNC_DIALOG_H

#include <QDialog>

#include "kksgui_config.h"

class QPushButton;

class KKSSyncWidget;

class _GUI_EXPORT KKSSyncDialog : public QDialog
{
public:
    KKSSyncDialog (QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~KKSSyncDialog (void);

    KKSSyncWidget * getSyncWidget (void) const;

private:
    //
    // Variables
    //
    KKSSyncWidget * syncW;
    QPushButton * pbOk;
    QPushButton * pbCancel;

private:
    Q_OBJECT
};

#endif
