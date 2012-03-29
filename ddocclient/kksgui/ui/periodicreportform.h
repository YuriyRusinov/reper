#ifndef _PeriodicReportForm_H
#define _PeriodicReportForm_H

#include <kksgui_config.h>
#include <QDialog>
#include "KKSList.h"

class QDateTime;

class KKSCategory;
class KKSObjectExemplar;
class KKSAttrValue;
class KKSCategoryAttr;

namespace Ui
{
    class periodic_report_form;
};

class _GUI_EXPORT PeriodicReportForm : public QDialog
{
public:
    PeriodicReportForm (bool isCreate=true, QWidget * parent=0, Qt::WFlags f=0);
    ~PeriodicReportForm (void);

    KKSObjectExemplar * getReport (void) const;
    void setReport (KKSObjectExemplar * peio);

    const KKSList<KKSAttrValue *>& attrValues() const;
    KKSList<KKSAttrValue *>& attrValues (void);
    void setValues (const KKSList<KKSAttrValue *>& vals);

public slots:
    void constructReport (void);

signals:
    void loadReference (QString tableName, QWidget * valW, int attrId);

private slots:
    void loadCategory (void);
    void loadAddressee (void);
    void loadController (void);
    void loadExecutor (void);
    void periodCheckStateChanged (int state);
    void setDeadLine (const QDateTime& datetime);
    void periodTimeChanged (const QString & text);
    void periodUnitChanged (int index);
    void setReason (void);
    void archiveStateChanged (int state);
    void loadState (void);

private:
    //
    // Variables
    //
    Ui::periodic_report_form * UI;
    KKSObjectExemplar * periodicEIO;
    KKSList<KKSAttrValue *> attrVals;
private:
    Q_OBJECT
};

#endif
