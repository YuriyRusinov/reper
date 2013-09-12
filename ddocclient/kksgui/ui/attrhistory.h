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
class QContextMenuEvent;
class QMenu;
class QAction;
class QModelIndex;
class QItemSelection;

namespace Ui
{
    class attr_history;
};

class _GUI_EXPORT AttrHistory : public QDialog
{
public:
    AttrHistory (const KKSList<KKSAttrValue*> & histlist, QWidget *parent=0, Qt::WFlags f=0);
    virtual ~AttrHistory();

private slots:
    void viewVal (void);
    void viewDblVal (const QModelIndex&);
    void upClicked (void);
    void downClicked (void);

    void histSelectionChanged (const QItemSelection& selected, const QItemSelection& deselected);

protected:
    //
    // Overrides functions
    //
    void contextMenuEvent (QContextMenuEvent * event);
    
private:
    //
    // Functions
    //
    void view(const KKSList<KKSAttrValue*> &);

signals:
    void viewAttrValue (int, QWidget *);

private:
    //
    // Variables
    //
    Ui::attr_history *UI;
    QMenu * pHistMenu;
    QAction * aViewVals;
    Q_OBJECT
};

#endif //_AttrHistory_H
