#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QGridLayout>
#include <QDoubleValidator>
#include <QGroupBox>

#include "paramwidget.h"

ParamWidget :: ParamWidget (SeaObjectParameters sp, QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    sop (sp),
    lLength (new QLabel (tr("Length"), this)),
    lELength (new QLineEdit (this)),
    lWidth (new QLabel (tr("Width"), this)),
    lEWidth (new QLineEdit (this)),
    cbDepth (new QCheckBox (tr("Depth"), this)),
    lEDepth (new QLineEdit (this)),
    cbResolv (new QCheckBox (tr("Resolution"), this)),
    lEResolv (new QLineEdit (this)),
    lAzimuth (new QLabel (tr("Azimuth"), this)),
    lEAzimuth (new QLineEdit (this)),
    cbElev (new QCheckBox (tr("Elevation angle"), this)),
    lEElev (new QLineEdit (this)),
    cbSecProp (new QCheckBox (tr("Secondary property"), this)),
    lESec (new QLineEdit (this))
{
    QGridLayout * gLay = new QGridLayout (this);
    QGroupBox * gbImageParams = new QGroupBox (tr("Image parameters"), this);
    gLay->addWidget (gbImageParams, 0, 0, 1, 1);
    QGridLayout * gImLay = new QGridLayout ( gbImageParams );
    gImLay->addWidget (lLength, 0, 0, 1, 1);
    gImLay->addWidget (lELength, 0, 1, 1, 1);
    QDoubleValidator * dLVal = new QDoubleValidator (0, 1000, 16, this);
    lELength->setValidator (dLVal);
    lELength->setText (QString::number (sp.length));

    gImLay->addWidget (lWidth, 1, 0, 1, 1);
    gImLay->addWidget (lEWidth, 1, 1, 1, 1);
    QDoubleValidator * dWVal = new QDoubleValidator (0, 1000, 16, this);
    lEWidth->setValidator (dWVal);
    lEWidth->setText (QString::number (sp.width));

    gImLay->addWidget (cbDepth, 2, 0, 1, 1);
    gImLay->addWidget (lEDepth, 2, 1, 1, 1);
    QDoubleValidator * dDVal = new QDoubleValidator (0, 1000, 16, this);
    lEDepth->setValidator (dDVal);
    if (sp.depth >= 0.0)
    {
        cbDepth->setCheckState (Qt::Checked);
        lEDepth->setText (QString::number (sp.depth));
        lEDepth->setEnabled (true);
    }
    else
    {
        cbDepth->setCheckState (Qt::Unchecked);
        lEDepth->setEnabled (false);
    }
    gImLay->addWidget (cbResolv, 3, 0, 1, 1);
    gImLay->addWidget (lEResolv, 3, 1, 1, 1);
    QDoubleValidator * dRVal = new QDoubleValidator (0.0, 100.0, 5, this);
    lEResolv->setValidator (dRVal);
    cbResolv->setCheckState (Qt::Unchecked);
    lEResolv->setEnabled (false);

    QGroupBox * gbPhysParams = new QGroupBox (tr("Physical parameters"), this);
    gLay->addWidget (gbPhysParams, 1, 0, 1, 1);

    QGridLayout * grPhysLay = new QGridLayout (gbPhysParams);

    grPhysLay->addWidget (lAzimuth, 0, 0, 1, 1);
    grPhysLay->addWidget (lEAzimuth, 0, 1, 1, 1);
    QDoubleValidator * dAzVal = new QDoubleValidator (0, 183, 6, this);
    lEAzimuth->setValidator (dAzVal);
    lEAzimuth->setText (QString::number (sp.azimuth));

    grPhysLay->addWidget (cbElev, 1, 0, 1, 1);
    grPhysLay->addWidget (lEElev, 1, 1, 1, 1);
    QDoubleValidator * dElevVal = new QDoubleValidator (0, 83, 6, this);
    lEElev->setValidator (dElevVal);
    if (sp.elevation_angle >= 0)
    {
        cbElev->setCheckState (Qt::Checked);
        lEElev->setEnabled (true);
        lEElev->setText (QString::number (sp.elevation_angle));
    }
    else
    {
        cbElev->setCheckState (Qt::Unchecked);
        lEElev->setEnabled (false);
    }

    grPhysLay->addWidget (cbSecProp, 2, 0, 1, 1);
    grPhysLay->addWidget (lESec, 2, 1, 1, 1);
    if (!sp.secProp.isEmpty())
    {
        cbSecProp->setCheckState (Qt::Checked);
        lESec->setEnabled (true);
        lESec->setText (sp.secProp);
    }
    else
    {
        cbSecProp->setCheckState (Qt::Unchecked);
        lESec->setEnabled (false);
    }

    connect (cbDepth, SIGNAL (stateChanged(int)), this, SLOT (depthStateChanged(int)) );
    connect (cbResolv, SIGNAL (stateChanged(int)), this, SLOT (resolvStateChanged (int)) );
    connect (cbElev, SIGNAL (stateChanged(int)), this, SLOT (elevStateChanged (int)) );
    connect (cbSecProp, SIGNAL (stateChanged(int)), this, SLOT (secStateChanged (int)) );
}

ParamWidget :: ~ParamWidget (void)
{
}

SeaObjectParameters ParamWidget :: getData (void) const
{
    double l = lELength->text().toDouble ();
    double w = lEWidth->text().toDouble ();
    double d = lEDepth->isEnabled() ? lEDepth->text().toDouble() : -1.0;
    double az = lEAzimuth->text().toDouble ();
    double elev = lEElev->isEnabled() ? lEElev->text().toDouble() : -1.0;
    double r = lEResolv->isEnabled() ? lEResolv->text().toDouble() : -1.0;
    QString sProp = lESec->isEnabled() ? lESec->text() : QString();
    SeaObjectParameters sp (sop.bRect, l, w, d, az, elev, r, sProp);
    return sp;
}

void ParamWidget :: depthStateChanged (int state)
{
    lEDepth->setEnabled ((state == Qt::Checked));
}

void ParamWidget :: elevStateChanged (int state)
{
    lEElev->setEnabled ((state == Qt::Checked));
}

void ParamWidget :: secStateChanged (int state)
{
    lESec->setEnabled ((state == Qt::Checked));
}

void ParamWidget :: resolvStateChanged (int state)
{
    lEResolv->setEnabled ((state == Qt::Checked));
}
