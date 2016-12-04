#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QGridLayout>
#include <QDoubleValidator>
#include <QMessageBox>
#include <QGroupBox>

#include "paramwidget.h"

ParamWidget :: ParamWidget (SeaObjectParameters sp, QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    sop (sp),
    lLength (new QLabel (tr("Length:"), this)),
    lELength (new QLineEdit (this)),
    cbWidth (new QCheckBox (tr("Width:"), this)),
    lEWidth (new QLineEdit (this)),
    wlRel (sp.width / sp.length),
    cbResolv (new QCheckBox (tr("Resolution:"), this)),
    lEResolv (new QLineEdit (this)),
    lAzimuth (new QLabel (tr("Azimuth:"), this)),
    lEAzimuth (new QLineEdit (this)),
    cbElev (new QCheckBox (tr("Elevation angle:"), this)),
    lEElev (new QLineEdit (this)),
    cbSecProp (new QCheckBox (tr("Secondary property:"), this)),
    lESec (new QLineEdit (this))
{
    QGridLayout * gLay = new QGridLayout (this);
    QGroupBox * gbImageParams = new QGroupBox (tr("Image parameters"), this);
    gLay->addWidget (gbImageParams, 0, 0, 1, 1);
    QGridLayout * gImLay = new QGridLayout ( gbImageParams );
    gImLay->addWidget (lLength, 0, 0, 1, 1);
    gImLay->addWidget (lELength, 0, 1, 1, 1);
    QDoubleValidator * dLVal = new QDoubleValidator (10, 500, 16, this);
    lELength->setValidator (dLVal);
    lELength->setText (QString::number (sp.length));

    gImLay->addWidget (cbWidth, 1, 0, 1, 1);
    gImLay->addWidget (lEWidth, 1, 1, 1, 1);
    QDoubleValidator * dWVal = new QDoubleValidator (10, 200, 16, this);
    lEWidth->setValidator (dWVal);
    lEWidth->setText (QString::number (sp.width));
    cbWidth->setCheckState (Qt::Unchecked);
    lEWidth->setEnabled (false);

    gImLay->addWidget (cbResolv, 2, 0, 1, 1);
    gImLay->addWidget (lEResolv, 2, 1, 1, 1);
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

    connect (lELength, SIGNAL (editingFinished (void)), this, SLOT (lEEditFinished(void)) );
    connect (lELength, SIGNAL (textEdited (const QString &)), this, SLOT (lETextEdited(const QString&)) );
    connect (cbWidth, SIGNAL (stateChanged(int)), this, SLOT (widthStateChanged(int)) );
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
    double w = lEWidth->isEnabled() ? lEWidth->text().toDouble () : -1.0;
    double d = /*lEDepth->isEnabled() ? lEDepth->text().toDouble() :*/ -1.0;
    double az = lEAzimuth->text().toDouble ();
    double elev = lEElev->isEnabled() ? lEElev->text().toDouble() : -1.0;
    double r = lEResolv->isEnabled() ? lEResolv->text().toDouble() : -1.0;
    QString sProp = lESec->isEnabled() ? lESec->text() : QString();
    SeaObjectParameters sp (sop.bRect, l, w, d, az, elev, r, sProp);
    return sp;
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

void ParamWidget :: widthStateChanged (int state)
{
    lEWidth->setEnabled ((state == Qt::Checked));
}

QValidator::State ParamWidget :: isValid (void) const
{
    int pos;
    QString lstr = lELength->text();
    QValidator::State w = lELength->validator()->validate (lstr, pos);
    if (w == QValidator::Acceptable)
    {
        lstr = lEWidth->text();
        w = lEWidth->validator()->validate (lstr, pos);
    }
    return w;
}

void ParamWidget :: lEEditFinished (void)
{
    QValidator::State w = isValid();
    if (w != QValidator::Acceptable)
    {
        QMessageBox::warning (this, tr("Parameters"), tr("Image is not geocoded"), QMessageBox::Ok);
        return;
    }
}

void ParamWidget :: lETextEdited (const QString& text)
{
    double l = text.toDouble ();
    double w = l*wlRel;
    if (cbWidth->checkState() == Qt::Unchecked)
        lEWidth->setText (QString::number (w));
}
