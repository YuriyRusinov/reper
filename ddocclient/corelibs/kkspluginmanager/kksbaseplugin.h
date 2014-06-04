#ifndef KKSBASEPLUGIN_H
#define KKSBASEPLUGIN_H

#include <QPixmap>
#include <QString>
#include <QAction>
#include <QTextEdit>

#include <kkspluginmanager_config.h>

class KKSCoreApplication;

/*!\ingroup PluginGroup
\brief ������� ����������� ����� (���������) ��� ���� ��������, ����������� ��� ���������� DynamicDocs.

��� ������� ������ ���� ��������� �������� ��� ���������� �� ������� ������.*/
class _KKSPLUGIN_EXPORT KKSBasePlugin
{
    public:
        KKSBasePlugin() : action(0) {}
        virtual ~KKSBasePlugin(){}

        virtual QString getName() const = 0; //!<���������� �������� �������. ���������� �������������� ������ ����� � �������� �������
        virtual QString getDesc() const = 0;//!<���������� �������� �������. ���������� �������������� ������ ����� � �������� �������
        virtual QPixmap getPixmap() const = 0;//!<���������� ������, ������������ ��� ����������� ��������������� ������ � ���� ��� �� ������ ������������. ���������� �������������� ������ ����� � �������� �������.

        /*!\brief ����� ������������� ������ ������ QAction, ��������������� ������� �������.
         
          ��������������, ��� ������� ������� ������������� ������ QAction
          (������ �� ������ ������������ �/��� ����). ��� ��������� ������� ���������
          ������ ������ ������ ������������� ����������� ������ setAction() � getPixmap() (����� ����� �������������� ����� getAction()).
          ��� ���� ��� ������ �� ������ ��������� ������ QAction.
          ��������� ��������� � ������ initPlugin() ������ PluginLoader, �������
          ��������� �������� ���� ��������� ��������.
         
          ���������� �������������� ������ ����� � �������� ������� ����� �������, ����� � ��� ��������� ����� QObject::connect(action, SIGNAL(activated()), this, SLOT(theMethod())), ��� theMethod() - �����, ������� ������ ����������� ��� ������� �� ������.
          
          ����� ���� ���������� ��������� �������:
          \code
            void WsViewBasePlugin::setAction( QAction * act )
            {
                if ( action )
                    disconnect( action, SIGNAL( toggled(bool) ), this, SLOT( setActive(bool) ) );
                action = act;
                if( action )
                {
                    action->setCheckable( TRUE );
                    connect( action, SIGNAL( toggled(bool) ), this, SLOT( setActive(bool) ) );
                }
            }
          \endcode
         */
        virtual void setAction(QAction * _action) = 0;

        /*!\brief ����� ���������� ������ QAction, ��������������� ������� �������.
         
         */
        virtual QAction * getAction() { return action; }

        /*!\brief ����� ���������� ������ QWidget ��� ������� �������, ���������� ������� ������������. ���� �������� ����� (��������������� ������) ����� ������ ������������, �� ���������� �������������� ������ ����� � ���� ������. */
        virtual QWidget * getToolWindow ( void ) { return 0; }

        virtual void setLogWidget(QTextEdit * logWidget) {Q_UNUSED(logWidget);}
        virtual QTextEdit * logWidget() {return NULL;}

        /*!\brief ����� ��������� ��������� �������. 
        
        ����� ���� ������������� � ������ �������� ������ (�������). �������� ������� ������ ����������� � ���, ��� �� ���������� ��������� ��������� �������, ������� ������������ ��� ������������ ������� triggered() ����������� �������� ���������� ������ �������� (��. �������� ������ setAction()).
        
        � ������� ���������� ������ ����� ������ �������� QAction::trigger().
        
        ��������� ������ ����� ������������� ����� ��� �������������� � �������������, �� � �������� ��������� ������ ����� ��������� ��������� �� ������������ ����. ������������� ������������ ��� � �������� ��������� ��� �������� ���� ������ �������.

        \param parent ��������� �� ������������ ����*/
        virtual int activate(QWidget * parent = NULL) {Q_UNUSED(parent); if(action){ action->trigger(); return 1;} return 0;}

        virtual void setFactoryManager(KKSCoreApplication * fManager) {Q_UNUSED(fManager);}

