#ifndef _DYNDOC_SYNCH_H
#define _DYNDOC_SYNCH_H

#include <QTreeView>

class dyndocTreeView : public QTreeView
{
    Q_OBJECT

    public:
        dyndocTreeView(QWidget* parent);
        ~dyndocTreeView();
    private:
};

#endif
