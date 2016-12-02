#ifndef _ParamWidget_H
#define _ParamWidget_H

#include <QWidget>

#include "seaobjectparameters.h"

class QLabel;
class QLineEdit;
class QCheckBox;

class ParamWidget : public QWidget
{
public:
    ParamWidget (SeaObjectParameters sp, QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~ParamWidget (void);

    SeaObjectParameters getData (void) const;

private slots:
    void depthStateChanged (int state);
    void elevStateChanged (int state);
    void secStateChanged (int state);
    void resolvStateChanged (int state);

private:
    //
    // Variables
    //
    SeaObjectParameters sop;
    QLabel * lLength;
    QLineEdit * lELength;

    QLabel * lWidth;
    QLineEdit * lEWidth;

    QCheckBox * cbDepth;
    QLineEdit * lEDepth;

    QCheckBox * cbResolv;
    QLineEdit * lEResolv;

    QLabel * lAzimuth;
    QLineEdit * lEAzimuth;

    QCheckBox * cbElev;
    QLineEdit * lEElev;

    QCheckBox * cbSecProp;
    QLineEdit * lESec;
private:
    Q_OBJECT
};

#endif
