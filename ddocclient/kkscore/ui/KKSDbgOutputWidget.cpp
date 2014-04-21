#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>
#include <QMainWindow>
#include <QFont>
#include <QDateTime>
#include <QScrollArea>
#include <QScrollBar>
#include <QMutex>

#include "KKSDbgOutputWidget.h"
#include <kksdebug.h>
#include <defines.h>

//чтобы в окно отладки все сыпалось по очереди. Для поддержки многопоточности
QMutex mutex;

KKSDbgOutputWidget :: KKSDbgOutputWidget(bool bForDockable, QWidget *parent, Qt::WindowFlags flags)
    : QDockWidget (tr("Debug output window"), parent, flags)
{

    m_logWidget = new QTextEdit(this);
    //m_logWidget->setReadOnly(true);
    m_logWidget->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    m_logWidget->setWordWrapMode(QTextOption::NoWrap);
    m_logWidgetCursor = NULL;

    initWidget();

    this->setWidget (m_logWidget);
    this->initSysMenu (bForDockable);
}

KKSDbgOutputWidget :: ~KKSDbgOutputWidget(void)
{
    if(m_logWidgetCursor)
        delete m_logWidgetCursor;
}

void KKSDbgOutputWidget :: initMenuEmitting()
{
    if(!m_logWidget)
        return;

    m_logWidget->setContextMenuPolicy (Qt::CustomContextMenu);

    connect(m_logWidget,
            SIGNAL(customContextMenuRequested(const QPoint &)),
            this,
            SLOT(showSysContextMenu(const QPoint &))
            );

    //connect(m_
}

