
/* 
 * File:   KKSDbgOutputWidget.h
 * Author: sergey Karin
 *
 * Created on 19 »юл€ 2013 г., 08:36
 */

#ifndef _KKSDBGOUTPUTWIDGET_H
#define	_KKSDBGOUTPUTWIDGET_H

#include <QDockWidget>
#include <QTextEdit>
#include <QTextCursor>

#include "config_core.h"

class QMenu;
class QAction;
class QActionGroup;

class __CORE_EXPORT KKSDbgOutputWidget : public QDockWidget
{
    Q_OBJECT

public:

    KKSDbgOutputWidget(bool bForDockable, QWidget *parent = 0, Qt::WindowFlags flags = 0);
    virtual ~KKSDbgOutputWidget(void);

    void initMenuEmitting();

    enum Criticality {
        cDebugMsg         =   0, //отладочное
        cInfoMsg          =   1, //блок информации
        cImportantInfoMsg =   2, //блок важной информации, который также должен быть представлен в виде QMessageBox
        cWarningMsg       =   3, //предупреждение
        cCriticalMsg      =   4, //ошибка
        cFatalMsg         =   5  //системна€ ошибка
    };



public slots:

    void printMessage(Criticality c, const QString & message);

    void showSysContextMenu(const QPoint & pos);
    void changeFloating(bool on);

    void placeLeft(bool p);
    void placeRight(bool p);
    void placeTop(bool p);
    void placeBottom(bool p);

    void showDbgMsg(bool);
    void showInfoMsg(bool);
    void showImpInfoMsg(bool);
    void showWrnMsg(bool);
    void showErrMsg(bool);

    void clearLog();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *e);
    //virtual bool event(QEvent *);

private:
    //
    // Functions
    //
    void initWidget(void);
    

    void initSysMenu(bool bForDockable);

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

    QAction * m_clearLog;
    QAction * m_dbgMinLog;
    QAction * m_infoMinLog;
    QAction * m_impInfoMinLog;
    QAction * m_wrnMinLog;
    QAction * m_errMinLog;

    QTextEdit * m_logWidget;
    QTextCursor * m_logWidgetCursor;

};

#endif	/* _KKSDBGOUTPUTWIDGET_H */

