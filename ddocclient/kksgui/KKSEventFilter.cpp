#include <QEvent>
#include <QMouseEvent>
#include <QTreeView>
#include <QModelIndex>
#include <QPoint>
#include <QItemSelectionModel>
#include <QtDebug>

#include "KKSEventFilter.h"

KKSEventFilter :: KKSEventFilter (QObject *parent)
    : QObject (parent)
{
}

KKSEventFilter :: ~KKSEventFilter (void)
{
}

bool KKSEventFilter :: eventFilter (QObject *obj, QEvent *event)
{
    if (obj->parent() && obj->parent()->inherits ("QTreeView") && event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent * mEvent = static_cast<QMouseEvent *> (event);

        if (mEvent->button() != Qt::LeftButton)
            return QObject::eventFilter (obj, event);

        QTreeView * treeObj = static_cast<QTreeView *> (obj->parent());
        QPoint p (mEvent->x(), mEvent->y());
        QModelIndex index = treeObj->indexAt (p);
        if (index.isValid())
            return false;
        else
        {
            if(treeObj)
                if(treeObj->selectionModel()) 
                    treeObj->selectionModel ()->setCurrentIndex (index, QItemSelectionModel::Clear);
            
            return true;
        }
    }
    else
        return QObject::eventFilter (obj, event);
}