void KKSDbgOutputWidget :: initWidget()
{
    if(!m_logWidget)
        return;

    if(m_logWidgetCursor){
        delete m_logWidgetCursor;
        m_logWidgetCursor = NULL;
    }

    QTextDocument * doc = m_logWidget->document();
    if(!doc){
        return;
    }

    m_logWidgetCursor = new QTextCursor(doc);

    m_logWidgetCursor->setVisualNavigation(true);

    m_logWidgetCursor->atStart();
    QTextCharFormat charFormat;
    charFormat.setForeground(Qt::black);
    m_logWidgetCursor->atBlockStart();
    m_logWidgetCursor->insertText(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"));
    m_logWidgetCursor->insertText("\n\n");

}

void KKSDbgOutputWidget :: printMessage(Criticality c, const QString & message)
{
    //все остальные потоки, имеющие желание вывести отладочное сообщение будут ждать
    bool b = mutex.tryLock();
    if(!b)
        return;

    Criticality oldCriticality = m_criticality;
    m_criticality = c;
    //QString oldMessage = m_message;

    QString sCriticality = criticalityAsString(c);
    if(!m_logWidgetCursor){
        mutex.unlock();
        return;
    }
        
    QBrush brush;
        
    switch(m_criticality){
        case cCriticalMsg:
        case cFatalMsg:
            brush = QBrush(Qt::darkRed);
            break;
        case cWarningMsg:
            brush = QBrush(Qt::darkYellow);
            break;
        case cInfoMsg:
        case cImportantInfoMsg:
            brush = QBrush(Qt::darkGreen);
            break;
        default://cDebugMsg
            brush = QBrush(Qt::black);
            break;
    }

    QTextCharFormat charFormat;
    charFormat.setForeground(brush);
    //m_logWidgetCursor->insertBlock();
    m_logWidgetCursor->insertText(sCriticality + " " + message + "\n", charFormat);
    if(m_logWidget->verticalScrollBar()){
        if(m_logWidgetCursor->atEnd())
            m_logWidget->verticalScrollBar()->setValue(m_logWidget->verticalScrollBar()->maximum());
    }
    if(m_logWidget->horizontalScrollBar()){
        int v = m_logWidget->horizontalScrollBar()->minimum();
        m_logWidget->horizontalScrollBar()->setValue(v);
    }

    m_criticality = oldCriticality;
    //m_message = oldMessage;

    mutex.unlock();
}

QString KKSDbgOutputWidget :: criticalityAsString(Criticality c)
{
    QString s;
    switch (c){
        case cInfoMsg:
        case cImportantInfoMsg:
            s = QObject::tr("INFO:");
            break;
        case cWarningMsg:
            s = QObject::tr("WARNING!");
            break;
        default:
        case cCriticalMsg:
        case cFatalMsg:
            s = QObject::tr("ERROR!");
            break;
        case cDebugMsg:
            s = QObject::tr("DEBUG:");
            break;
    }

    return s;
}


void KKSDbgOutputWidget :: initSysMenu(bool bForDockable)
{
    floatingAction = new QAction(tr("Floating"), this);
    floatingAction->setCheckable(true);
    connect(floatingAction, SIGNAL(triggered(bool)), SLOT(changeFloating(bool)));

    areaActions = new QActionGroup(this);
    areaActions->setExclusive(true);

    leftAction = new QAction(tr("Place on Left") , this);
    leftAction->setCheckable(true);
    connect(leftAction, SIGNAL(triggered(bool)), SLOT(placeLeft(bool)));

    rightAction = new QAction(tr("Place on Right") , this);
    rightAction->setCheckable(true);
    connect(rightAction, SIGNAL(triggered(bool)), SLOT(placeRight(bool)));

    topAction = new QAction(tr("Place on Top") , this);
    topAction->setCheckable(true);
    connect(topAction, SIGNAL(triggered(bool)), SLOT(placeTop(bool)));

    bottomAction = new QAction(tr("Place on Bottom") , this);
    bottomAction->setCheckable(true);
    connect(bottomAction, SIGNAL(triggered(bool)), SLOT(placeBottom(bool)));

    areaActions->addAction(leftAction);
    areaActions->addAction(rightAction);
    areaActions->addAction(topAction);
    areaActions->addAction(bottomAction);

    m_clearLog = new QAction(tr("Clear log") , this);
    m_clearLog->setCheckable(false);
    connect(m_clearLog, SIGNAL(triggered()), SLOT(clearLog()));

    m_dbgMinLog = new QAction(tr("Show debug messages") , this);
    m_dbgMinLog->setCheckable(true);
    connect(m_dbgMinLog, SIGNAL(triggered(bool)), SLOT(showDbgMsg(bool)));

    m_infoMinLog = new QAction(tr("Show info messages") , this);
    m_infoMinLog->setCheckable(true);
    connect(m_infoMinLog, SIGNAL(triggered(bool)), SLOT(showInfoMsg(bool)));

    m_impInfoMinLog = new QAction(tr("Show important info messages") , this);
    m_impInfoMinLog->setCheckable(true);
    connect(m_impInfoMinLog, SIGNAL(triggered(bool)), SLOT(showImpInfoMsg(bool)));

    m_wrnMinLog = new QAction(tr("Show warning messages") , this);
    m_wrnMinLog->setCheckable(true);
    connect(m_wrnMinLog, SIGNAL(triggered(bool)), SLOT(showWrnMsg(bool)));

    m_errMinLog = new QAction(tr("Show error messages") , this);
    m_errMinLog->setCheckable(true);
    connect(m_errMinLog, SIGNAL(triggered(bool)), SLOT(showErrMsg(bool)));

    QActionGroup * dbgActions = new QActionGroup(this);
    dbgActions->setExclusive(true);
    dbgActions->addAction(m_dbgMinLog);
    dbgActions->addAction(m_infoMinLog);
    dbgActions->addAction(m_impInfoMinLog);
    dbgActions->addAction(m_wrnMinLog);
    dbgActions->addAction(m_errMinLog);
    switch(KKSDebug::minMsgType()){
        case KKSDebugMsg: m_dbgMinLog->setChecked(true); break;
        case KKSInfoMsg: m_infoMinLog->setChecked(true); break;
        case KKSImportantInfoMsg: m_impInfoMinLog->setChecked(true); break;
        case KKSWarningMsg: m_wrnMinLog->setChecked(true); break;
        case KKSCriticalMsg: m_errMinLog->setChecked(true); break;
    }


    m_pSysMenu = new QMenu(tr("Debug output"), this);
    
    if(bForDockable){
        m_pSysMenu->addAction(toggleViewAction());
        m_pSysMenu->addSeparator();
        QMenu * dbgMenu = new QMenu(tr("Log details"), this);
        dbgMenu->addActions(dbgActions->actions());
        m_pSysMenu->addAction(m_clearLog);
        m_pSysMenu->addMenu(dbgMenu);
        m_pSysMenu->addSeparator();
        m_pSysMenu->addAction(floatingAction);
        m_pSysMenu->addSeparator();
        m_pSysMenu->addActions(areaActions->actions());
    }
    else{
        m_pSysMenu->addAction(m_clearLog);
        m_pSysMenu->addSeparator();
        m_pSysMenu->addActions(dbgActions->actions());
    }

    //m_logWidget->s

}

void KKSDbgOutputWidget :: showSysContextMenu(const QPoint & pos)
{
    m_pSysMenu->popup(m_logWidget->mapToGlobal(pos));
}

void KKSDbgOutputWidget :: contextMenuEvent (QContextMenuEvent *e)
{
    e->accept ();
}

void KKSDbgOutputWidget :: placeLeft(bool p)
{
    place(Qt::LeftDockWidgetArea, p);
}

void KKSDbgOutputWidget :: placeRight(bool p)
{
    place(Qt::RightDockWidgetArea, p);
}

void KKSDbgOutputWidget :: placeTop(bool p)
{
    place(Qt::TopDockWidgetArea, p);
}

void KKSDbgOutputWidget :: placeBottom(bool p)
{
    place(Qt::BottomDockWidgetArea, p);
}

void KKSDbgOutputWidget :: changeFloating(bool on)
{
    setFloating (on);//floating);
}

void KKSDbgOutputWidget :: place(Qt::DockWidgetArea area, bool place)
{
    if (!place)
        return;

    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parentWidget());
    if(mainWindow)
        mainWindow->addDockWidget(area, this);

}

void KKSDbgOutputWidget :: showDbgMsg(bool yes)
{
    if(yes)
        KKSDebug::setMinMsgType(KKSDebugMsg);
}

void KKSDbgOutputWidget :: showInfoMsg(bool yes)
{
    if(yes)
        KKSDebug::setMinMsgType(KKSInfoMsg);
}

void KKSDbgOutputWidget :: showImpInfoMsg(bool yes)
{
    if(yes)
        KKSDebug::setMinMsgType(KKSImportantInfoMsg);
}

void KKSDbgOutputWidget :: showWrnMsg(bool yes)
{
    if(yes)
        KKSDebug::setMinMsgType(KKSWarningMsg);

}

void KKSDbgOutputWidget :: showErrMsg(bool yes)
{
    if(yes)
        KKSDebug::setMinMsgType(KKSCriticalMsg);
}

void KKSDbgOutputWidget :: clearLog()
{
    m_logWidget->clear();

    m_logWidgetCursor->atStart();
    QTextCharFormat charFormat;
    charFormat.setForeground(Qt::black);
    m_logWidgetCursor->atBlockStart();
    m_logWidgetCursor->insertText(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"));
    m_logWidgetCursor->insertText("\n\n");
}
