#ifndef KKSBASEPLUGIN_H
#define KKSBASEPLUGIN_H

#include <QPixmap>
#include <QString>
#include <QAction>
#include <QTextEdit>

#include <kkspluginmanager_config.h>

class KKSCoreApplication;

/*!\ingroup PluginGroup
\brief Базовый абстрактный класс (интерфейс) для всех плагинов, создаваемых для библиотеки DynamicDocs.

Все плагины должны быть порождены напрямую или ненапрямую от данного класса.*/
class _KKSPLUGIN_EXPORT KKSBasePlugin
{
    public:
        KKSBasePlugin() : action(0) {}
        virtual ~KKSBasePlugin(){}

        virtual QString getName() const = 0; //!<Возвращает название плагина. Необходимо переопределить данный метод в дочерних классах
        virtual QString getDesc() const = 0;//!<Возвращает описание плагина. Необходимо переопределить данный метод в дочерних классах
        virtual QPixmap getPixmap() const = 0;//!<Возвращает иконку, используемую для отображения соответствующей кнопки в меню или на панели инструментов. Необходимо переопределить данный метод в дочерних классах.

        /*!\brief Метод устанавливает объект класса QAction, соответствующий данному плагину.
         
          Предполагается, что каждому плагину соответствует объект QAction
          (кнопка на панели инструментов и/или меню). Для поддержки данного механизма
          каждый плагин должен реализовывать виртуальные методы setAction() и getPixmap() (также можно переопределить метод getAction()).
          При этом сам плагин не должен создавать объект QAction.
          Последний создается в методе initPlugin() класса PluginLoader, который
          выполняет загрузку всех доступных плагинов.
         
          Необходимо переопределить данный метод в дочерних классах таким образом, чтобы в нем вызывался метод QObject::connect(action, SIGNAL(activated()), this, SLOT(theMethod())), где theMethod() - метод, который должен выполняться при нажатии на кнопку.
          
          Может быть реализован следующим образом:
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

        /*!\brief Метод возвращает объект QAction, соответствующий данному плагину.
         
         */
        virtual QAction * getAction() { return action; }

        /*!\brief Метод возвращает объект QWidget для данного плагина, являющийся панелью инструментов. Если дочерний класс (разрабатываемый плагин) имеет панель инструментов, то необходимо переопределить данный метод в этом классе. */
        virtual QWidget * getToolWindow ( void ) { return 0; }

        virtual void setLogWidget(QTextEdit * logWidget) {Q_UNUSED(logWidget);}
        virtual QTextEdit * logWidget() {return NULL;}

        /*!\brief Метод выполняет активацию плагина. 
        
        Может быть переопределен в каждом дочернем классе (плагине). Действие данного метода заключается в том, что он программно дублирует активацию плагина, которая производится при срабатывании сигнала triggered() визуального элемента управления данным плагином (см. описание метода setAction()).
        
        В базовой реализации данный метод просто вызывает QAction::trigger().
        
        Поскольку плагин может предоставлять форму для взаимодействия с пользователем, то в качестве параметра данный метод принимает указатель на родительское окно. Рекомендуется использовать его в качестве параметра при создании форм такого плагина.

        \param parent Указатель на родительское окно*/
        virtual int activate(QWidget * parent = NULL) {Q_UNUSED(parent); if(action){ action->trigger(); return 1;} return 0;}

        virtual void setFactoryManager(KKSCoreApplication * fManager) {Q_UNUSED(fManager);}

    protected:
        QAction * action;
};

Q_DECLARE_INTERFACE(KKSBasePlugin, "org.dynamicsoft.dynamicdocs.KKSBasePlugin/1.0");

#endif