    protected:
        QAction * action;
};

Q_DECLARE_INTERFACE(KKSBasePlugin, "org.dynamicsoft.dynamicdocs.KKSBasePlugin/1.0");

#endif

/*!\defgroup PluginGroup ������, ��������������� ��� �������� ���������� (��������) ��� ���������� DynamicDocs

DynamicDocs ��������� ��������, ����������� �������������, ������������ ��, ��������� ��� ��� ���������� (�������). 
��� ��������� �������� ������� � ������ ������� ����������������� ������������ � �������������� ���������� Qt ������ 4.x. 
����� ����, ������� ������ ������������� �����������, ������������� � ��� ����������� DynamicDocs.

��� ������� ��� DynamicDocs ������ ���� ��������� �� �������� ������������ ������ KKSBasePlugin � �������������� ��� ��� ����������� ������. 
���� ������� ���������� �� ������� DynamicDocs �� �������������. ��� ���� ���������� ��� �������������� ����������, ������������� ���������� ������������ ����������. 

��� ���� ���������� �����������, ��� ���� ������ �������� �� �� KKSBasePlugin
�� �� ������ ���� ����� �������� �� KKSBasePlugin. � ��������� ������ ������ �� ���������� ����������� DynamicDocs. ������:
\code
...
class ZoomInPlugin : public QObject,
                     public KKSBasePlugin,
                     public KKSGISPlugin
{
    Q_OBJECT
    Q_INTERFACES(KKSBasePlugin);
    Q_INTERFACES(KKSGISPlugin);
    public:
        ...
};
\endcode

��������������, ��� ������ ������ ����� ����� ��������� �� ��������������� ������ ������ QAction. 
��� ��������� ������� ��������� ������ ������ ������ ������������� ����������� ������ KKSBasePlugin::setAction() � KKSBasePlugin::getPixmap(). 
��� ���� ��� ������ �� ������ ��������� ������ QAction. ��������� ��������� � ������ KKSPluginLoader::initPlugin(), ������� ��������� �������� ���� ��������� �������� � ��������� ����������� � ����.

����� �������� ������� �������� �������� ������ � ��������� \link Plugins ������� ���������� (��������) ���������� DynamicDocs\endlink.
*/

