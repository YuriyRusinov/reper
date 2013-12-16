#include "map_window.h"
#include "kksgiswidget.h"

MapWindow::MapWindow(QWidget* parent, Qt::WFlags fl)
    : QMainWindow(parent,fl)
{
    //init's
    setupUi(this);

    mpKKSGISWidget = KKSGISWidget::initQGISWidget(parent, fl);
    if(!mpKKSGISWidget){
        QMessageBox::critical(0, "", "");
        return;
    }
    connect(mpKKSGISWidget, SIGNAL(SIGNALchangeWindowTitle()), this, SLOT(SLOTmpActionChangeTitle()));
    setWindowTitle(mpKKSGISWidget->windowTitle());

    setStatusBar(mpKKSGISWidget->statusBar());
    setWindowIcon(mpKKSGISWidget->windowIcon());

    setCentralWidget(mpKKSGISWidget);
    
    QDockWidget * mLegendDock = new QDockWidget( tr( "Слои" ), this );
    mLegendDock->setObjectName( "Legend" );
    mLegendDock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
    mLegendDock->setWidget( mpKKSGISWidget->mapLegendWidget() );
    addDockWidget( Qt::LeftDockWidgetArea, mLegendDock );

    QDockWidget * mLayerOrderDock = new QDockWidget( tr( "Порядок слоев" ), this );
    mLayerOrderDock->setObjectName( "LayerOrder" );
    mLayerOrderDock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
    mLayerOrderDock->setWidget( mpKKSGISWidget->mapLayerOrderWidget() );
    addDockWidget( Qt::LeftDockWidgetArea, mLayerOrderDock );
    mLayerOrderDock->hide();

    //QDockWidget * mpLeftDock = new QDockWidget("Legend", this);
    //addDockWidget(Qt::LeftDockWidgetArea, mpLeftDock, Qt::Horizontal);
    //mpLeftDock->setWidget(mpKKSGISWidget->tableLegend());




    // добавляем фрейм и вставляем в него виджет "Окно карты"(Map Canvas)
    //mpMapLayout = new QVBoxLayout(frameMap); //main Layout
    //mpMapLayout->addWidget(mpMapCanvas); // adding MapCanvas in Layout

    setMenuBar(mpKKSGISWidget->menuBar());
    
    QMap<QString, QMenu*> menus = mpKKSGISWidget->menuMap();
    QMenu * fileMenu = menus.value(tr("File"));
    if(fileMenu){
        //create actions
        mpActionFileExit = new QAction(QIcon(":/ico/mActionFileExit.png"), tr("Выход"), this);
        mpActionFileExit->setStatusTip(tr("Закрыть приложение"));
        mpActionFileExit->setShortcuts(QKeySequence::Close);
        connect(mpActionFileExit, SIGNAL(triggered()), this, SLOT(SLOTmpActionFileExit()));
        fileMenu->addSeparator();
        fileMenu->addAction(mpActionFileExit);
    }
    
    addToolBar(mpKKSGISWidget->toolBar());
    // add file exit menu!!!

    statusBar()->showMessage("ПК Репер готов к работе.");
}

MapWindow::~MapWindow()
{
  // deleteAll
}

void MapWindow::SLOTmpActionFileExit() // Exit from Application
{
    bool ProjectChange(true); // changing of Project NEEDED TO CONNECT
    QString pMessageText(tr("Точно выйти?"));
    uint pButtonsPack(0);
    int pButton(0);
 
    if (ProjectChange)
    {
        pButtonsPack = 0x00410800;

    }
    else
    {
        pButtonsPack = 0x00404000;

    }

    QMessageBox * pExitApp = new QMessageBox (QMessageBox::Question,
                                              tr("Выход из приложения"),
                                              pMessageText,
                                              (QMessageBox::StandardButton)pButtonsPack);
    pButton = pExitApp->exec();
    delete pExitApp;
    if (pButton == QMessageBox::Yes || pButton == QMessageBox::No)
    {
        this->close();
    }
    else if (pButton == QMessageBox::Save)
    {
        // save NEEDED TO CONNECT
        this->close();
        mpKKSGISWidget->saveProject();
    }
    
    mpKKSGISWidget->closeProject();
}

void MapWindow::SLOTmpActionChangeTitle()
{
    setWindowTitle(mpKKSGISWidget->windowTitle());
}