/*!\defgroup PluginGroup Классы, предназначенные для создания расширений (плагинов) для библиотеки DynamicDocs

DynamicDocs реализует механизм, позволяющий разработчикам, использующим ее, создавать для нее расширения (плагины). 
При написании плагинов следует в первую очередь руководствоваться требованиями и рекомендациями библиотеки Qt версии 4.x. 
Кроме того, плагины должны удовлетворять требованиям, предъявляемым к ним библиотекой DynamicDocs.

Все плагины для DynamicDocs должны быть порождены от базового виртуального класса KKSBasePlugin и переопределять все его виртуальные методы. 
Иных базовых требований со стороны DynamicDocs не предъявляется. При этом существует ряд дополнительных требований, предъявляемых различными подсистемами библиотеки. 

При этом необходимо подчеркнуть, что если плагин порожден не от KKSBasePlugin
то он ДОЛЖЕН быть также порожден от KKSBasePlugin. В противном случае плагин не загрузится библиотекой DynamicDocs. Пример:
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

Предполагается, что каждый плагин должн иметь указатель на соответствующий объект класса QAction. 
Для поддержки данного механизма каждый плагин должен реализовывать виртуальные методы KKSBasePlugin::setAction() и KKSBasePlugin::getPixmap(). 
При этом сам плагин не должен создавать объект QAction. Последний создается в методе KKSPluginLoader::initPlugin(), который выполняет загрузку всех доступных плагинов и некоторые манипуляции с ними.

Более подробно процесс создания плагинов описан в документе \link Plugins Система расширений (плагинов) библиотеки DynamicDocs\endlink.
*/

