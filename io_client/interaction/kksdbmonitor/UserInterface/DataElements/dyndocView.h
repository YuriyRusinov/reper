#ifndef _DYNDOC_SYNCH_H
#define _DYNDOC_SYNCH_H

#include <QTreeView>

class dyndocView : public QTreeView
{
    Q_OBJECT

    public:
        dyndocView(QWidget* parent = 0);
        ~dyndocView();
};

#endif