/*!\page Plugins ������� ���������� (��������) ���������� DynamicDocs

DynamicDocs ��������� ��������, ����������� �������������, ������������ ��, ��������� ��� ��� ���������� (�������). 
��� ��������� �������� ������� � ������ ������� ����������������� ������������ � �������������� ���������� Qt ������ 4.x. 
����� ����, ������� ������ ������������� �����������, ������������� � ��� ����������� DynamicDocs.

��� ������� ��� ���������� DynamicDocs ������ ���� ��������� �� �������� ������������ ������ KKSBasePlugin � �������������� ��� ��� ����������� ������. 
���� ������� ���������� �� ������� DynamicDocs �� �������������. ��� ���� ���������� ��� �������������� ����������, ������������� ���������� ������������ ���������� DynamicDocs. 

��� ���� ���������� �����������, ��� ���� ������ �������� �� �� KKSBasePlugin
�� �� ������ ���� ����� �������� �� KKSBasePlugin. � ��������� ������ ������ �� ���������� ����������� DynamicDocs. ������:
\code
...
class ZoomInPlugin : public QObject,
                     public KKSBasePlugin,
                     public KKSGISPlugin
{
    Q_OBJECT
    Q_INTERFACES(KKSBasePlugin);
    Q_INTERFACES(KKSGISPlugin);
    public:
        ...
};
\endcode

��������������, ��� ������ ������ ����� ����� ��������� �� ��������������� ������ ������ QAction. ��� ��������� ������� ��������� ������ ������ 
������ ������������� ����������� ������ KKSBasePlugin::setAction() � KKSBasePlugin::getPixmap(). ��� ���� ��� ������ �� ������ ��������� ������ QAction. 
��������� ��������� � ������ KKSPluginLoader::initPlugin(), ������� ��������� �������� ���� ��������� �������� � ��������� ����������� � ����.

���������� �������� ������ (����������) ��� ���� �������� DynamicDocs:
\code
class KKSBasePlugin
{
    public:
        KKSBasePlugin() : action(0) {}
        virtual ~KKSBasePlugin(){}

        virtual QString getName() const = 0;
        virtual QString getDesc() const = 0;
        virtual QPixmap getPixmap() const = 0;

        virtual void setAction(QAction * _action) = 0;

        virtual QAction * getAction() { return action; }

    protected:
        QAction * action;
};

Q_DECLARE_INTERFACE(XGisBasePlugin, "org.nil5.xgis.XGisBasePlugin/1.0");
\endcode

��� ���������� ������� ���������� ��������� ��������� ����:
   \li �������� ����� ������� � ������������ ��� �� QObject � �� ����������� �������, ������� ������ ������ ������ �������������. ���������� ������������ ����� ������ Q_OBJECT.
   \li ��� ����, ����� ���� ����� ������� Qt �� ������������ ������ �������� ����������� ����������� ������ Q_INTERFACES().
   \li �������������� ������ ��� ������ ������� Q_EXPORT_PLUGIN(). ������ ������ ������ � ���� ����������� ������ ���� ���. ������� ����� ������ ������ �������� ����� ����� ����� *.cpp.
   \li �������������� � ������� ������ ��� ������������ ����������� ����������, ��������� ��������������� *.pro ����.

������ ����� *.pro:
\code
TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += ../../src #���� � ������������ ������ libxgis
win32:LIBS += -L../../build -lxgis #���� � ���������� libxgis
HEADERS += zoominplugin.h Zoomin.xpm
SOURCES += zoominplugin.cpp
\endcode

\section BuildKey ���� ������ (Build Key) ��������

��� �������� �������� ������� Qt ��������� �.�. ���� ������ ������� �� ���. ������� ����� ���������. ��������� ������ ������� ������� ������ ����� �� �� ������������, ��� � ����������� �� ����������. �������, � ������� ��������� ������ ���������� �� ��������������� ���������� ����������, ��������� �� �����.

��������� ������. ��������, ���������� ���������� � �������������� ����������� qmake. ��������������, ��� ����� ����� ���� ������� *.pro, ������ �������� �������� ����:
\code
....
CONFIG  += debug
....
\endcode

�.�. ���������� ���������� � ������ �������.

��������, ��������������� ����� ������, ������� ������ ����������� ������������� �����������. ����� �� ����� ���������� � �������������� ����������� qmake. ��������������, �� ����� ����� ���� �������, ������ �������� �������� ����:
\code
....
CONFIG  += plugin release
....
\endcode

�.�. ������ ���������� � ������ ������.

� ����� �������� ������� �������� ������� ����������� �������� ���������, ��������� ���������� � ������ ����� ������ ��������� ������ (���������� ���������� � ���������� ������, � ������ - � ������ ������). �������� �������� ����� ������ � ����������, � ������� � ����� � ��� �� ������, �������� � ����������. ��� ���������� ���� ������� ������ ��������� ��������� �������:
\code
....
CONFIG  += debug
....
\endcode

��� �������:
\code
....
CONFIG  += plugin debug
....
\endcode

����� �������� �� ���� �������� � ������������ �� ���������� Qt http://doc.trolltech.com/4.0/plugins-howto.html#the-build-key .

\section WsViewPlugins ������� ��� ���������� ����������� �������� �����

��� �������������� � ������������� � ���������� ����������� � ����������� �������������� �������� ����� ������������ ����� WsView. �� ����, ������� ��� ������ ���������� ��������������� ������ � WsView. ��� �������������� ����� �������������� � ���� �������: � ������������� � � ���������������.

\subsection WsViewBasePlugins �������, ����������������� � ����������� ����������� �������� � ��������������� ������

����� ������� ������������� ��� ���������� �����-���� ��������� �������� (��������), �.�. ������ ���������� �������� � ������ ������ ��������� ������ WsView (������������ ������� WsContainer::getActiveWorkspace()), ��������� �������� � ��������� ������. �������� ������ ������� ����� ������� �������� ��������� �����.

�������� ������� ������ ���� ��������� �� ������� XGisBasePlugin � WsViewBasePlugin. ��������� ����� ��� �������� ������: WsViewBasePlugin::getActiveWsView() � WsViewBasePlugin::setWsContainer().

\subsection WsViewModePlugins �������, ����������������� � ����������� ����������� �������� � ������������� ������

����� ������� ������������ �������� ������������ ��� ��� �������������� � WsView. ������ ����� ���������� ������� ��� ������������� ��������� �������:
\li ������������ �������� ������ ����� (mousePress);
\li ������������ �������� ������ ���� (mouseRelease);
\li ������������ �������� �������� ���� (mouseMove);
\li ������������ �������� ������� ������ ����� (mouseDoubleClick);
\li ������������ ������� ������� ������� ������� ���� ����� (viewportResize);
\li ��������� �������� ������ ������������ ���� (mouseContextMenu).

��� ������������� �������, ����������������� � WsView, ������ ���� ������������ �� ������� XGisBasePlugin, WsViewBasePlugin � WsViewModePlugin. ����� ���������� ��������, ��� �������� ������ ������ WsView ������� ����������. �� �������� � ���������� ������������ WsView �������� ����� WsContainer. ������ WsView � ������ ������ ������� ��������� � ������������ ������ ����������������. ������������ ���������� � ���������� ������� ����������. ����� ���������������� ����������� ������� Mode. ������ �� ����������� ������������� �������� ������������ � ������� ���� ��������� ����� (��. ����� ModeList::registerMode()). ��� ������ ������� � ������ �������, ������� ����������� ������� ModeList. ����� PluginLoader ������ ������ �������, � ������� ����� ���������� ��������� � ������� WsContainer ���������� WsView. ���� ������ ������������ � ������ �������� ����������, �.�. � ������ ������ ������ PluginLoader::getPluginActions().

������ ������������� ������ ��� WsView ������ ��������� ���� ��������� ������ Mode, �.�. ���� ����� ����������������. � ���������� ��� �������� ������� ������� ��� ����� ����� ��������������� � ModeList ������� PluginLoader � ������� ���������� �����. � ���������� ������������ ����������� ���� ������� ����� ������ ��� ������, � ������ �������� ��������� ������ �������� WsView.

��� ���������������� ����� ��������� ����������, ����� ������ ������ ������������ ��������� ����������� ������ ���������� WsViewModePlugin:
- getMode();
- leaveMode();
- enterMode();
- failedEnterMode();
- setActive();
- addWsView();
- suspendListen();
- resumeListen().

���������� ������� �� ������������� ���� ������� ������� � ��������������� �������� ������������.

��� �������� ���������� ��������� ����� ���� ��������� ������� �������, ��� ����������� �� ���������� ��������� WsView. ����� �������, ���� ������ �������� �� ������ ���������� WsView. ��� ����, ����� ������, ����� �� ����������� WsView � ������ ������ �������� ��������, ������������ ����� WsViewBasePlugin::getActiveWsView(). �����, ��� ��������� �������� WsView ������ ������� ���������� ���������, ����� �� WsView �������� ��������, �.�. �������� ����� WsViewBasePlugin::getActiveWsView().
 
��� ����������� ������� ������������ ��������� ��������.

������ ������ ������������� ����� setAction(), � ������� ������ ���� ������� ���������:
\code
void ThePlugin::setAction( QAction * act )
{
    if ( action )
        disconnect( action, SIGNAL( toggled(bool) ), this, SLOT( setActive(bool) ) );
    action = act;
    if( action )
    {
        action->setCheckable( TRUE );
        connect( action, SIGNAL( toggled(bool) ), this, SLOT( setActive(bool) ) );
    }
}
\endcode
������ ������� toggled() ����� �������������� ����� ������ ���������� ������ ������ QAction. ������ ��� ������ ������������ ����� �������� ������ QAction � ������ setActive() ������� �������. �������������� ������ ���� ���������� ����� setActive(), � ������� ������ ���������� ����� setMode() ������ WsView. ����� setActive() ����� ���� ����������, ��������, ��������� �������:
\code
void ThePlugin::setActive( bool on )
{
    if(!wsContainer) return;
    wsContainer->setMode(on ? mode : NULL);
}
\endcode
� ������ ���������� ��� ������� �� ������ (�.�. ��� �������� �� � ��������� isChecked() == TRUE) ���������� ����� WsContainer::setMode(), � �������� ��������� ��������� ��������� ��������� �� ����� ������� �������. � ��������� ������ � �������� ��������� setMode() ��������� NULL, ����������� �� ��, ��� �� ������ ������� ������� ���������� �����. ��� ���� �� �������� ������ ������� ������ �������� ��������� WsView.

����� � ������ WsContainer::setMode() �����������, ����� �� ����������� WsView �������� ��������. ��� ���� ���������� ����� WsView::setMode(). � ���� ������ ������������ ������ leaveMode, ������� �������� �� ������ leaveMode() �������. ���� � ������ ������ ������� ����� �� ������ ������ ������� ������� ��������, �� ���� �������� ����� WsView::setCurrentMode(NULL). � ��������� ������ �� ������ �� ������. ����� ����� ����� WsView::setMode() � ������, ���� ����� �� �������� ������ ������� (�.�. �� ����� NULL), ���������� ������ enterMode, ������� �������� �� ������ enterMode() �������. ������ ���� ��������� ��������, ����������� ��� ����, ����� ������ ����� �������� � ��������� ������� WsView. ��������, �� ����� ��������� ���:
\code
void ThePlugin::enterMode(Mode * mode)
{
  WsView * view = getActiveWsView();
  if(!mode || !view || mode->getNumber() != this->mode->getNumber()) return;

  connect(view, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(mousePress(QMouseEvent *)));
  view->setCurrentMode(mode);
}
\endcode

��� ��������� ����������� ������ � ����������� WsView ���������� ����������� ������ resumeListen() � suspendListen(). � ������� ��������� ��������, ����������� ��� ������������� � ������������ ������ ������� �������������� (�.�. connect ��� disconnect ��� ���� �������������� �������� ��������������). ����� ���������� ����������� ���������� ������ save() � restore(), ������� ��������� ��������� ���������� ���������� ������� � �������������� �� ���������������. ���������� ���������� ������� �������� ��� \code QHash<WsView*, QVariant> stack;\endcode. �.�. ������ ���������� �������� ��� QVariant.
��������� ���� ������� �������� ���������:
\code
class MoveMapPlugin : public QObject,
                      public XGisBasePlugin,
                      public WsViewBasePlugin,
                      public WsViewModePlugin
{
    Q_OBJECT
    Q_INTERFACES(XGisBasePlugin);
    Q_INTERFACES(WsViewBasePlugin);
    Q_INTERFACES(WsViewModePlugin);

    public:
        MoveMapPlugin(){ mi = NULL; }
        ~MoveMapPlugin();

        QString getName()const{return QString(tr("Move map"));}
        QString getDesc()const{return QString(tr("Move map"));}
        QPixmap getPixmap()const{return QPixmap(Pan);}
        
        void setAction(QAction * _action);
        Mode * getMode();
        void addWsView( WsView * _view );

    public slots:
        void leaveMode(Mode * mode);
        void enterMode(Mode * mode);
        void suspendListen(WsView * view, Mode * mode);
        void resumeListen(WsView * view, Mode * mode);
        void failedEnterMode(Mode * mode);
        void setActive( bool on );

        void mousePress(QMouseEvent * e);
        void mouseMove(QMouseEvent * e);
        void mouseRelease(QMouseEvent * e);
    private:
        class MapImage : public QtCanvasRectangle
        {
            public:
                MapImage( QImage p, QtCanvas * c) : QtCanvasRectangle(c)
                {
                    setSize(p.width(), p.height());
                    pix = QPixmap::fromImage(p);
                }
            protected:
                void drawShape( QPainter & p)
                {
                    p.drawPixmap( (int)x(), (int)y(), pix);
                }
            private:
                QPixmap pix;
        };

        MapImage * mi;
        QPoint start;
        QPoint current;
        QPixmap screenCopy;

        void createMode();
    
    protected:
        void save(WsView * view);
        void restore(WsView * view);
        void checkAction(QObject * plugin);
        void uncheckAction(QObject * plugin);
};

//......

void MoveMapPlugin::suspendListen( WsView * view, Mode * mode )
{
    if(!mode || !view || mode->getNumber() != this->mode->getNumber()) return;
    
    disconnect(view, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(mousePress(QMouseEvent *)));
    disconnect(view, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(mouseMove(QMouseEvent *)));
    disconnect(view, SIGNAL(mouseRelease(QMouseEvent *)), this, SLOT(mouseRelease(QMouseEvent *)));
    uncheckAction(this);
  
    save(view);
}

void MoveMapPlugin::resumeListen( WsView * view, Mode * mode )
{
    if(!mode || !view || mode->getNumber() != this->mode->getNumber()) return;
    restore(view);
     
    connect(view, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(mousePress(QMouseEvent *)));
    connect(view, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(mouseMove(QMouseEvent *)));
    connect(view, SIGNAL(mouseRelease(QMouseEvent *)), this, SLOT(mouseRelease(QMouseEvent *)));
    checkAction(this);
}

void MoveMapPlugin::save( WsView * view )
{
    QVariant data;
    QVariantList list;
    
    list.append(qVariantFromValue((QObject*)mi));
    list.append(start);
    list.append(current);
    list.append(screenCopy);
    data.setValue(list);
    stack.insert(view, data);
}

void MoveMapPlugin::restore( WsView * view )
{
    QVariant data = stack.take(view);
    QVariantList list = data.toList();
    QVariant v = list.at(0);
    QObject * o = NULL;
    if(v.canConvert<QObject*>()) 
        o = v.value<QObject*>();
    mi = (MapImage*) o;
    start = list.at(1).value<QPoint>();
    current = list.at(2).value<QPoint>();
    screenCopy = list.at(3).value<QPixmap>();
}

\endcode

�� ���� ��� ��������������� ��������, ����������� ��� ���������������� �������, �����������. �������� ���� ����������� ������ ������ �������. ������ ����������� ������� �������� ����. ������ ������ ��������� �������� ������ �����.

���������� �������� ������ (����������) ��� ���� �������� ���������� ����������� � ����������� ��������������:
\code
class WsViewModePlugin
{
    public:
        WsViewModePlugin() : mode(0) {}
        virtual ~WsViewModePlugin() {}

        virtual Mode * getMode() = 0;
        
        virtual void addWsView(WsView * _view) = 0;

    public slots:
        virtual void setActive( bool on ) = 0;
        
        virtual void leaveMode(Mode * mode) = 0;

        virtual void enterMode(Mode * mode) = 0;

        virtual void failedEnterMode( Mode * _mode ) = 0;
        
        virtual void suspendListen(WsView * view, Mode * mode) = 0;
        
        virtual void resumeListen(WsView * view, Mode * mode) = 0;

    protected:
        Mode * mode;
        QHash<WsView*, QVariant> stack;

        virtual void save(WsView * view){}
        
        virtual void restore(WsView * view){}
};

Q_DECLARE_INTERFACE(WsViewModePlugin, "org.nil5.xgis.WsViewModePlugin/1.0");
\endcode

������ ������ �������������� �������:
\code
class ZoomInPlugin : public QObject,
                     public XGisBasePlugin,
                     public WsViewBasePlugin,
                     public WsViewModePlugin
{
    Q_OBJECT
    Q_INTERFACES(XGisBasePlugin);
    Q_INTERFACES(WsViewBasePlugin);
    Q_INTERFACES(WsViewModePlugin);
    public:
        ZoomInPlugin(){qDebug("created zoomInPlugin");}
        ~ZoomInPlugin(){qDebug("destroyed zoomInPlugin");}

        QString getName() const {return QString(tr("��������� �������"));}
        QString getDesc() const {return QString(tr("��������� �������"));}
        QPixmap getPixmap() const {return QPixmap(Zoomin);}
  
        void setAction(QAction * _action);
        Mode * getMode();
        void addWsView( WsView * _view );


    public slots:
        void leaveMode(Mode * mode);
        void enterMode(Mode * mode);
        void failedEnterMode(Mode * mode);
        void resumeListen(WsView * view, Mode * mode);
        void suspendListen(WsView * view, Mode * mode);
        void setActive( bool on );

        void mousePress(QMouseEvent * e);
    
    protected:
        void checkAction(QObject * plugin);
        void uncheckAction(QObject * plugin);
};
\endcode

���������� ������ �������:
\code

#include <QtGui>

#include <mapitem.h>
#include <workspace.h>
#include <mapobject.h>
#include <mode.h>
#include <wsview.h>
#include <wscontainer.h>

#include "zoominplugin.h"

void ZoomInPlugin::leaveMode(Mode * mode)
{
  WsView * view = getActiveWsView();
  if(!mode || !view || mode->getNumber() != this->mode->getNumber()) return;
  
  disconnect(view, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(mousePress(QMouseEvent *)));
  view->setCurrentMode(NULL);
  uncheckAction(this);
}

void ZoomInPlugin::enterMode(Mode * mode)
{
  WsView * view = getActiveWsView();
  if(!mode || !view || mode->getNumber() != this->mode->getNumber()) return;

  connect(view, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(mousePress(QMouseEvent *)));
  view->setCurrentMode(mode);
}

void ZoomInPlugin::resumeListen( WsView * view, Mode * mode )
{  
  if(!mode || !view || mode->getNumber() != this->mode->getNumber()) return;
 
  connect(view, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(mousePress(QMouseEvent *)));
  checkAction(this);
}

void ZoomInPlugin::suspendListen( WsView * view, Mode * mode )
{
  if(!mode || !view || mode->getNumber() != this->mode->getNumber()) return;

  disconnect(view, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(mousePress(QMouseEvent *)));
  uncheckAction(this);
}

void ZoomInPlugin::setAction( QAction * _action )
{
    if ( action )
        disconnect( action, SIGNAL( toggled(bool) ), this, SLOT( setActive(bool) ) );
    action = _action;
    if( action )
    {
        action->setCheckable( TRUE );
        connect( action, SIGNAL( toggled(bool) ), this, SLOT( setActive(bool) ) );
    }
}

void ZoomInPlugin::setActive( bool on )
{
    if(!wsContainer) return;
    wsContainer->setMode(on ? mode : NULL);
}

Mode * ZoomInPlugin::getMode( )
{
    if ( ! mode )
    {
        mode = new Mode();
        mode->setName( getName() );
        mode->setDesc( getDesc() );
    }

    return mode;
}

void ZoomInPlugin::addWsView( WsView * _view )
{
    if(!_view) return;
    connect(_view, SIGNAL(leaveMode(Mode*)), this, SLOT(leaveMode(Mode*)));
    connect(_view, SIGNAL(enterMode(Mode*)), this, SLOT(enterMode(Mode*)));
    connect(_view, SIGNAL(failedEnterMode(Mode*)), this, SLOT(failedEnterMode(Mode*)));
    connect(_view, SIGNAL(resumeListen(WsView *, Mode*)), this, SLOT(resumeListen(WsView *, Mode*)));
    connect(_view, SIGNAL(suspendListen(WsView *, Mode*)), this, SLOT(suspendListen(WsView *, Mode*)));
}

void ZoomInPlugin::failedEnterMode( Mode * _mode )
{
    if ( _mode && mode && mode->getNumber() == _mode->getNumber() )
        uncheckAction(this);
}


void ZoomInPlugin :: checkAction(QObject * plugin)
{
    if(!action) return;
    if(action->isCheckable() && action->isChecked() == false){
        QObject::disconnect(action, SIGNAL(toggled(bool)), plugin, SLOT(setActive(bool)));
        action->setChecked(true);
        QObject::connect(action, SIGNAL(toggled(bool)), plugin, SLOT(setActive(bool)));
    }
}

void ZoomInPlugin :: uncheckAction(QObject * plugin)
{
    if(!action) return;
    if(action->isCheckable() && action->isChecked() == true){
        QObject::disconnect(action, SIGNAL(toggled(bool)), plugin, SLOT(setActive(bool)));
        action->setChecked(false);
        QObject::connect(action, SIGNAL(toggled(bool)), plugin, SLOT(setActive(bool)));
    }
}


//==================================================================
//==========================������ �������==========================
//==================================================================

void ZoomInPlugin::mousePress(QMouseEvent * e)
{
    WsView * view = getActiveWsView();
    if(!view) return;

    if (e->button() != Qt::LeftButton)  return;
    QPoint p = e->pos();

    view->setNewCenter(p);
    view->incScale();
    view->showMap();
}

Q_EXPORT_PLUGIN(ZoomInPlugin)
\endcode
*/