/*!\page Plugins Система расширений (плагинов) библиотеки DynamicDocs

DynamicDocs реализует механизм, позволяющий разработчикам, использующим ее, создавать для нее расширения (плагины). 
При написании плагинов следует в первую очередь руководствоваться требованиями и рекомендациями библиотеки Qt версии 4.x. 
Кроме того, плагины должны удовлетворять требованиям, предъявляемым к ним библиотекой DynamicDocs.

Все плагины для библиотеки DynamicDocs должны быть порождены от базового виртуального класса KKSBasePlugin и переопределять все его виртуальные методы. 
Иных базовых требований со стороны DynamicDocs не предъявляется. При этом существует ряд дополнительных требований, предъявляемых различными подсистемами библиотеки DynamicDocs. 

При этом необходимо подчеркнуть, что если плагин порожден не от KKSBasePlugin
то он ДОЛЖЕН быть также порожден от KKSBasePlugin. В противном случае плагин не загрузится библиотекой DynamicDocs. Пример:
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

Предполагается, что каждый плагин должн иметь указатель на соответствующий объект класса QAction. Для поддержки данного механизма каждый плагин 
должен реализовывать виртуальные методы KKSBasePlugin::setAction() и KKSBasePlugin::getPixmap(). При этом сам плагин не должен создавать объект QAction. 
Последний создается в методе KKSPluginLoader::initPlugin(), который выполняет загрузку всех доступных плагинов и некоторые манипуляции с ними.

Объявление базового класса (интерфейса) для всех плагинов DynamicDocs:
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

При реализации плагина необходимо выполнить следующие шаги:
   \li Объявить класс плагина и унаследовать его от QObject и от абстрактных классов, которые данный плагин должен реализовывать. Необходимо использовать также макрос Q_OBJECT.
   \li Для того, чтобы дать знать системе Qt об используемых данным плагином интерфейсах используйте макрос Q_INTERFACES().
   \li Экспортировать плагин при помощи макроса Q_EXPORT_PLUGIN(). Данный макрос должен в коде встречаться только один раз. Поэтому самым лучшим местом является самый конец файла *.cpp.
   \li Скомпилировать и собрать плагин как динамическую разделяемую библиотеку, используя соответствующий *.pro файл.

Пример файла *.pro:
\code
TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += ../../src #путь к заголовочным файлам libxgis
win32:LIBS += -L../../build -lxgis #путь к библиотеке libxgis
HEADERS += zoominplugin.h Zoomin.xpm
SOURCES += zoominplugin.cpp
\endcode

\section BuildKey Ключ сборки (Build Key) плагинов

При загрузке плагинов система Qt проверяет т.н. ключ сборки каждого из них. Имеется ввиду следующее. Параметры сборки каждого плагина должен иметь ту же конфигурацию, что и загружаемое их приложение. Плагины, у которых параметры сборки отличаются от соответствующих параметров приложения, загружены не будут.

Небольшой пример. Допустим, приложение собирается с использованием инструмента qmake. Соответственно, оно будет иметь файл проекта *.pro, пример которого приведен ниже:
\code
....
CONFIG  += debug
....
\endcode

Т.е. приложение собирается в режиме отладки.

Допустим, разрабатывается также плагин, который должен загружаться вышеописанным приложением. Пусть он также собирается с использованием инструмента qmake. Соответственно, он будет иметь файл проекта, пример которого приведен ниже:
\code
....
CONFIG  += plugin release
....
\endcode

Т.е. плагин собирается в режиме релиза.

В такой ситуации попытка загрузки плагина приложением окажется неудачной, поскольку приложение и плагин имеют разные параметры сборки (приложение собирается в отладочном режиме, а плагин - в режиме релиза). Решением проблемы будет сборка и приложения, и плагина в одном и том же режиме, например в отладочном. Для приложения файл проекта должен выглядеть следующим образом:
\code
....
CONFIG  += debug
....
\endcode

Для плагина:
\code
....
CONFIG  += plugin debug
....
\endcode

Более подробно об этом написано в документации по библиотеке Qt http://doc.trolltech.com/4.0/plugins-howto.html#the-build-key .

\section WsViewPlugins Плагины для подсистемы отображения объектов карты

Для взаимодействия с пользователем в подсистеме отображения и визуального редактирования объектов карты предназначен класс WsView. По сути, плагины для данной подсистемы взаимодействуют именно с WsView. Это взаимодействие может осуществляться в двух режимах: в интерактивном и в неинтерактивном.

\subsection WsViewBasePlugins Плагины, взаимодействующие с подсистемой отображения объектов в неинтерактивном режиме

Такие плагины предназначены для выполнения какой-либо отдельной операции (действия), т.е. плагин определяет активный в данный момент экземпляр класса WsView (возвращается методом WsContainer::getActiveWorkspace()), выполняет действие и завершает работу. Примером такого плагина может служить создание градусной сетки.

Подобные плагины должны быть порождены от классов XGisBasePlugin и WsViewBasePlugin. Последний имеет два основных метода: WsViewBasePlugin::getActiveWsView() и WsViewBasePlugin::setWsContainer().

\subsection WsViewModePlugins Плагины, взаимодействующие с подсистемой отображения объектов в интерактивном режиме

Такие плагины обрабатывают действия пользователя при его взаимодействии с WsView. Данный класс генерирует сигналы при возникновении следующих событий:
\li Пользователь выполнил щелчек мышью (mousePress);
\li Пользователь отпустил кнопку мыши (mouseRelease);
\li Пользователь выполнил движение мыши (mouseMove);
\li Пользователь выполнил двойной щелчек мышью (mouseDoubleClick);
\li Пользователь изменил размеры видимой области окна карты (viewportResize);
\li Выполнена операция вызова контекстного меню (mouseContextMenu).

Все интерактивные плагины, взаимодействующие с WsView, должны быть унаследованы от классов XGisBasePlugin, WsViewBasePlugin и WsViewModePlugin. Здесь необходимо отметить, что напрямую объект класса WsView создать невозможно. За создание и управление экземплярами WsView отвечает класс WsContainer. Объект WsView в каждый момент времени находится в определенном режиме функционирования. Одновременно находиться в нескольких режимах невозможно. Режим функционирования описывается классом Mode. Каждый из загруженных интерактивных плагинов регистрирует в системе свой отдельный режим (см. метод ModeList::registerMode()). Все режимы сведены в список режимов, который описывается классом ModeList. Класс PluginLoader хранит список режимов, в которых могут находиться созданные с помощью WsContainer экземпляры WsView. Этот список определяется в момент загрузки приложения, т.е. в момент вызова метода PluginLoader::getPluginActions().

Каждый интерактивный плагин для WsView должен создавать свой экземпляр класса Mode, т.е. свой режим функционирования. В дальнейшем при загрузке данного плагина его режим будет зарегистрирован в ModeList данного PluginLoader и получит уникальный номер. В дальнейшем обрабатывать приведенные выше сигналы будет только тот плагин, в режиме которого находится сейчас активный WsView.

Для функционирования этого механизма необходимо, чтобы каждый плагин реализовывал следующие виртуальные методы интерфейса WsViewModePlugin:
- getMode();
- leaveMode();
- enterMode();
- failedEnterMode();
- setActive();
- addWsView();
- suspendListen();
- resumeListen().

Назначение каждого из перечисленных выше методов описано в соответствующих разделах документации.

При загрузке приложения создается ровно один экземпляр каждого плагина, вне зависимости от количества созданных WsView. Иными словами, один плагин отвечает за работу нескольких WsView. Для того, чтобы понять, какой из экземпляров WsView в данный момент является активным, предназначен метод WsViewBasePlugin::getActiveWsView(). Далее, при обработке сигналов WsView внутри плагина необходимо проверять, какой из WsView является активным, т.е. вызывать метод WsViewBasePlugin::getActiveWsView().
 
Для активизации плагина предназначен следующий механизм.

Плагин должен реализовывать метод setAction(), в котором должно быть вызвано следующее:
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
Вместо сигнала toggled() может использоваться любой другой допустимый сигнал класса QAction. Данный код ставит соответствие между сигналом класса QAction и слотом setActive() данного плагина. Соответственно должен быть реализован метод setActive(), в котором должен вызываться метод setMode() класса WsView. Метод setActive() может быть реализован, например, следующим образом:
\code
void ThePlugin::setActive( bool on )
{
    if(!wsContainer) return;
    wsContainer->setMode(on ? mode : NULL);
}
\endcode
В данной реализации при нажатии на кнопку (т.е. при переводе ее в состояние isChecked() == TRUE) вызывается метод WsContainer::setMode(), в качестве параметра последний принимает указатель на режим данного плагина. В противном случае в качестве параметра setMode() принимает NULL, указывающий на то, что из режима данного плагина необходимо выйти. При этом из текущего режима выходит только активный экземпляр WsView.

Далее в методе WsContainer::setMode() проверяется, какой из экземпляров WsView является активным. Для него вызывается метод WsView::setMode(). В этом методе генерируется сигнал leaveMode, который сопряжен со слотом leaveMode() плагина. Если в данный момент времени выйти из режима работы данного плагина возможно, то слот вызывает метод WsView::setCurrentMode(NULL). В противном случае он ничего не делает. После этого метод WsView::setMode() в случае, если выйти из текущего режима удалось (т.е. он равен NULL), генерирует сигнал enterMode, который сопряжен со слотом enterMode() плагина. Данный слот выполняет действия, необходимые для того, чтобы плагин начал работать и принимать сигналы WsView. Например, он может выглядеть так:
\code
void ThePlugin::enterMode(Mode * mode)
{
  WsView * view = getActiveWsView();
  if(!mode || !view || mode->getNumber() != this->mode->getNumber()) return;

  connect(view, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(mousePress(QMouseEvent *)));
  view->setCurrentMode(mode);
}
\endcode

Для поддержки возможности работы с несколькими WsView необходимо реализовать методы resumeListen() и suspendListen(). В которых выполнить действия, необходимые для возобновления и приостановки работы плагина соответственно (т.е. connect или disconnect для всех обрабатываемых сигналов соответственно). Также необходимо реализовать защищенные методы save() и restore(), которые сохраняют состояние внутренних переменных плагина и соответственно их восстанавливают. Внутренние переменные плагина хранятся как \code QHash<WsView*, QVariant> stack;\endcode. Т.е. каждая переменная хранится как QVariant.
Примерами этих методов являются следующие:
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

На этом все вспомогательные действия, необходимые для функционирования плагина, реализованы. Остается лишь реализовать логику работы плагина. Пример простейшего плагина приведен ниже. Данный плагин выполняет операцию сдвига карты.

Объявление базового класса (интерфейса) для всех плагинов подсистемы отображения и визуального редактирования:
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

Пример класса интерактивного плагина:
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

        QString getName() const {return QString(tr("Увеличить масштаб"));}
        QString getDesc() const {return QString(tr("Увеличить масштаб"));}
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

Реализация класса плагина:
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
//==========================ЛОГИКА ПЛАГИНА==========================
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
