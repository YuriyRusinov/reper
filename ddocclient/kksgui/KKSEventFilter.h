#ifndef _KKSEventFilter_H
#define _KKSEventFilter_H

#include <kksgui_config.h>

#include <QObject>

class _GUI_EXPORT KKSEventFilter : public QObject
{
    public:
        KKSEventFilter (QObject *parent=0);
        ~KKSEventFilter (void);

    protected:
        bool eventFilter (QObject *obj, QEvent *event);

    private:
        Q_OBJECT
};

#endif
