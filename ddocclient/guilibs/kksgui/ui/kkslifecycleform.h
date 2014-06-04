/* 
 * File:   kkslifecycleform.h
 * Author: yuriyrusinov
 *
 * Created on 28 Май 2013 г., 15:24
 */

#ifndef KKSLIFECYCLEFORM_H
#define	KKSLIFECYCLEFORM_H

#include <QDialog>
#include <QModelIndex>
#include <QVector>

#include "kksgui_config.h"

namespace Ui
{
    class kks_life_cycle_form;
};

class QLineEdit;
class QTextEdit;
class QAbstractItemModel;
class KKSLifeCycleEx;

class _GUI_EXPORT kkslifecycleform : public QDialog
{
public:
    kkslifecycleform(KKSLifeCycleEx * lc, bool mode=true, QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~kkslifecycleform();

    KKSLifeCycleEx * getLC (void) const;
    
    enum LCStates
    {
        lcStart = 1,
        lcAttrChanged = 2,
        lcIndChanged = 3
    };
    
    void updateLC (KKSLifeCycleEx * lc);

public slots:
    void save (void);

private slots:
    void lcAccept (void);
    void setState (void);
    void setStateAttr (void);
    void setStateInd (void);

    void addState (void);
    void delState (void);

    void clearState (void);
    void clearStateAttr (void);
    void clearStateInd (void);

signals:
    void loadState (KKSLifeCycleEx * lc, QLineEdit * le, int lcst, const QVector<qint64>& availableStates);
    void addLCState (KKSLifeCycleEx * lc, QAbstractItemModel * stateMod);
    void saveLifeCycle (KKSLifeCycleEx * lc);

private:
    //
    // Functions
    //
    void initStates (void);
    QVector<qint64> loadAvStates (void) const;
private:
    //
    // Variables
    //
    Ui::kks_life_cycle_form * UI;
    KKSLifeCycleEx * lifeCycle;
private:
    Q_OBJECT

};

#endif	/* KKSLIFECYCLEFORM_H */

