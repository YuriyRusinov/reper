#ifndef _ParamWidget_H
#define _ParamWidget_H

#include <QWidget>
#include <QValidator>

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
    QValidator::State isValid (void) const;

private slots:
    void widthStateChanged (int state);
    void elevStateChanged (int state);
    void secStateChanged (int state);
    void resolvStateChanged (int state);
    void lEEditFinished (void);
    void lETextEdited (const QString& text);

private:
    //
    // Variables
    //
    SeaObjectParameters sop;
    QLabel * lLength;
    QLineEdit * lELength;

    QCheckBox * cbWidth;
    QLineEdit * lEWidth;
    double wlRel;

//    QCheckBox * cbDepth;
//    QLineEdit * lEDepth;

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
