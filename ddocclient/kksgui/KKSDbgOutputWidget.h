
/* 
 * File:   KKSDbgOutputWidget.h
 * Author: sergey Karin
 *
 * Created on 19 Èþëÿ 2013 ã., 08:36
 */

#ifndef _KKSDBGOUTPUTWIDGET_H
#define	_KKSDBGOUTPUTWIDGET_H

#include <QDockWidget>
#include <QTextEdit>
#include <QTextCursor>

#include "kksgui_config.h"

class QMenu;
class QAction;
class QActionGroup;

class _GUI_EXPORT KKSDbgOutputWidget : public QDockWidget
{
    Q_OBJECT

public:

    KKSDbgOutputWidget(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~KKSDbgOutputWidget(void);

    void initMenuEmitting();

    enum Criticality {
        cMessage = 0,
        cWarning = 1,
        cError = 2,
        cCritical = 3
    };



public slots:

    void printMessage(Criticality c, const QString & message);

    void showSysContextMenu(const QPoint & pos);
    void changeFloating(bool on);

    void placeLeft(bool p);
    void placeRight(bool p);
    void placeTop(bool p);
    void placeBottom(bool p);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *e);
    //virtual bool event(QEvent *);

private:
    //
    // Functions
    //
    void initWidget(void);
    

    void initSysMenu();

    void place(Qt::DockWidgetArea area, bool place);
    QString criticalityAsString(Criticality c);

private:
    //
    // Variables
    //
    Criticality m_criticality;

    QMenu * m_pSysMenu;
    QAction *floatingAction;

    QActionGroup *areaActions;
    QAction *leftAction;
    QAction *rightAction;
    QAction *topAction;
    QAction *bottomAction;

    QTextEdit * m_logWidget;
    QTextCursor * m_logWidgetCursor;

};

#endif	/* _KKSDBGOUTPUTWIDGET_H */

