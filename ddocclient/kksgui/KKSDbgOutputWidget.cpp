#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>
#include <QMainWindow>
#include <QFont>
#include <QDateTime>
#include <QScrollArea>
#include <QScrollBar>

#include "KKSDbgOutputWidget.h"

#include <defines.h>

KKSDbgOutputWidget :: KKSDbgOutputWidget(QWidget *parent, Qt::WindowFlags flags)
    : QDockWidget (tr("Debug output window"), parent, flags)
{

    m_logWidget = new QTextEdit(this);
    //m_logWidget->setReadOnly(true);
    m_logWidget->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    m_logWidget->setWordWrapMode(QTextOption::NoWrap);
    m_logWidgetCursor = NULL;

    initWidget();

    this->setWidget (m_logWidget);
    this->initSysMenu ();
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
    m_logWidgetCursor->insertText(QDateTime::currentDateTime().toString(Qt::TextDate));
    m_logWidgetCursor->insertText("\n");

}

void KKSDbgOutputWidget :: printMessage(Criticality c, const QString & message)
{
    Criticality oldCriticality = m_criticality;
    m_criticality = c;
    //QString oldMessage = m_message;

    QString sCriticality = criticalityAsString(c);
    if(!m_logWidgetCursor)
        return;
        
    QBrush brush;
        
    switch(m_criticality){
        case cError:
        case cCritical:
            brush = QBrush(Qt::darkRed);
            break;
        case cWarning:
            brush = QBrush(Qt::darkYellow);
            break;
        case cMessage:
            brush = QBrush(Qt::darkGreen);
            break;
        default:
            brush = QBrush(Qt::black);
            break;
    }

    QTextCharFormat charFormat;
    charFormat.setForeground(brush);
    m_logWidgetCursor->insertBlock();
    m_logWidgetCursor->insertText(sCriticality + " " + message, charFormat);
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
}

QString KKSDbgOutputWidget :: criticalityAsString(Criticality c)
{
    QString s;
    switch (c){
        case cMessage:
            s = QObject::tr("MESSAGE.");
            break;
        case cWarning:
            s = QObject::tr("WARNING!");
            break;
        default:
        case cError:
            s = QObject::tr("ERROR!");
            break;
        case cCritical:
            s = QObject::tr("CRITICAL!");
            break;
    }

    return s;
}


void KKSDbgOutputWidget :: initSysMenu()
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

    m_pSysMenu = new QMenu(tr("Debug output"), this);
    m_pSysMenu->addAction(toggleViewAction());
    m_pSysMenu->addSeparator();
    m_pSysMenu->addAction(floatingAction);
    m_pSysMenu->addSeparator();
    m_pSysMenu->addActions(areaActions->actions());

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
