/***********************************************************************
 * Module:  KKSPointsItemDelegate.h
 * Author:  yuriy
 * Modified: 27 ����� 2010 �. 19:20
 * Purpose: Declaration of the class KKSPointsItemDelegate
 * Comment: ����� ����� 㭠᫥����� �� QItemDelegate � 
 * �।�����祭 ��� �⮡ࠦ���� ������ � ⠡����.
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSPointsItemDelegate_h)
#define __KKSSITOOOM_KKSPointsItemDelegate_h

#include <QItemDelegate>

#include "kksgui_config.h"

class _GUI_EXPORT KKSPointsItemDelegate : public QItemDelegate
{
public:
    KKSPointsItemDelegate (QObject *parent=0);
    ~KKSPointsItemDelegate (void);

    virtual QWidget * createEditor (QWidget * parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual void setEditorData (QWidget * editor, const QModelIndex& index) const;
    virtual void setModelData (QWidget * editor, QAbstractItemModel * model, const QModelIndex& index) const;

private:
    Q_OBJECT
};
#endif
