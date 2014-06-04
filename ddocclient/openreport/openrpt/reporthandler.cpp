/*
 * OpenRPT report writer and rendering engine
 * Copyright (C) 2001-2008 by OpenMFG, LLC
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * Please contact info@openmfg.com with any questions on this license.
 */

#include "reporthandler.h"
#include "reportgridoptions.h"
#include "memdbloader.h"
#include "data.h"
#include "documentwindow.h"
#include "documentscene.h"
#include "graphicsitems.h"
#include "graphicssection.h"
#include "fontutils.h"

// dialogs
#include "dbfiledialog.h"
#include "editpreferences.h"

// common
#include "login.h"
#include "parsexmlutils.h"
#include "parameter.h"
#include "parameteredit.h"
#include "previewdialog.h"

// renderer
#include <openreports.h>
#include <renderobjects.h>
#include <orprerender.h>
#include <orprintrender.h>

#include "dbtools.h"

// qt
#include <QApplication>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QCursor>
#include <QEvent>
#include <QToolBar>
#include <QMenuBar>
#include <QRect>
#include <QMainWindow>
#include <QAction>
#include <QPixmap>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QSpinBox>
#include <QWorkspace>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QPrintDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QColorDialog>

extern bool __dosnap;

//int dpi  = 72; // assume a 72 dpi until we can get to the code that sets the proper value
int dpiX = 72;
int dpiY = 72;


QRect padRect(QRect r)
{
  return QRect(r.x() - 3, r.y() - 3, r.width() + 6, r.height() + 6);
}

class ReportWriterCopyData
{
  public:
    ReportWriterCopyData()
    {
      copy_item = 0;
    }

    // copy information
    QPointF copy_offset;

    int copy_item; // the item type that we have information on

    QString copy_str1;     // string 1 (label:text  field/text/barcode/image:queryname)
    QString copy_str2;     // string 2 (field/text/barcode/image:column)
    QString copy_str3;     // string 3 (barcode:format image:inlineImageData field:trackTotalFormat)
    QString copy_str4;     // string 4 (image:mode)

    QFont copy_font;       // font (label/field/text:font)
    QRectF copy_rect;       // rect (all but line)

    double copy_dbl;       // text bottom padding
    int    copy_int1;      // line:startx barcode:maxlength
    int    copy_int2;      // line:starty barcode:alignment
    int    copy_int3;      // line:endx
    int    copy_int4;      // line:endy
    int    copy_int5;      // line:weight

    QPointF copy_line_start;
    QPointF copy_line_end;

    bool   copy_bool1;     // image:isInline field:trackTotal
    bool   copy_bool2;     // field: trackBuiltinFormat
    bool   copy_bool3;     // field: useSubTotal

    QBrush copy_brush;     // all entities
    QPen   copy_pen;       // all entities

    ORGraphData copy_graph; // graph
};

//
// define and implement the ReportWriterSectionData class
// a simple class to hold/hide data in the ReportHandler class
//
class ReportWriterSectionData
{
  public:
    ReportWriterSectionData()
    {
      mouseAction = ReportWriterSectionData::MA_None;
      insertItem = NoItem;
    }
    virtual ~ReportWriterSectionData()
    {
    }

    enum ItemType
    {
      NoItem = 0,
      LabelItem,
      FieldItem,
      TextItem,
      LineItem,
      BarcodeItem,
      ImageItem,
      GraphItem,
      RectItem,
      CrossTabItem
    };
    enum MouseAction
    {
      MA_None = 0,
      MA_Insert = 1,
      MA_Grab = 2,
      MA_MoveStartPoint,
      MA_MoveEndPoint,
      MA_ResizeNW = 8,
      MA_ResizeN,
      MA_ResizeNE,
      MA_ResizeE,
      MA_ResizeSE,
      MA_ResizeS,
      MA_ResizeSW,
      MA_ResizeW
    };

    int selected_x_offset;
    int selected_y_offset;

    MouseAction mouseAction;
    ItemType insertItem;

    // copy data
    QPointF copy_pos;      // the base position of the copy (typically the first items original pos)
    QList<ReportWriterCopyData> copy_list;
};


//
// Constructor(s)
//
ReportHandler::ReportHandler(QObject * parent, const char * name)
  : QObject(parent)
{
  if(name)
    setObjectName(name);
  // initialize data
  _parentWindow = 0;
  _parentWindowIsWorkspace = false;
  _noDatabase = FALSE;
  _allowDBConnect = TRUE;
  _placeMenusOnWindows = FALSE;
  _placeToolbarsOnWindows = FALSE;

  _strMenuConnect = QObject::tr("Connect to Database");
  _strMenuDisconnect = QObject::tr("Disconnect from Database");


//  Q_INIT_RESOURCE(OpenRPTWrtembed);

  gridOptions = new ReportGridOptions(qApp->desktop()->logicalDpiX(),
                                      qApp->desktop()->logicalDpiY(),
                                      this, "grid options");
  sectionData = new ReportWriterSectionData();

  grpDoc = new QActionGroup(this);
  grpDoc->setExclusive(false);
  fileNewAction = new QAction( QIcon(getIconName(":/OpenRPT/icons/icons_size/document_size.png")),
                              tr("&New File"),this);
  fileNewAction->setShortcut(QKeySequence::New);
  fileOpenAction = new QAction( QIcon(getIconName(":/OpenRPT/icons/icons_size/files_zoom_size.png")),
                               tr("&Open File..."),this);
  fileOpenAction->setShortcut(QKeySequence::Open);
  fileSaveAction = new QAction( QIcon(getIconName(":/OpenRPT/icons/icons_size/diskette_size.png")),
                               tr("&Save File"),grpDoc);
  fileSaveAction->setShortcut(QKeySequence::Save);
  fileSaveAsAction = new QAction( tr("Save &As..."),grpDoc);
  fileCloseAction = new QAction( tr("&Close"),grpDoc);
  fileCloseAction->setShortcut(QKeySequence::Close);
  filePreviewAction = new QAction( QIcon(getIconName(QString(":/OpenRPT/icons/icons_size/diskette_size.png"))),
                                   tr("Print Preview..."), grpDoc);
  filePrintAction = new QAction(tr("Print..."), grpDoc);
  filePrintAction->setShortcut(QKeySequence::Print);
  filePrintToPDFAction = new QAction(tr("Print to PDF..."), grpDoc);
  fileExitAction = new QAction( tr("E&xit"),this);
  fileExitAction->setShortcut(Qt::ALT+Qt::Key_F4);

  grpEdit = new QActionGroup(this);
  grpEdit->setExclusive(false);
  editCutAction = new QAction( QIcon(getIconName(":/OpenRPT/icons/icons_size/scissors_size.png")),
                              tr("Cu&t"), grpEdit);
  editCutAction->setShortcut(QKeySequence::Cut);
  editCopyAction = new QAction( QIcon(getIconName(":/OpenRPT/icons/icons_size/copy_size.png")),
                               tr("&Copy"), grpEdit);
  editCopyAction->setShortcut(QKeySequence::Copy);
  editPasteAction = new QAction( QIcon(getIconName(":/OpenRPT/icons/icons_size/paste_size.png")),
                                tr("&Paste"), grpEdit);
  editPasteAction->setShortcut(QKeySequence::Paste);
  editDeleteAction = new QAction( QIcon(getIconName(":/OpenRPT/icons/icons_size/delete_size.png")),
                                 tr("&Delete"), grpEdit);
  editDeleteAction->setShortcut(QKeySequence::Delete);
  editPreferencesAction = new QAction( QIcon(getIconName(":/OpenRPT/icons/icons_size/font_size.png")),
                                      tr("Preferences"), this);
  editSelectAllAction = new QAction( tr("Select All"), this);
  editPropertiesAction = new QAction( tr("Properties"), this);

  grpGrid = new QActionGroup(this);
  grpGrid->setExclusive(false);
  gridShowAction = new QAction( QIcon(getIconName(":/OpenRPT/icons/icons_size/numeric_field_size.png")),
                               tr("Show Grid"), grpGrid);
  gridShowAction->setCheckable(true);
  gridShowAction->setChecked(gridOptions->isVisible());

  gridSnapAction = new QAction( QIcon(getIconName(":/OpenRPT/icons/icons_size/numeric_field_ok_size.png")),
                               tr("Snap to Grid"), grpGrid);
  gridSnapAction->setCheckable(true);
  gridSnapAction->setChecked(gridOptions->isSnap());

  grpItem = new QActionGroup(this);
  grpItem->setExclusive(true);
  itemLabelAction = new QAction( QIcon(getIconName(":/OpenRPT/icons/icons_size/attribute_size.png")),
                                tr("Insert Label"), grpItem);
  itemFieldAction = new QAction( QIcon(getIconName(":/OpenRPT/icons/icons_size/field_size.png")),
                                tr("Insert Field"), grpItem);
  itemTextAction = new QAction( QIcon(getIconName(":/OpenRPT/icons/icons_size/text_field_size.png")),
                               tr("Insert Text"), grpItem);
  itemLineAction = new QAction( QIcon(getIconName(":/OpenRPT/icons/icons_size/line_size.png")),
                               tr("Insert Line"), grpItem);
  itemRectAction = new QAction( QIcon(getIconName(":/OpenRPT/icons/icons_size/box_size.png")),
                                tr("Insert Rectangle"), grpItem);
  itemCrossTabAction = new QAction( QIcon(getIconName(QString(":/OpenRPT/icons/icons_size/crosstab_size.png"))),
                                    tr("Insert CrossTab"), grpItem);
  itemBarcodeAction = new QAction( QIcon(getIconName(":/OpenRPT/icons/icons_size/barcode_size.png")),
                                  tr("Insert Bar Code"), grpItem);
  itemImageAction = new QAction( QIcon(getIconName(":/OpenRPT/icons/icons_size/image_field_size.png")),
                                tr("Insert Image"), grpItem);
  itemGraphAction = new QAction( QIcon(getIconName(":/OpenRPT/icons/icons_size/statistic_size.png")),
                                tr("Insert Chart/Graph"), grpItem);

  foreach(QAction *action, grpItem->actions())
      action->setCheckable(true);

  dbConnectAction = new QAction( _strMenuConnect, this);
  dbLoadAction = new QAction( QIcon(getIconName(":/OpenRPT/icons/icons_size/export_database_size.png")),
                             tr("Load from Database"), this);
  dbSaveAction = new QAction( QIcon(getIconName(":/OpenRPT/icons/icons_size/database_add_size.png")),
                             tr("Save to Database"), this);

  docTitleAction           = new QAction(tr("Properties..."),         grpDoc);
  docPageSetupAction       = new QAction(tr("&Page Setup..."),        grpDoc);
  docQuerySourceListAction = new QAction(tr("Query &Sources..."),     grpDoc);
  docSectionEditorAction   = new QAction(tr("Section Editor..."),     grpDoc);
  docColorListAction       = new QAction(tr("Color Definitions..."),  grpDoc);
  docDefinedParamsAction   = new QAction(tr("Defined Parameters..."), grpDoc);

  grpAlign = new QActionGroup(this);
  alignTopAction = new QAction(tr("Align Top"), grpAlign);
  alignVCenterAction = new QAction(tr("Align V. Center"), grpAlign);
  alignBottomAction = new QAction(tr("Align Bottom"), grpAlign);
  alignLeftAction = new QAction(tr("Align Left"), grpAlign);
  alignHCenterAction = new QAction(tr("Align H. Center"), grpAlign);
  alignRightAction = new QAction(tr("Align Right"), grpAlign);

/*
  cbFont = new QFontComboBox();
  cbFont->clearEditText();

  cbFontSize = new QComboBox();
  cbFontSize->setEditable(true);
  QStringList sizesList;
  sizesList << "6" << "7" << "8" << "9" << "10" << "11" << "12" << "14" << "16" << "18" << "20" << "24" << "28" << "36" << "48" << "72";
  cbFontSize->addItems(sizesList);
  cbFontSize->clearEditText();

  btFontWeight = new QPushButton(tr("B"));
  btFontWeight->setFixedWidth(24);
  btFontWeight->setCheckable(true);
  QFont font = btFontWeight->font();
  font.setBold(true);
  btFontWeight->setFont(font);

  btFontStyle = new QPushButton(tr("i"));
  btFontStyle->setFixedWidth(24);
  btFontStyle->setCheckable(true);
  font = btFontStyle->font();
  font.setItalic(true);
  btFontStyle->setFont(font);
*/

  grpFont = new QActionGroup(this);
  grpFont->setExclusive(false);
  
  grpProperties = new QActionGroup(this);
  colorAction = new QAction(tr("Color"), grpProperties);
  fillAction = new QAction(tr("Fill"), grpProperties);
  rotationAction = new QAction(tr("Rotation"), grpProperties);


  // connect the actions to various slots so they functions properly
  connect(fileNewAction, SIGNAL(activated()), this, SLOT(fileNew()));
  connect(fileOpenAction, SIGNAL(activated()), this, SLOT(fileOpen()));
  connect(fileSaveAction, SIGNAL(activated()), this, SLOT(fileSave()));
  connect(fileSaveAsAction, SIGNAL(activated()), this, SLOT(fileSaveAs()));
  connect(fileCloseAction, SIGNAL(activated()), this, SLOT(fileClose()));
  connect(filePreviewAction, SIGNAL(activated()), this, SLOT(filePreview()));
  connect(filePrintAction, SIGNAL(activated()), this, SLOT(filePrint()));
  connect(filePrintToPDFAction, SIGNAL(activated()), this, SLOT(filePrintToPDF()));
  connect(editCutAction, SIGNAL(activated()), this, SLOT(editCut()));
  connect(editCopyAction, SIGNAL(activated()), this, SLOT(editCopy()));
  connect(editPasteAction, SIGNAL(activated()), this, SLOT(editPaste()));
  connect(editDeleteAction, SIGNAL(activated()), this, SLOT(editDelete()));
  connect(editSelectAllAction, SIGNAL(activated()), this, SLOT(editSelectAll()));
  connect(editPreferencesAction, SIGNAL(activated()), this, SLOT(editPreferences()));
  connect(editPropertiesAction, SIGNAL(activated()), this, SLOT(editProperties()));
  connect(gridShowAction, SIGNAL(toggled(bool)), gridOptions, SLOT(setVisible(bool)));
  connect(gridSnapAction, SIGNAL(toggled(bool)), gridOptions, SLOT(setSnap(bool)));
  connect(itemLabelAction, SIGNAL(activated()), this, SLOT(itemLabel()));
  connect(itemFieldAction, SIGNAL(activated()), this, SLOT(itemField()));
  connect(itemTextAction, SIGNAL(activated()), this, SLOT(itemText()));
  connect(itemLineAction, SIGNAL(activated()), this, SLOT(itemLine()));
  connect(itemRectAction, SIGNAL(activated()), this, SLOT(itemRect()));
  connect(itemCrossTabAction, SIGNAL(activated()), this, SLOT(itemCrossTab()));
  connect(itemBarcodeAction, SIGNAL(activated()), this, SLOT(itemBarcode()));
  connect(itemImageAction, SIGNAL(activated()), this, SLOT(itemImage()));
  connect(itemGraphAction, SIGNAL(activated()), this, SLOT(itemGraph()));
  connect(dbConnectAction, SIGNAL(activated()), this, SLOT(dbConnect()));
  connect(dbLoadAction, SIGNAL(activated()), this, SLOT(dbLoadDoc()));
  connect(dbSaveAction, SIGNAL(activated()), this, SLOT(dbSaveDoc()));
  connect(docTitleAction, SIGNAL(activated()), this, SLOT(docTitle()));
  connect(docPageSetupAction, SIGNAL(activated()), this, SLOT(docPageSetup()));
  connect(docQuerySourceListAction, SIGNAL(activated()), this, SLOT(docQuerySourceList()));
  connect(docSectionEditorAction, SIGNAL(activated()), this, SLOT(docSectionEditor()));
  connect(docColorListAction, SIGNAL(activated()), this, SLOT(docColorList()));
  connect(docDefinedParamsAction, SIGNAL(activated()), this, SLOT(docDefinedParams()));
  connect(alignTopAction, SIGNAL(activated()), this, SLOT(alignTop()));
  connect(alignVCenterAction, SIGNAL(activated()), this, SLOT(alignVCenter()));
  connect(alignBottomAction, SIGNAL(activated()), this, SLOT(alignBottom()));
  connect(alignLeftAction, SIGNAL(activated()), this, SLOT(alignLeft()));
  connect(alignHCenterAction, SIGNAL(activated()), this, SLOT(alignHCenter()));
  connect(alignRightAction, SIGNAL(activated()), this, SLOT(alignRight()));
  connect(colorAction, SIGNAL(activated()), this, SLOT(color()));
  connect(fillAction, SIGNAL(activated()), this, SLOT(fill()));
  connect(rotationAction, SIGNAL(activated()), this, SLOT(rotation()));
  
//Обработка параметров командной строки для подключения к БД
    QStringList arguments =qApp->arguments();
		bool haveDatabaseURL=false;
		bool haveUsername=false;
		bool havePasswd=false;
		QString databaseURL, username, passwd;

    for ( QStringList::Iterator it = arguments.begin(); it != arguments.end(); ++it ) {
      QString argument( *it );
		
      if (argument.startsWith("-databaseURL=", Qt::CaseInsensitive)) {
        haveDatabaseURL = TRUE;
        databaseURL    = argument.right(argument.length() - 13);
      }
      else if (argument.startsWith("-username=", Qt::CaseInsensitive))
      {
        haveUsername = TRUE;
        username     = argument.right(argument.length() - 10);
      }
      else if (argument.startsWith("-passwd=", Qt::CaseInsensitive))
      {
        havePasswd = TRUE;
        passwd     = argument.right(argument.length() - 8);
      }
    }
//		qDebug()<<"Url: "<<databaseURL;
//		qDebug()<<"User: "<<username;
//		qDebug()<<"passwd: "<<passwd;
      if ( (haveDatabaseURL) && (haveUsername) && (havePasswd) )
    {
        QSqlDatabase db = QSqlDatabase::database(QSqlDatabase::defaultConnection,FALSE);
			db = databaseFromURL( databaseURL );
      if (db.isValid())
      {
	      db.setUserName(username);
   	   db.setPassword(passwd);

	      if (db.open()){
		    emit dbOpenClosed();
	      
	      }
	    }
	  }
}

ReportHandler::~ReportHandler()
{
    if (sectionData != 0)
        delete sectionData;
}

//
// toolbar and menu creation methods
//
void ReportHandler::docToolBars(QMainWindow * mw, int /*edge*/, bool /*newLine*/)
{
  if(mw)
  {
    // create the toolbars
    QToolBar * tbFiles = mw->addToolBar(tr("File Operations"));
    tbFiles->setObjectName("tbFiles");
    tbFiles->setIconSize(QSize(32, 32));
    tbFiles->addAction(fileNewAction);
    tbFiles->addAction(fileOpenAction);
    tbFiles->addAction(fileSaveAction);

    if(!_noDatabase)
    {
      QToolBar * tbDatabase = mw->addToolBar(tr("Database Operations"));
      tbDatabase->setObjectName("tbDatabase");
      tbDatabase->setIconSize(QSize(32, 32));
      tbDatabase->addAction(dbLoadAction);
      tbDatabase->addAction(dbSaveAction);
    }

    QToolBar * tbEdit = mw->addToolBar(tr("Edit Operations"));
    tbEdit->setObjectName("tbEdit");
    tbEdit->setIconSize(QSize(32, 32));
    tbEdit->addAction(editCutAction);
    tbEdit->addAction(editCopyAction);
    tbEdit->addAction(editPasteAction);
    tbEdit->addAction(editDeleteAction);
  
    QToolBar * tbOptions = mw->addToolBar(tr("Layout Options"));
    tbOptions->setObjectName("tbOptions");
    tbOptions->setIconSize(QSize(32, 32));
    tbOptions->addAction(gridShowAction);
    tbOptions->addAction(gridSnapAction);
    tbOptions->addAction(editPreferencesAction);

    QToolBar * tbItems = mw->addToolBar(tr("Report Elements"));
    tbItems->setObjectName("tbItems");
    mw->addToolBar(Qt::RightToolBarArea, tbItems);
    tbItems->setIconSize(QSize(32, 32));
    tbItems->addAction(itemLabelAction);
    tbItems->addAction(itemFieldAction);
    tbItems->addAction(itemTextAction);
    tbItems->addAction(itemLineAction);
    tbItems->addAction(itemCrossTabAction);
    tbItems->addAction(itemRectAction);
    tbItems->addAction(itemBarcodeAction);
    tbItems->addAction(itemImageAction);
    tbItems->addAction(itemGraphAction);

    QFontComboBox * lcbFont = new QFontComboBox();
    lcbFont->setObjectName("font_family");
    lcbFont->clearEditText();

    QComboBox * lcbFontSize = new QComboBox();
    lcbFontSize->setObjectName("font_size");
    lcbFontSize->setEditable(true);
    QStringList sizesList;
    sizesList << "6" << "7" << "8" << "9" << "10" << "11" << "12" << "14" << "16" << "18" << "20" << "24" << "28" << "36" << "48" << "72";
    lcbFontSize->addItems(sizesList);
    lcbFontSize->clearEditText();
  
    QPushButton * lbtFontWeight = new QPushButton(tr("B"));
    lbtFontWeight->setObjectName("font_weight");
    lbtFontWeight->setFixedWidth(24);
    lbtFontWeight->setCheckable(true);
    QFont font = lbtFontWeight->font();
    font.setBold(true);
    lbtFontWeight->setFont(font);
  
    QPushButton * lbtFontStyle = new QPushButton(tr("i"));
    lbtFontStyle->setObjectName("font_style");
    lbtFontStyle->setFixedWidth(24);
    lbtFontStyle->setCheckable(true);
    font = lbtFontStyle->font();
    font.setItalic(true);
    lbtFontStyle->setFont(font);

    connect(lcbFont, SIGNAL(currentIndexChanged (const QString&)), this, SLOT(fontChange(const QString&)));
    connect(lcbFontSize, SIGNAL(currentIndexChanged (const QString&)), this, SLOT(fontSizeChange(const QString&)));
    connect(lbtFontStyle, SIGNAL(clicked (bool)), this, SLOT(fontStyleChange(bool)));
    connect(lbtFontWeight, SIGNAL(clicked (bool)), this, SLOT(fontWeightChange(bool)));

    QToolBar * tbFont = mw->addToolBar(tr("Font"));
    tbFont->setObjectName("tbFont");
    grpFont->addAction(tbFont->addWidget(lcbFont));
    grpFont->addAction(tbFont->addWidget(lcbFontSize));
    grpFont->addAction(tbFont->addWidget(lbtFontWeight));   
    grpFont->addAction(tbFont->addWidget(lbtFontStyle));       
  }
}

QAction * ReportHandler::populateMenuBar(QMenuBar * menubar, QAction * exitAction)
{
  QMenu * mFile = menubar->addMenu(tr("&File"));
  mFile->addAction(fileNewAction);
  mFile->addAction(fileOpenAction);
  mFile->addAction(fileSaveAction);
  mFile->addAction(fileSaveAsAction);
  mFile->addAction(fileCloseAction);

  mFile->addSeparator();
  mFile->addAction(filePreviewAction);
  mFile->addAction(filePrintAction);
  mFile->addAction(filePrintToPDFAction);

  if(exitAction)
  {
    mFile->addSeparator();
    mFile->addAction(exitAction);
  }

  if(!_noDatabase)
  {
    QMenu * mDb = menubar->addMenu(tr("Data&base"));
    connect(mDb, SIGNAL(aboutToShow()), this, SLOT(dbMenuAboutToShow()));
    mDb->addAction(dbLoadAction);
    mDb->addAction(dbSaveAction);
    if(_allowDBConnect)
    {
      mDb->addSeparator();
      mDb->addAction(dbConnectAction);
    }
  }

  QMenu * mEdit = menubar->addMenu(tr("&Edit"));
  mEdit->addAction(editCutAction);
  mEdit->addAction(editCopyAction);
  mEdit->addAction(editPasteAction);
  mEdit->addAction(editDeleteAction);
  mEdit->addSeparator();
  mEdit->addAction(editSelectAllAction);
  mEdit->addSeparator();
  mEdit->addAction(editPreferencesAction);

  QMenu * mInsert = menubar->addMenu(tr("&Insert"));
  mInsert->addAction(itemLabelAction);
  mInsert->addAction(itemFieldAction);
  mInsert->addAction(itemTextAction);
  mInsert->addAction(itemLineAction);
  mInsert->addAction(itemCrossTabAction);
  mInsert->addAction(itemRectAction);
  mInsert->addAction(itemBarcodeAction);
  mInsert->addAction(itemImageAction);
  mInsert->addAction(itemGraphAction);

  QMenu * mDoc = menubar->addMenu(tr("&Document"));
  mDoc->addAction(docTitleAction);
  mDoc->addAction(docPageSetupAction);
  mDoc->addAction(docQuerySourceListAction);
  mDoc->addAction(docSectionEditorAction);
  mDoc->addAction(docColorListAction);
  mDoc->addAction(docDefinedParamsAction);

  QAction * sid = menubar->addSeparator();


  /*QMenu * mHelp =*/ menubar->addMenu(tr("&Help"));

  return sid;
}

void ReportHandler::setParentWindow(QWidget * pw)
{
  _parentWindow = pw;
  _parentWindowIsWorkspace = (pw && (pw->metaObject()->className() == QString("QWorkspace")));
}


void ReportHandler::updateSelectedEntity()
{
  if(selectionCount() > 0)
  {
    QString message = QString::null;
    if(selectionCount() == 1)
    {
      DocumentWindow * gw = activeDocumentWindow();
      if(!gw)
        return;
      QGraphicsItem * item = gw->_scene->selectedItems().at(0);

      QRectF rect;
      QLineF line;
      double dx, dy, dw, dh;

      switch(item->type())
      {
        case ORGraphicsRectItem::Type:
        case ORGraphicsLabelItem::Type:
        case ORGraphicsFieldItem::Type:
        case ORGraphicsTextItem::Type:
        case ORGraphicsBarcodeItem::Type:
        case ORGraphicsImageItem::Type:
        case ORGraphicsGraphItem::Type:
        case ORGraphicsCrossTabItem::Type:
          rect = ((QGraphicsRectItem*)item)->rect();
          rect = QRectF(item->mapToParent(rect.topLeft()), rect.size());

          dx = (rect.x())/100.0;
          dy = (rect.y())/100.0;
          dw = (rect.width())/100.0;
          dh = (rect.height())/100.0;

          message = QString().sprintf(tr("Position [X: %s, Y: %s]    Size [W: %s, H: %s]").toLatin1().constData(),
                                               QString::number(dx,'g',3).toLatin1().constData(),
                                               QString::number(dy,'g',3).toLatin1().constData(),
                                               QString::number(dw,'g',3).toLatin1().constData(),
                                               QString::number(dh,'g',3).toLatin1().constData() );
          break;
        case ORGraphicsLineItem::Type:
          line = ((QGraphicsLineItem*)item)->line();
          line = QLineF(item->mapToParent(line.p1()), item->mapToParent(line.p2()));

          dx = ((line.p1().x()) / 100.0);
          dy = ((line.p1().y()) / 100.0);
          dw = ((line.p2().x()) / 100.0);
          dh = ((line.p2().y()) / 100.0);

          message = QString().sprintf(tr("Start Point [X: %s, Y: %s]    End Point [X: %s, Y: %s]").toLatin1().constData(),
                                               QString::number(dx,'g',3).toLatin1().constData(),
                                               QString::number(dy,'g',3).toLatin1().constData(),
                                               QString::number(dw,'g',3).toLatin1().constData(),
                                               QString::number(dh,'g',3).toLatin1().constData() );
          break;
        default:
          message = tr("Unknown Entity Type");
      }
    }
    else
      message = tr("Group Selection");
    emit messageChanged(message);
  }
  else
    emit messageCleared();
}

unsigned int ReportHandler::selectionCount()
{
  DocumentWindow * gw = activeDocumentWindow();
  if(gw)
    return gw->_scene->selectedItems().count();

  return 0;
}


//
// slot implementations
//
DocumentWindow* ReportHandler::fileNew()
{
  return fileNewTest();
}

DocumentWindow* ReportHandler::fileNewTest()
{
  DocumentWindow * view = new DocumentWindow(gridOptions, _parentWindow);
  connect(view->_scene, SIGNAL(selectionChanged()), this, SLOT(updateSelectedEntity()));

  if(_placeMenusOnWindows)
  {
    QMenuBar * mb = 0;
#ifdef Q_WS_MACX
    mb = new QMenuBar();
    view->setMenuBar(mb);
#else
    mb = view->menuBar();
#endif
    populateMenuBar(mb);
  }
  if(_placeToolbarsOnWindows)
    docToolBars(view);

  addDocumentWindow(view);
  view->showMaximized();
  return view;
}

void ReportHandler::fileOpen()
{
  // ok first we need to get a file to open.
  QString file = QFileDialog::getOpenFileName(
        0, tr("Open File"),
        QString::null, tr("XML (*.xml)") );
  fileOpen(file);
}

void ReportHandler::fileOpen(const QString &fileName)
{
  QString file = fileName;

  if(file.isNull())
    return; // user canceled

  // once we have a file lets go ahead and load it all up.
  QFile * f = new QFile(file);

  QDomDocument doc = QDomDocument();
  QString errMsg;
  int errLine, errCol;
  if(doc.setContent(f,&errMsg,&errLine,&errCol))
  {
    // ok lets read it
    DocumentWindow* rw = fileOpen(doc);
    if(rw)
      rw->_scene->filename = file;
  }
  else
  {
    // ERROR
    QMessageBox::critical(0, tr("Failed read on Open File"),
        QString().sprintf(tr("Encountered and error while parsing %s\n\n\t%s (Line %d Column %d)").toLatin1().constData(),file.toLatin1().data(),errMsg.toLatin1().data(),errLine,errCol));
  }
  delete f;
}


//
// File Actions
//
DocumentWindow * ReportHandler::fileOpen(const QDomDocument & doc, const QString & rptName, int rptGrade)
{
  DocumentWindow * rw = fileOpen(doc);
  if(rw) {
    // setup any variables to let this doc know how it was loaded
    // so when a save command is issued it knows what to do
    rw->_scene->lastSaveToDb = true;
    rw->_scene->dbRecordName = rptName;
    rw->_scene->dbRecordGrade = rptGrade;
  }
  return rw;
}

DocumentWindow * ReportHandler::fileOpen(const QDomDocument & doc)
{
    QDomElement root = doc.documentElement();
    if(root.tagName() != "report") {
        // arg we got an xml file but not one i know of
        qDebug("root element was not <report>");
        return 0;
    }
#define pageOptions pageOptions()
    DocumentWindow * dv = fileNew();
    DocumentScene * rw = dv->_scene;
    rw->removeSection(0,TRUE);

    QDomNodeList nlist = root.childNodes();
    QDomNode it;

    __dosnap = false;
    for(int i = 0; i < nlist.count(); i++ ) {
        it = nlist.item(i);
        // at this level all the children we get should be Elements
        if(it.isElement()) {
            QString n = it.nodeName();
            if(n == "title") {
                rw->setReportTitle(it.firstChild().nodeValue());
            } else if(n == "name") {
                rw->setReportName(it.firstChild().nodeValue());
            } else if(n == "description") {
                rw->setReportDescription(it.firstChild().nodeValue());
            } else if(n == "parameter") {
                ORParameter param;
                param.name = it.toElement().attribute("name");
                param.type = it.toElement().attribute("type");
                param.defaultValue = it.toElement().attribute("default");
                param.active = (it.toElement().attribute("active") == "true");
                param.listtype = it.toElement().attribute("listtype");
                if(param.listtype.isEmpty())
                  param.description = it.firstChild().nodeValue();
                else
                {
                  QDomNodeList section = it.childNodes();
                  for(int nodeCounter = 0; nodeCounter < section.count(); nodeCounter++)
                  {
                    QDomElement elemThis = section.item(nodeCounter).toElement();
                    if(elemThis.tagName() == "description")
                      param.description = elemThis.text();
                    else if(elemThis.tagName() == "query")
                      param.query = elemThis.text();
                    else if(elemThis.tagName() == "item")
                      param.values.append(qMakePair(elemThis.attribute("value"), elemThis.text()));
                    else
                      qDebug("While parsing parameter encountered an unknown element: %s",elemThis.tagName().toLatin1().constData());
                  }
                }
                rw->addDefinedParameter(param.name, param);
            } else if(n == "watermark") {
                ORWatermarkData wmData;
                parseReportWatermark(it.toElement(), wmData);
                rw->setWatermarkOpacity(wmData.opacity);
                rw->setWatermarkFont(wmData.font);
                rw->setWatermarkText(wmData.text);
                rw->setWatermarkUseDefaultFont(wmData.useDefaultFont);
                rw->setWatermarkUseStaticText(wmData.staticText);
                rw->setWatermarkQuery(wmData.data.query);
                rw->setWatermarkColumn(wmData.data.column);
            } else if(n == "background") {
                ORBackgroundData bgData;
                parseReportBackground(it.toElement(), bgData);
                rw->setBgEnabled(bgData.enabled);
                rw->setBgStatic(bgData.staticImage);
                rw->setBgImage(bgData.image);
                rw->setBgQuery(bgData.data.query);
                rw->setBgColumn(bgData.data.column);
                rw->setBgOpacity(bgData.opacity);
                rw->setBgResizeMode(bgData.mode);
                rw->setBgAlign(bgData.align);
                rw->setBgBoundsX(bgData.rect.x());
                rw->setBgBoundsY(bgData.rect.y());
                rw->setBgBoundsWidth(bgData.rect.width());
                rw->setBgBoundsHeight(bgData.rect.height());
            } else if(n == "size") {
                if(it.firstChild().isText()) {
                    rw->pageOptions->setPageSize(it.firstChild().nodeValue());
                } else {
                    //bad code! bad code!
                    // this code doesn't check the elemts and assums they are what
                    // they should be.
                    QDomNode n1 = it.firstChild();
                    QDomNode n2 = n1.nextSibling();
                    if(n1.nodeName() == "width") {
                        rw->pageOptions->setCustomWidth(n1.firstChild().nodeValue().toDouble() / 100.0);
                        rw->pageOptions->setCustomHeight(n2.firstChild().nodeValue().toDouble() / 100.0);
                    } else {
                        rw->pageOptions->setCustomWidth(n2.firstChild().nodeValue().toDouble() / 100.0);
                        rw->pageOptions->setCustomHeight(n1.firstChild().nodeValue().toDouble() / 100.0);
                    }
                    rw->pageOptions->setPageSize("Custom");
                }
            } else if(n == "labeltype") {
                rw->pageOptions->setLabelType(it.firstChild().nodeValue());
            } else if(n == "portrait") {
                rw->pageOptions->setPortrait(TRUE);
            } else if(n == "landscape") {
                rw->pageOptions->setPortrait(FALSE);
            } else if(n == "topmargin") {
                rw->pageOptions->setMarginTop(it.firstChild().nodeValue().toDouble() / 100.0);
            } else if(n == "bottommargin") {
                rw->pageOptions->setMarginBottom(it.firstChild().nodeValue().toDouble() / 100.0);
            } else if(n == "leftmargin") {
                rw->pageOptions->setMarginLeft(it.firstChild().nodeValue().toDouble() / 100.0);
            } else if(n == "rightmargin") {
                rw->pageOptions->setMarginRight(it.firstChild().nodeValue().toDouble() / 100.0);
            } else if(n == "querysource") {
                QDomNodeList qnl = it.childNodes();
                QDomElement qde = it.toElement();
                QString qname, qsql, qmgroup, qmname;
                bool lfdb = (qde.attribute("loadFromDb") == "true");
                QDomNode qit;
                for(int qi = 0; qi < qnl.count(); qi++) {
                    qit = qnl.item(qi);
                    if(qit.nodeName() == "name")
                        qname = qit.firstChild().nodeValue();
                    else if(qit.nodeName() == "sql")
                        qsql = qit.firstChild().nodeValue();
                    else if(qit.nodeName() == "mqlgroup")
                        qmgroup = qit.firstChild().nodeValue();
                    else if(qit.nodeName() == "mqlname")
                        qmname = qit.firstChild().nodeValue();
                    else
                        qDebug("While parsing quersource elements encountered unknown node.");
                }
                rw->qsList->add(new QuerySource(qname,qsql, lfdb, qmgroup, qmname));
            } else if(n == "colordef") {
                QDomNodeList qnl = it.childNodes();
                QString cname = QString::null;
                int red = 0, green = 0, blue = 0;
                QDomNode qit;
                for(int qi = 0; qi < qnl.count(); qi++) {
                    qit = qnl.item(qi);
                    if(qit.nodeName() == "name")
                        cname = qit.firstChild().nodeValue();
                    else if(qit.nodeName() == "red")
                        red = qit.firstChild().nodeValue().toInt();
                    else if(qit.nodeName() == "green")
                        green = qit.firstChild().nodeValue().toInt();
                    else if(qit.nodeName() == "blue")
                        blue = qit.firstChild().nodeValue().toInt();
                    if(cname.length() > 0) {
                        rw->addColorDef(cname, QColor(red, green, blue));
                    }
                }
            } else if(n == "rpthead") {
                if(rw->getReportHead() == 0) {
                    rw->insertReportHead();
                    rw->getReportHead()->initFromXML(it);
                } else {
                    qDebug("While loading xml tried to add more than one rpthead");
                }
            } else if(n == "rptfoot") {
                if(rw->getReportFoot() == 0) {
                    rw->insertReportFoot();
                    rw->getReportFoot()->initFromXML(it);
                } else {
                    qDebug("While loading xml tried to add more than one rpthead");
                }
            } else if(n == "pghead") {
                // we need to determine which page this is for
                // firstpage | odd | even | lastpage
                // or any if none was specified
                ORGraphicsSectionItem * rs = 0;
                if(!it.namedItem("firstpage").isNull()) {
                    if(rw->getPageHeadFirst() == 0) {
                        rw->insertPageHeadFirst();
                        rs = rw->getPageHeadFirst();
                    } else {
                        qDebug("tried to load more than one page head first");
                    }
                } else if(!it.namedItem("odd").isNull()) {
                    if(rw->getPageHeadOdd() == 0) {
                        rw->insertPageHeadOdd();
                        rs = rw->getPageHeadOdd();
                    } else {
                        qDebug("tried to load more than one page head odd");
                    }
                } else if(!it.namedItem("even").isNull()) {
                    if(rw->getPageHeadEven() == 0) {
                        rw->insertPageHeadEven();
                        rs = rw->getPageHeadEven();
                    } else {
                        qDebug("tried to load more than one page head even");
                    }
                } else if(!it.namedItem("lastpage").isNull()) {
                    if(rw->getPageHeadLast() == 0) {
                        rw->insertPageHeadLast();
                        rs = rw->getPageHeadLast();
                    } else {
                        qDebug("tried to load more than one page head last");
                    }
                } else {
                    // we have an any pghead
                    if(rw->getPageHeadAny() == 0) {
                        rw->insertPageHeadAny();
                        rs = rw->getPageHeadAny();
                    } else {
                        qDebug("tried to load more than one page head any");
                    }
                }
                if(rs) rs->initFromXML(it);
            } else if(n == "pgfoot") {
                // we need to determine which page this is for
                ORGraphicsSectionItem * rs = 0;
                if(!it.namedItem("firstpage").isNull()) {
                    if(rw->getPageFootFirst() == 0) {
                        rw->insertPageFootFirst();
                        rs = rw->getPageFootFirst();
                    } else {
                        qDebug("tried to load more than one page foot first");
                    }
                } else if(!it.namedItem("odd").isNull()) {
                    if(rw->getPageFootOdd() == 0) {
                        rw->insertPageFootOdd();
                        rs = rw->getPageFootOdd();
                    } else {
                        qDebug("tried to load more than one page foot odd");
                    }
                } else if(!it.namedItem("even").isNull()) {
                    if(rw->getPageFootEven() == 0) {
                        rw->insertPageFootEven();
                        rs = rw->getPageFootEven();
                    } else {
                        qDebug("tried to load more than one page foot even");
                    }
                } else if(!it.namedItem("lastpage").isNull()) {
                    if(rw->getPageFootLast() == 0) {
                        rw->insertPageFootLast();
                        rs = rw->getPageFootLast();
                    } else {
                        qDebug("tried to load more than one page foot last");
                    }
                } else {
                    // we have the any page foot
                    if(rw->getPageFootAny() == 0) {
                        rw->insertPageFootAny();
                        rs = rw->getPageFootAny();
                    } else {
                        qDebug("tried to load more than one page foot any");
                    }
                }
                if(rs) rs->initFromXML(it);
            } else if(n == "section") {
                // we need to load a section.
                ORGraphicsSectionDetail * rsd = new ORGraphicsSectionDetail(rw, rw);
                rsd->initFromXML(it);
                rw->insertSection(rw->detailSectionCount(),rsd);
            }
            else if(n == "database")
            {
                _databaseElt = it.toElement();
                MemDbLoader mdb;
				bool ok = mdb.load(_databaseElt, QSqlDatabase::database());
                if(!ok)
                    QMessageBox::warning(_parentWindow, rw->filename, mdb.lastError());
            } else {
                qDebug("Encountered an unknown Element: %s", n.toLatin1().data());
            }
        }
        else {
            qDebug("Encountered a child node of root that is not an Element");
        }
    }
    __dosnap = true;
    if(rw != 0) {
        rw->setModified(FALSE);
    }
#undef pageOptions
    return dv;
}

void ReportHandler::fileSave()
{
  DocumentWindow * gw = activeDocumentWindow();
  if(gw)
    gw->_scene->save(gw);
}

void ReportHandler::fileSaveAs()
{
  DocumentWindow * gw = activeDocumentWindow();
  if(gw)
    gw->_scene->saveAs(gw);
}

void ReportHandler::fileClose()
{
  DocumentWindow * gw = activeDocumentWindow();
  if(gw)
    gw->close();
}

//
// Edit Actions
//
void ReportHandler::editDelete()
{
    DocumentWindow * gw = activeDocumentWindow();
    if(gw)
    {
      gw->_scene->deleteSelected();
      return;
    }
}

void ReportHandler::editCut()
{
  if(selectionCount() > 0) {
    editCopy();
    editDelete();
  }
}

void ReportHandler::editCopy() {
  DocumentWindow * gw = activeDocumentWindow();
  if(gw)
  {
    QList<QGraphicsItem*> list = gw->_scene->selectedItems();
    QGraphicsItem * item = 0;
    if(list.count() > 0)
    {
      item = list.at(0);
      sectionData->copy_list.clear();
      if(item->type() == ORGraphicsLineItem::Type)
        sectionData->copy_pos = item->mapToScene(((ORGraphicsLineItem*)item)->line().p1());
      else
        sectionData->copy_pos = item->scenePos();
      ReportWriterCopyData cp;
      for(int i = 0; i < list.count(); i++)
      {
        item = list.at(i);
        int rtti = item->type();
        if(rtti == ORGraphicsLabelItem::Type)
        {
          ORGraphicsLabelItem * ent = (ORGraphicsLabelItem*)item;
          cp.copy_item = ReportWriterSectionData::LabelItem;
          cp.copy_str1 = ent->text();
          cp.copy_font = ent->font();
          cp.copy_pen  = ent->pen();
          cp.copy_brush = ent->brush();
          cp.copy_int1 = ent->textFlags();
          cp.copy_rect = QRectF(ent->mapToScene(ent->rect().topLeft()), ent->rect().size());
          cp.copy_offset = cp.copy_rect.topLeft() - sectionData->copy_pos;
          sectionData->copy_list.append(cp);
        }
        else if(rtti == ORGraphicsFieldItem::Type)
        {
          ORGraphicsFieldItem * ent = (ORGraphicsFieldItem*)item;
          cp.copy_item = ReportWriterSectionData::FieldItem;
          cp.copy_str1 = ent->query();
          cp.copy_str2 = ent->column();
          cp.copy_str3 = ent->format();
          cp.copy_font = ent->font();
          cp.copy_pen  = ent->pen();
          cp.copy_brush = ent->brush();
          cp.copy_int1 = ent->textFlags();
          cp.copy_rect = QRectF(ent->mapToScene(ent->rect().topLeft()), ent->rect().size());
          cp.copy_bool1 = ent->trackTotal();
          cp.copy_bool2 = ent->trackBuiltinFormat();
          cp.copy_bool3 = ent->useSubTotal();
          cp.copy_offset = cp.copy_rect.topLeft() - sectionData->copy_pos;
          sectionData->copy_list.append(cp);
        }
        else if(rtti == ORGraphicsTextItem::Type)
        {
          ORGraphicsTextItem * ent = (ORGraphicsTextItem*)item;
          cp.copy_item = ReportWriterSectionData::TextItem;
          cp.copy_str1 = ent->query();
          cp.copy_str2 = ent->column();
          cp.copy_font = ent->font();
          cp.copy_pen  = ent->pen();
          cp.copy_brush = ent->brush();
          cp.copy_int1 = ent->textFlags();
          cp.copy_rect = QRectF(ent->mapToScene(ent->rect().topLeft()), ent->rect().size());
          cp.copy_dbl = ent->bottomPadding();
          cp.copy_offset = cp.copy_rect.topLeft() - sectionData->copy_pos;
          sectionData->copy_list.append(cp);
        }
        else if(rtti == ORGraphicsLineItem::Type)
        {
          ORGraphicsLineItem * ent = (ORGraphicsLineItem*)item;
          cp.copy_item = ReportWriterSectionData::LineItem;
          cp.copy_line_start = ent->mapToScene(ent->line().p1());
          cp.copy_line_end = ent->mapToScene(ent->line().p2());
          cp.copy_pen  = ent->pen();
          cp.copy_offset = cp.copy_line_start - sectionData->copy_pos;
          sectionData->copy_list.append(cp);
        }
        else if(rtti == ORGraphicsRectItem::Type)
        {
          ORGraphicsRectItem * ent = (ORGraphicsRectItem*)item;
          cp.copy_item = ReportWriterSectionData::RectItem;
          cp.copy_rect = QRectF(ent->mapToScene(ent->rect().topLeft()), ent->rect().size());
          cp.copy_pen  = ent->pen();
          cp.copy_brush = ent->brush();
          cp.copy_offset = cp.copy_rect.topLeft() - sectionData->copy_pos;
          sectionData->copy_list.append(cp);
        }
        else if(rtti == ORGraphicsBarcodeItem::Type)
        {
          ORGraphicsBarcodeItem * ent = (ORGraphicsBarcodeItem*)item;
          cp.copy_item = ReportWriterSectionData::BarcodeItem;
          cp.copy_str1 = ent->query();
          cp.copy_str2 = ent->column();
          cp.copy_str3 = ent->format();
          cp.copy_int1 = ent->maxLength();
          cp.copy_int2 = ent->alignment();
          cp.copy_dbl = ent->narrowBarWidth();
          cp.copy_pen  = ent->pen();
          cp.copy_brush = ent->brush();
          cp.copy_rect = QRectF(ent->mapToScene(ent->rect().topLeft()), ent->rect().size());
          cp.copy_offset = cp.copy_rect.topLeft() - sectionData->copy_pos;
          sectionData->copy_list.append(cp);
        }
        else if(rtti == ORGraphicsImageItem::Type)
        {
          ORGraphicsImageItem * ent = (ORGraphicsImageItem*)item;
          cp.copy_item = ReportWriterSectionData::ImageItem;
          cp.copy_str1 = ent->query();
          cp.copy_str2 = ent->column();
          cp.copy_str3 = ent->inlineImageData();
          cp.copy_str4 = ent->mode();
          cp.copy_bool1 = ent->isInline();
          cp.copy_pen  = ent->pen();
          cp.copy_brush = ent->brush();
          cp.copy_rect = QRectF(ent->mapToScene(ent->rect().topLeft()), ent->rect().size());
          cp.copy_offset = cp.copy_rect.topLeft() - sectionData->copy_pos;
          sectionData->copy_list.append(cp);
        }
        else if(rtti == ORGraphicsGraphItem::Type)
        {
          ORGraphicsGraphItem * ent = (ORGraphicsGraphItem*)item;
          cp.copy_item = ReportWriterSectionData::GraphItem;
          cp.copy_rect = QRectF(ent->mapToScene(ent->rect().topLeft()), ent->rect().size());
          cp.copy_pen  = ent->pen();
          cp.copy_brush = ent->brush();
          ent->copyData(cp.copy_graph);
          cp.copy_offset = cp.copy_rect.topLeft() - sectionData->copy_pos;
          sectionData->copy_list.append(cp);
        }
        else
        {
          qDebug("Tried to copy an item I know nothing about!");
        }
      }
    }
  }
}

void ReportHandler::editPaste()
{
  // call the editPaste function passing it a point
  //  that make sense as defaults (same canvas / slightly offset pos of orig copy)
  QPointF p;
  p = sectionData->copy_pos + QPointF(7.0, 7.0);
  editPaste(p);
}


void ReportHandler::editPaste(const QPointF & pos)
{
  DocumentWindow * gw = activeDocumentWindow();
  if(!gw)
    return;

  // paste a new item of the copy we have in the specified location
  if(sectionData->copy_list.count() < 1)
    return;

  QGraphicsItem * section = 0;
  QList<QGraphicsItem*> list = gw->_scene->items(pos);
  for(int i = 0; i < list.count(); i++)
  {
    if(list.at(i)->type() == ORGraphicsSectionItem::Type)
    {
      section = list.at(i);
      break;
    }
  }

  if(!section)
  {
    QMessageBox::critical(gw, tr("No Section Found"), tr("Items must be pasted into a section."));
    return;
  }

  QGraphicsItem * pasted_ent = 0;
  gw->_scene->clearSelection();
  sectionData->mouseAction = ReportWriterSectionData::MA_None;
  ReportWriterCopyData cp;
  for(int i = 0; i < sectionData->copy_list.count(); i++)
  {
    pasted_ent = 0;
    cp = sectionData->copy_list[i];
    if(cp.copy_item == ReportWriterSectionData::LabelItem)
    {
      ORGraphicsLabelItem * ent = new ORGraphicsLabelItem(section);
      ent->setText(cp.copy_str1);
      ent->setFont(cp.copy_font);
      ent->setPen(cp.copy_pen);
      ent->setBrush(cp.copy_brush);
      ent->setTextFlags(cp.copy_int1);
      ent->setPos(section->mapFromScene(pos + cp.copy_offset));
      ent->setRect(0, 0, cp.copy_rect.width(),cp.copy_rect.height());
      pasted_ent = ent;
    }
    else if(cp.copy_item == ReportWriterSectionData::FieldItem)
    {
      ORGraphicsFieldItem * ent = new ORGraphicsFieldItem(section);
      ent->setQuery(cp.copy_str1);
      ent->setColumn(cp.copy_str2);
      ent->setFont(cp.copy_font);
      ent->setPen(cp.copy_pen);
      ent->setBrush(cp.copy_brush);
      ent->setTextFlags(cp.copy_int1);
      ent->setTrackTotal(cp.copy_bool1);
      ent->setFormat(cp.copy_str3, cp.copy_bool2);
      ent->setUseSubTotal(cp.copy_bool3);
      ent->setPos(section->mapFromScene(pos + cp.copy_offset));
      ent->setRect(0, 0, cp.copy_rect.width(),cp.copy_rect.height());
      pasted_ent = ent;
    }
    else if(cp.copy_item == ReportWriterSectionData::TextItem)
    {
      ORGraphicsTextItem * ent = new ORGraphicsTextItem(section);
      ent->setQuery(cp.copy_str1);
      ent->setColumn(cp.copy_str2);
      ent->setFont(cp.copy_font);
      ent->setPen(cp.copy_pen);
      ent->setBrush(cp.copy_brush);
      ent->setTextFlags(cp.copy_int1);
      ent->setPos(section->mapFromScene(pos + cp.copy_offset));
      ent->setRect(0, 0, cp.copy_rect.width(),cp.copy_rect.height());
      ent->setBottomPadding(cp.copy_dbl);
      pasted_ent = ent;
    }
    else if(cp.copy_item == ReportWriterSectionData::LineItem)
    {
      ORGraphicsLineItem * ent = new ORGraphicsLineItem(section);
      ent->setPen(cp.copy_pen);
      QLineF line(section->mapFromScene(cp.copy_line_start + cp.copy_offset), section->mapFromScene(cp.copy_line_end + cp.copy_offset));
      ent->setLine(line);
      pasted_ent = ent;
    }
    else if(cp.copy_item == ReportWriterSectionData::RectItem)
    {
      ORGraphicsRectItem * ent = new ORGraphicsRectItem(section);
      ent->setPen(cp.copy_pen);
      ent->setBrush(cp.copy_brush);
      ent->setPos(section->mapFromScene(pos + cp.copy_offset));
      ent->setRect(0, 0, cp.copy_rect.width(),cp.copy_rect.height());
      pasted_ent = ent;
    }
    else if(cp.copy_item == ReportWriterSectionData::BarcodeItem)
    {
      ORGraphicsBarcodeItem * ent = new ORGraphicsBarcodeItem(section);
      ent->setQuery(cp.copy_str1);
      ent->setColumn(cp.copy_str2);
      ent->setPen(cp.copy_pen);
      ent->setBrush(cp.copy_brush);
      ent->setFormat(cp.copy_str3);
      ent->setMaxLength(cp.copy_int1);
      ent->setAlignment(cp.copy_int2);
      ent->setNarrowBarWidth(cp.copy_dbl);
      ent->setPos(section->mapFromScene(pos + cp.copy_offset));
      ent->setRect(0, 0, cp.copy_rect.width(),cp.copy_rect.height());
      pasted_ent = ent;
    }
    else if(cp.copy_item == ReportWriterSectionData::ImageItem)
    {
      ORGraphicsImageItem * ent = new ORGraphicsImageItem(section);
      ent->setQuery(cp.copy_str1);
      ent->setColumn(cp.copy_str2);
      ent->setPen(cp.copy_pen);
      ent->setBrush(cp.copy_brush);
      ent->setInlineImageData(cp.copy_str3);
      ent->setMode(cp.copy_str4);
      ent->setInline(cp.copy_bool1);
      ent->setPos(section->mapFromScene(pos + cp.copy_offset));
      ent->setRect(0, 0, cp.copy_rect.width(),cp.copy_rect.height());
      pasted_ent = ent;
    }
    else if(cp.copy_item == ReportWriterSectionData::GraphItem)
    {
      ORGraphicsGraphItem * ent = new ORGraphicsGraphItem(section);
      ent->setData(cp.copy_graph);
      ent->setPen(cp.copy_pen);
      ent->setBrush(cp.copy_brush);
      ent->setPos(section->mapFromScene(pos + cp.copy_offset));
      ent->setRect(0, 0, cp.copy_rect.width(),cp.copy_rect.height());
      pasted_ent = ent;
    }
    else 
    {
      qDebug("tried to paste an item I don't understand.");
    }

    if(pasted_ent)
    {
      pasted_ent->setSelected(true);
      sectionData->mouseAction = ReportWriterSectionData::MA_Grab;
      gw->_scene->setModified(TRUE);
    }
  }
}

void ReportHandler::editPreferences()
{
  EditPreferences * dlgPref = new EditPreferences(0);
  if(dlgPref)
  {
    dlgPref->setDefaultFont(QFont(ORGraphicsRectItem::getDefaultEntityFont()));
    dlgPref->setShowGrid(gridOptions->isVisible());
    dlgPref->setSnapGrid(gridOptions->isSnap());
    dlgPref->setGridSize(gridOptions->xInterval() ,gridOptions->yInterval());
    if(dlgPref->exec() == QDialog::Accepted)
    {
      ORGraphicsRectItem::setDefaultEntityFont(dlgPref->defaultFont());
      gridShowAction->setChecked(dlgPref->showGrid());
      gridSnapAction->setChecked(dlgPref->snapGrid());
      gridOptions->setXInterval(dlgPref->gridSizeX());
      gridOptions->setYInterval(dlgPref->gridSizeY());
      QString newLanguage = dlgPref->selectedLanguage();
      if(newLanguage != _languages.selectedTitle())
      {
        QMessageBox::information(dlgPref, QString(tr("Language: %1").arg(newLanguage)), tr("The language change will take effect the next time the report writer will be run."));
        _languages.selectFromTitle(newLanguage);
      }
    }
    delete dlgPref;
  }
}

void ReportHandler::editSelectAll()
{
  DocumentWindow * gw = activeDocumentWindow();
  if(gw)
  {
    QList<QGraphicsItem*> list = gw->_scene->items();
    for(int i = 0; i < list.count(); i++)
    {
        list.at(i)->setSelected(true);
    }
  }
}

void ReportHandler::editProperties()
{
  DocumentWindow * gw = activeDocumentWindow();
  if(gw)
  {
    QList<QGraphicsItem*> list = gw->_scene->selectedItems();
    for(int i = 0; i < list.count(); i++)
    {
      switch(list.at(i)->type())
      {
        case ORGraphicsLineItem::Type:
          (static_cast<ORGraphicsLineItem*>(list.at(i)))->properties(gw);
          break;
        case ORGraphicsRectItem::Type:
        case ORGraphicsLabelItem::Type:
        case ORGraphicsFieldItem::Type:
        case ORGraphicsTextItem::Type:
        case ORGraphicsBarcodeItem::Type:
        case ORGraphicsImageItem::Type:
        case ORGraphicsGraphItem::Type:
          (static_cast<ORGraphicsRectItem*>(list.at(i)))->properties(gw);
          break;
      };
    }
  }  
}


// Doc Actions
//
void ReportHandler::docTitle()
{
  DocumentWindow * gw = activeDocumentWindow();
  if(gw)
    gw->_scene->docInfoEditor(gw);
}

void ReportHandler::docPageSetup()
{
  DocumentWindow * gw = activeDocumentWindow();
  if(gw)
    gw->_scene->docPageSetup(gw);
}

void ReportHandler::docQuerySourceList()
{
  DocumentWindow * gw = activeDocumentWindow();
  if(gw)
    gw->_scene->querySourceList();
}

void ReportHandler::docSectionEditor()
{
  DocumentWindow * gw = activeDocumentWindow();
  if(gw)
    gw->_scene->sectionEditor(gw);
}

void ReportHandler::docColorList()
{
  DocumentWindow * gw = activeDocumentWindow();
  if(gw)
    gw->_scene->colorList(gw);
}

void ReportHandler::docDefinedParams()
{
  DocumentWindow * gw = activeDocumentWindow();
  if(gw)
    gw->_scene->editDefinedParameters(gw);
}

//
// Item Actions
//
void ReportHandler::itemLabel() {
    sectionData->mouseAction = ReportWriterSectionData::MA_Insert;
    sectionData->insertItem = ReportWriterSectionData::LabelItem;
}

void ReportHandler::itemField() {
    sectionData->mouseAction = ReportWriterSectionData::MA_Insert;
    sectionData->insertItem = ReportWriterSectionData::FieldItem;
}

void ReportHandler::itemText() {
    sectionData->mouseAction = ReportWriterSectionData::MA_Insert;
    sectionData->insertItem = ReportWriterSectionData::TextItem;
}

void ReportHandler::itemLine() {
    sectionData->mouseAction = ReportWriterSectionData::MA_Insert;
    sectionData->insertItem = ReportWriterSectionData::LineItem;
}

void ReportHandler::itemCrossTab() {                                                                                         
    sectionData->mouseAction = ReportWriterSectionData::MA_Insert;                                                           
    sectionData->insertItem = ReportWriterSectionData::CrossTabItem;                                                         
}

void ReportHandler::itemRect() {
    sectionData->mouseAction = ReportWriterSectionData::MA_Insert;
    sectionData->insertItem = ReportWriterSectionData::RectItem;
}

void ReportHandler::itemBarcode() {
    sectionData->mouseAction = ReportWriterSectionData::MA_Insert;
    sectionData->insertItem = ReportWriterSectionData::BarcodeItem;
}

void ReportHandler::itemImage() {
    sectionData->mouseAction = ReportWriterSectionData::MA_Insert;
    sectionData->insertItem = ReportWriterSectionData::ImageItem;
}

void ReportHandler::itemGraph() {
    sectionData->mouseAction = ReportWriterSectionData::MA_Insert;
    sectionData->insertItem = ReportWriterSectionData::GraphItem;
}

// TODO: add in support for a pointer/grabber/none option
//       and make the tool items toggled

//
// Db Actions
//
void ReportHandler::dbConnect() {
    if(!_allowDBConnect) return;

    bool dbConnected = (dbConnectAction->text() == _strMenuDisconnect);

    if(dbConnected) {
        // disconnect
        QSqlDatabase db = QSqlDatabase::database(QSqlDatabase::defaultConnection,FALSE);
        if(db.isValid()) db.close();
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    } else {
        // connect
        ParameterList params;
        params.append("name", name());
        params.append("copyright", copyright());
        params.append("version", version());
        params.append("build", QString("%1 %2").arg(__DATE__).arg(__TIME__));
        params.append("databaseURL", _databaseURL);

        login newdlg(0, "", TRUE);
        newdlg.set(params);

        if(newdlg.exec() == QDialog::Accepted) {
            _databaseURL = newdlg._databaseURL;
        }
    }

    emit dbOpenClosed();
}

void ReportHandler::dbLoadDoc() {
    QSqlDatabase db = QSqlDatabase::database(QSqlDatabase::defaultConnection, FALSE);
    if(db.isValid()) {
        DBFileDialog rptDiag;
        rptDiag.setWindowTitle(tr("Load Report from Database"));
        rptDiag._name->setReadOnly(true);
        rptDiag._grade->setEnabled(false);
        if(rptDiag.exec() == QDialog::Accepted) {
            QDomDocument doc;
            QString errMsg;
            int errLine, errCol;
            if(doc.setContent(rptDiag.getSource(),&errMsg,&errLine,&errCol)) {
                DocumentWindow * rw = fileOpen(doc);
                if(rw)
                {
                  // setup any variables to let this doc know how it was loaded
                  // so when a save command is issued it knows what to do
                  rw->_scene->lastSaveToDb = TRUE;
                  rw->_scene->dbRecordName = rptDiag.getNameById();
                  rw->_scene->dbRecordGrade = rptDiag.getGradeById();
                }
            } else {
                QMessageBox::warning(0, tr("Error Loading Report"),
                    QString().sprintf(QString::fromLocal8Bit("ReportWriterWindow::dbLoadDoc() : ERROR on setContent()\n\t%s (Line %d Column %d)").toLatin1().constData(),errMsg.toLatin1().data(),errLine,errCol));
            }
        }
    } else {
        QMessageBox::warning(0, tr("No Database Connection"),
            tr("There is no database connection that can be used to load a document."));
    }
}

void ReportHandler::dbSaveDoc()
{
  QSqlDatabase db = QSqlDatabase::database(QSqlDatabase::defaultConnection, FALSE);
  if(db.isValid())
  {
    DocumentWindow * gw = activeDocumentWindow();
    if(gw)
      gw->_scene->saveToDb(gw);
  }
  else
  {
    QMessageBox::warning(0, tr("No Database Connection"),
        tr("There is no database connection that can be used to save this document."));
  }
}


//
// ReportWindow tracking methods
//
DocumentWindow * ReportHandler::activeDocumentWindow()
{
  DocumentWindow * gw = 0;
  // Using the activeWindow call from QWorkspace is more
  // reliable cross platform.
  if(_parentWindowIsWorkspace)
  {
    /* the obvious
      return (ReportWindow*)((QWorkspace*)_parentWindow)->activeWindow();
      doesn't work - bug 5870 - after printing a report with another window,
      QWorkspace::activeWindow() returns that other window even after
      the user has clicked back on a ReportWindow
     */
    QWidgetList list = ((QWorkspace*)_parentWindow)->windowList(QWorkspace::StackingOrder);
    int i;
    for (i = list.size() - 1; i >= 0; i--)
      if (list.at(i)->inherits("DocumentWindow") || list.at(i)->inherits("ReportWindow"))
        break;

    if (i >= 0 && list.at(i)->inherits("DocumentWindow"))
      return static_cast<DocumentWindow*>(list.at(i));
    return 0;
  }
  else
  {
    for(int it = 0; it < gwList.count(); it++)
    {
      gw = gwList.at(it);
      if(gw && QApplication::activeWindow() == gw)
        return gw;
    }
  }
  return 0;
}

void ReportHandler::addDocumentWindow(DocumentWindow * gw)
{
  if(gw) {
    gw->_handler = this;
    gw->_scene->_handler = this;
    gwList.append(gw);
    connect(gw, SIGNAL(destroyed(QObject*)), this, SLOT(removeReportWindow(QObject*)));
    if(_parentWindowIsWorkspace)
      ((QWorkspace*)_parentWindow)->addWindow(gw);
  }
}

void ReportHandler::removeReportWindow(QObject * obj)
{
  DocumentWindow * gw = static_cast<DocumentWindow *>(obj);
  if(gw)
    gwList.removeAll(gw);
}

//
// dbMenuAboutToShow
//
void ReportHandler::dbMenuAboutToShow() {
    QSqlDatabase db = QSqlDatabase::database(QSqlDatabase::defaultConnection, FALSE);
    if(db.isValid() && db.isOpen() && _databaseElt.isNull()) {
        dbLoadAction->setEnabled(true);
        dbSaveAction->setEnabled(true);
        dbConnectAction->setText(_strMenuDisconnect);
    } else {
        dbLoadAction->setEnabled(false);
        dbSaveAction->setEnabled(false);
        dbConnectAction->setText(_strMenuConnect);
    }
}

void ReportHandler::filePreview()
{
  print(true);
}

void ReportHandler::filePrint()
{
  print(false);
}

void ReportHandler::print(bool showPreview)
{
  int rw = 0;
  DocumentWindow * gw = activeDocumentWindow();
  if(!rw && !gw) {
    return;
  }

  QMainWindow * mw = 0;
  if(gw)
    mw = gw;

  QDomDocument ddoc;
  if(gw)
    ddoc = gw->_scene->document();

  ORPreRender pre;
  pre.setDom(ddoc);

  ParameterEdit dlgEdit(mw);
  if(dlgEdit.setDocument(ddoc))
  {
    if(dlgEdit.exec() != QDialog::Accepted) 
      return;
  }

  pre.setParamList(dlgEdit.getParameterList());
  ORODocument * doc = pre.generate();

  if(doc)
  {
    QPrinter printer(QPrinter::HighResolution);

    ORPrintRender render;
    render.setupPrinter(doc, &printer);

    if(showPreview) 
    {
      if(printer.printerName().isEmpty()) { // no default printer
        QPrintDialog pd(&printer);
        if(pd.exec() != QDialog::Accepted)
        {
          return; // no printer, can't preview
        }
      }

      PreviewDialog preview (doc, &printer, mw);
      if (preview.exec() == QDialog::Rejected) 
        return;
    }

    QPrintDialog pd(&printer);
    pd.setMinMax(1, doc->pages());
    if(pd.exec() == QDialog::Accepted)
    {
      render.setPrinter(&printer);
      render.render(doc);
    }
    delete doc;
  }
}

void ReportHandler::filePrintToPDF()
{
  int rw = 0;
  DocumentWindow * gw = activeDocumentWindow();
  if(!rw && !gw)
    return;
  QMainWindow * mw = 0;
  if(gw)
    mw = gw;
  QString outfile = QFileDialog::getSaveFileName( mw, tr("Choose filename to save"), tr("print.pdf"), tr("Pdf (*.pdf)") );

  if(outfile.isEmpty()) // User canceled save dialog
    return;

  if(gw)
    filePrintToPDF(gw, gw->_scene->document(), outfile);
}


void ReportHandler::filePrintToPDF(QWidget *wnd, const QDomDocument & doc, QString & pdfFileName)
{
  if(pdfFileName.isEmpty()) 
    return;

  if ( QFileInfo( pdfFileName ).suffix().isEmpty() )
    pdfFileName.append(".pdf");

  ParameterEdit dlgEdit(wnd);
  dlgEdit.setDocument(doc);
  if(dlgEdit.exec() != QDialog::Accepted) 
    return;

  ORPreRender pre;
  pre.setDom(doc);
  pre.setParamList(dlgEdit.getParameterList());
  ORODocument *odoc = pre.generate();
  if (odoc) {
      ORPrintRender::exportToPDF(odoc, pdfFileName );
      delete odoc;
  }
}

void ReportHandler::sReportsChanged(int pReportid, bool pLocaleUpdate)
{
  emit reportsChanged(pReportid, pLocaleUpdate);
}

void ReportHandler::onWinChanged(QWidget* w)
{
  bool show = w ? true : false;
  dbSaveAction->setEnabled(show & dbLoadAction->isEnabled());
  grpEdit->setEnabled(show);
  grpItem->setEnabled(show);
  grpGrid->setEnabled(show);
  grpDoc->setEnabled(show);
  grpFont->setEnabled(show);
}

QString ReportHandler::name()
{
#if defined Q_WS_WIN
  return QObject::tr("OpenRPT Report Writer for Windows");
#elif defined Q_WS_X11
  return QObject::tr("OpenRPT Report Writer for Linux");
#elif defined Q_WS_MAC
  return QObject::tr("OpenRPT Report Writer for OS X");
#else
  return QObject::tr("OpenRPT Report Writer");
#endif
}

QString ReportHandler::copyright()
{
  return QObject::tr("Copyright (c) 2002-2009, OpenMFG, LLC.");
}

QString ReportHandler::version()
{
  return "1.0";
}

int ReportHandler::insertItemCode() const
{
  return sectionData->insertItem;
}

void ReportHandler::resetInsertItemCode()
{
  sectionData->insertItem = ReportWriterSectionData::NoItem;

  foreach(QAction *action, grpItem->actions())
      action->setChecked(false);
}

void ReportHandler::buildItemContextMenu(QMenu * menu)
{
  if(selectionCount() > 0)
  {
    menu->addAction(editCutAction);
    menu->addAction(editCopyAction);
  }
  if(sectionData->copy_list.count() > 0)
  {
    menu->addAction(editPasteAction);
  }
  if(selectionCount() == 1)
  {
    menu->addSeparator();
    menu->addAction(editPropertiesAction);
  }
  if(selectionCount() > 1)
  {
    menu->addSeparator();
    menu->addAction(alignTopAction);
    menu->addAction(alignVCenterAction);
    menu->addAction(alignBottomAction);
    menu->addAction(alignLeftAction);
    menu->addAction(alignHCenterAction);
    menu->addAction(alignRightAction);
  }
  if(selectionCount() >= 1)
  {
    menu->addSeparator();
    menu->addAction(colorAction);
    menu->addAction(fillAction);
	menu->addAction(rotationAction);
  } 
}

QPointF ReportHandler::getCopyPoint() const
{
  return sectionData->copy_pos;
}

void ReportHandler::setCopyPoint(const QPointF & p)
{
  sectionData->copy_pos = p;
}

//////////////////////////////////////////////////////////////////////////////                                               
// Description                                                                                                               
//   Get icon name using the options for iconsize
//////////////////////////////////////////////////////////////////////////////
QString ReportHandler::getIconName(const QString& pDefaultName)
{
  QString defaultName = pDefaultName;
  QRegExp rx("_size");      // match ampersands but not &amp;

  // TODO: Get toolbar iconsize from options
  unsigned int iconSize(32);
  switch (iconSize)
  {
    case (16)  : defaultName.replace(rx, "_16x16");
                 break;
    case (24)  : defaultName.replace(rx, "_24x24");
                 break;
    case (32)  : defaultName.replace(rx, "_32x32");
                 break;
    case (48)  : defaultName.replace(rx, "_48x48");
                 break;
    case (64)  : defaultName.replace(rx, "_64x64");
                 break;
    default    : defaultName.replace(rx, "_32x32");
                 break;
  }
  defaultName.replace(":/OpenRPT/icons", "images");
//  qDebug()<<defaultName;
  return defaultName;
}


void ReportHandler::alignTop()
{
  qreal y = -1;
  DocumentWindow *gw = activeDocumentWindow();
  if(gw)
  {
    __dosnap = false;
    QList<QGraphicsItem*> list = gw->_scene->selectedItems();
    if(list.count() > 1)
    {
      y = list.at(0)->sceneBoundingRect().top();
      int i = 0;
      for(i = 0; i < list.count(); i++)
      {
        if(list.at(i)->type() == ORGraphicsLineItem::Type)
          y = qMin(y, list.at(i)->sceneBoundingRect().top());
        else
          y = qMin(y, list.at(i)->scenePos().y());
      }
      for(i = 0; i < list.count(); i++)
      {
        if(list.at(i)->type() == ORGraphicsLineItem::Type)
        {
          ORGraphicsLineItem * li = static_cast<ORGraphicsLineItem*>(list.at(i));
          qreal d = y - li->sceneBoundingRect().top();
          QLineF l = li->line();
          l.translate(0, d);
          li->setLine(l);
        }
        else
        {
          QGraphicsItem * item = list.at(i);
          QPointF p(item->scenePos().x(), y);
          item->setPos(item->mapToParent(item->mapFromScene(p)));
        }
      }
    }
    __dosnap = true;
  }
}

void ReportHandler::alignVCenter()
{
  qreal y = 0;
  DocumentWindow *gw = activeDocumentWindow();
  if(gw)
  {
    __dosnap = false;
    QList<QGraphicsItem*> list = gw->_scene->selectedItems();
    if(list.count() > 1)
    {
      int i = 0;
      for(i = 0; i < list.count(); i++)
      {
        if(list.at(i)->type() == ORGraphicsLineItem::Type)
          y += list.at(i)->sceneBoundingRect().center().y();
        else
        {
          QGraphicsRectItem * ri = static_cast<QGraphicsRectItem*>(list.at(i));
          if(ri)
            y += ri->scenePos().y() + (ri->rect().height() / 2.0);
          else
            y += list.at(i)->sceneBoundingRect().center().y();
        }
      }
      y = y / (qreal)i;
      for(i = 0; i < list.count(); i++)
      {
        if(list.at(i)->type() == ORGraphicsLineItem::Type)
        {
          ORGraphicsLineItem * li = static_cast<ORGraphicsLineItem*>(list.at(i));
          qreal d = y - li->sceneBoundingRect().center().y();
          QLineF l = li->line();
          l.translate(0, d);
          li->setLine(l);
        }
        else
        {
          QGraphicsRectItem * ri = static_cast<QGraphicsRectItem*>(list.at(i));
          if(ri)
          {
            QPointF p(ri->scenePos().x(), y - (ri->rect().height() / 2.0));
            ri->setPos(ri->mapToParent(ri->mapFromScene(p)));
          }
          else
          {
            QGraphicsItem * item = list.at(i);
            QPointF p(item->scenePos().x(), y - (item->sceneBoundingRect().height() / 2.0));
            item->setPos(item->mapToParent(item->mapFromScene(p)));
          }
        }
      }
    }
    __dosnap = true;
  }
}

void ReportHandler::alignBottom()
{
  qreal y = -1;
  DocumentWindow *gw = activeDocumentWindow();
  if(gw)
  {
    __dosnap = false;
    QList<QGraphicsItem*> list = gw->_scene->selectedItems();
    if(list.count() > 1)
    {
      y = list.at(0)->sceneBoundingRect().bottom();
      int i = 0;
      for(i = 0; i < list.count(); i++)
      {
        if(list.at(i)->type() == ORGraphicsLineItem::Type)
          y = qMax(y, list.at(i)->sceneBoundingRect().bottom());
        else
        {
          QGraphicsRectItem * ri = static_cast<QGraphicsRectItem*>(list.at(i));
          if(ri)
            y = qMax(y, ri->scenePos().y() + ri->rect().height());
          else
            y = qMax(y, list.at(i)->sceneBoundingRect().bottom());
        }
      }
      for(i = 0; i < list.count(); i++)
      {
        if(list.at(i)->type() == ORGraphicsLineItem::Type)
        {
          ORGraphicsLineItem * li = static_cast<ORGraphicsLineItem*>(list.at(i));
          qreal d = y - li->sceneBoundingRect().bottom();
          QLineF l = li->line();
          l.translate(0, d);
          li->setLine(l);
        }
        else
        {
          QGraphicsRectItem * ri = static_cast<QGraphicsRectItem*>(list.at(i));
          if(ri)
          {
            QPointF p(ri->scenePos().x(), y - ri->rect().height());
            ri->setPos(ri->mapToParent(ri->mapFromScene(p)));
          }
          else
          {
            QGraphicsItem * item = list.at(i);
            QPointF p(item->scenePos().x(), y - item->sceneBoundingRect().height());
            item->setPos(item->mapToParent(item->mapFromScene(p)));
          }
        }
      }
    }
    __dosnap = true;
  }
}

void ReportHandler::alignLeft()
{
  qreal x = -1;
  DocumentWindow *gw = activeDocumentWindow();
  if(gw)
  {
    __dosnap = false;
    QList<QGraphicsItem*> list = gw->_scene->selectedItems();
    if(list.count() > 1)
    {
      x = list.at(0)->sceneBoundingRect().left();
      int i = 0;
      for(i = 0; i < list.count(); i++)
      {
        if(list.at(i)->type() == ORGraphicsLineItem::Type)
          x = qMin(x, list.at(i)->sceneBoundingRect().left());
        else
          x = qMin(x, list.at(i)->scenePos().x());
      }
      for(i = 0; i < list.count(); i++)
      {
        if(list.at(i)->type() == ORGraphicsLineItem::Type)
        {
          ORGraphicsLineItem * li = static_cast<ORGraphicsLineItem*>(list.at(i));
          qreal d = x - li->sceneBoundingRect().left();
          QLineF l = li->line();
          l.translate(d, 0);
          li->setLine(l);
        }
        else
        {
          QGraphicsItem * item = list.at(i);
          QPointF p(x, item->scenePos().y());
          item->setPos(item->mapToParent(item->mapFromScene(p)));
        }
      }
    }
    __dosnap = true;
  }
}

void ReportHandler::alignHCenter()
{
  qreal x = 0;
  DocumentWindow *gw = activeDocumentWindow();
  if(gw)
  {
    __dosnap = false;
    QList<QGraphicsItem*> list = gw->_scene->selectedItems();
    if(list.count() > 1)
    {
      int i = 0;
      for(i = 0; i < list.count(); i++)
      {
        if(list.at(i)->type() == ORGraphicsLineItem::Type)
          x += list.at(i)->sceneBoundingRect().center().x();
        else
        {
          QGraphicsRectItem * ri = static_cast<QGraphicsRectItem*>(list.at(i));
          if(ri)
            x += ri->scenePos().x() + (ri->rect().width() / 2.0);
          else
            x += list.at(i)->sceneBoundingRect().center().x();
        }
      }
      x = x / (qreal)i;
      for(i = 0; i < list.count(); i++)
      {
        if(list.at(i)->type() == ORGraphicsLineItem::Type)
        {
          ORGraphicsLineItem * li = static_cast<ORGraphicsLineItem*>(list.at(i));
          qreal d = x - li->sceneBoundingRect().center().x();
          QLineF l = li->line();
          l.translate(d, 0);
          li->setLine(l);
        }
        else
        {
          QGraphicsRectItem * ri = static_cast<QGraphicsRectItem*>(list.at(i));
          if(ri)
          {
            QPointF p(x - (ri->rect().width() / 2.0), ri->scenePos().y());
            ri->setPos(ri->mapToParent(ri->mapFromScene(p)));
          }
          else
          {
            QGraphicsItem * item = list.at(i);
            QPointF p(x - (item->sceneBoundingRect().width() / 2.0), item->scenePos().y());
            item->setPos(item->mapToParent(item->mapFromScene(p)));
          }
        }
      }
    }
    __dosnap = true;
  }
}

void ReportHandler::alignRight()
{
  qreal x = -1;
  DocumentWindow *gw = activeDocumentWindow();
  if(gw)
  {
    __dosnap = false;
    QList<QGraphicsItem*> list = gw->_scene->selectedItems();
    if(list.count() > 1)
    {
      x = list.at(0)->sceneBoundingRect().right();
      int i = 0;
      for(i = 0; i < list.count(); i++)
      {
        if(list.at(i)->type() == ORGraphicsLineItem::Type)
          x = qMax(x, list.at(i)->sceneBoundingRect().right());
        else
        {
          QGraphicsRectItem * ri = static_cast<QGraphicsRectItem*>(list.at(i));
          if(ri)
            x = qMax(x, ri->scenePos().x() + ri->rect().width());
          else
            x = qMax(x, list.at(i)->sceneBoundingRect().right());
        }
      }
      for(i = 0; i < list.count(); i++)
      {
        if(list.at(i)->type() == ORGraphicsLineItem::Type)
        {
          ORGraphicsLineItem * li = static_cast<ORGraphicsLineItem*>(list.at(i));
          qreal d = x - li->sceneBoundingRect().right();
          QLineF l = li->line();
          l.translate(d, 0);
          li->setLine(l);
        }
        else
        {
          QGraphicsRectItem * ri = static_cast<QGraphicsRectItem*>(list.at(i));
          if(ri)
          {
            QPointF p(x - ri->rect().width(), ri->scenePos().y());
            ri->setPos(ri->mapToParent(ri->mapFromScene(p)));
          }
          else
          {
            QGraphicsItem * item = list.at(i);
            QPointF p(x - item->sceneBoundingRect().width(), item->scenePos().y());
            item->setPos(item->mapToParent(item->mapFromScene(p)));
          }
        }
      }
    }
    __dosnap = true;
  }
}

void ReportHandler::color()
{
    DocumentWindow *gw = activeDocumentWindow();
    if(!gw) 
        return;

    QList<QGraphicsItem*> list = gw->_scene->selectedItems();
    if(list.count() == 0) 
        return; // no selected items

    QColor selectedColor;

    for(int i = 0; i < list.count(); i++)
    {
        if(list.at(i)->type() == ORGraphicsLineItem::Type)
        {
          QGraphicsLineItem * l = static_cast<QGraphicsLineItem*>(list.at(i));
          if(l) {
            QPen p = l->pen();
            if(i==0) {
              selectedColor = QColorDialog::getColor (p.color(), gw);
              if(!selectedColor.isValid())
                break;
            }
            p.setColor(selectedColor);
            l->setPen(p);
          }
        }
        else
        {
          QAbstractGraphicsShapeItem * gs = static_cast<QAbstractGraphicsShapeItem*>(list.at(i));
          if(gs)
          {
            QPen p = gs->pen();
            if(i==0) {
              selectedColor = QColorDialog::getColor (p.color(), gw);
              if(!selectedColor.isValid())
                break;
            }
            p.setColor(selectedColor);
            gs->setPen(p);
          }
        }
    }
}


void ReportHandler::fill()
{
    DocumentWindow *gw = activeDocumentWindow();
    if(!gw) 
        return;

    QList<QGraphicsItem*> list = gw->_scene->selectedItems();
    if(list.count() == 0) 
        return; // no selected items

    QColor selectedColor;

    for(int i = 0; i < list.count(); i++)
    {
        QAbstractGraphicsShapeItem * item = static_cast<QAbstractGraphicsShapeItem*>(list.at(i));
        if(list.at(i)->type() != ORGraphicsLineItem::Type && item) {
            if(i==0)
            {
                selectedColor = QColorDialog::getColor (item->brush().color(), gw);
                if(!selectedColor.isValid())
                    break;
            }
            QBrush brush = item->brush();
            brush.setStyle(Qt::SolidPattern);
            brush.setColor(selectedColor);
            item->setBrush(brush);
        }
    }
}


void ReportHandler::rotation()
{
	DocumentWindow *gw = activeDocumentWindow();
	if(!gw) 
		return;

	QList<QGraphicsItem*> list = gw->_scene->selectedItems();
	if(list.count() == 0) 
		return; // no selected items

	bool ok = false;
	qreal angle = 0;

	for(int i = 0; i < list.count(); i++)
	{
		if(list.at(i)->type() != ORGraphicsLineItem::Type)
		{
			ORGraphicsRectItem * r = static_cast<ORGraphicsRectItem*>(list.at(i));
			if(r) 
			{
				if(!ok) 
				{
					angle = QInputDialog::getDouble(gw, tr("Rotation"),
						tr("Angle (0-360) :"), r->rotation(), 0, 360, 1, &ok);

					if(!ok) 
						return;
				}

				r->setRotation(angle); //temp
			}
		}
	}
}


void ReportHandler::fontChange(const QString &text)
{
    DocumentWindow *gw = activeDocumentWindow();
    if(!gw) 
        return;

    QList<QGraphicsItem*> list = gw->_scene->selectedItems();

    for(int i = 0; i < list.count(); i++)
    {
        setItemFontFamily(list.at(i), text);
    }
}

void ReportHandler::fontSizeChange(const QString &text)
{
    DocumentWindow *gw = activeDocumentWindow();
    if(!gw) 
        return;

    QList<QGraphicsItem*> list = gw->_scene->selectedItems();

    for(int i = 0; i < list.count(); i++)
    {
        setItemFontSize(list.at(i), text);
    }
}

void ReportHandler::fontWeightChange(bool checked)
{
    DocumentWindow *gw = activeDocumentWindow();
    if(!gw) 
        return;

    QList<QGraphicsItem*> list = gw->_scene->selectedItems();

    for(int i = 0; i < list.count(); i++)
    {
        setItemFontWeight(list.at(i), checked);
    }
}

void ReportHandler::fontStyleChange(bool checked)
{
    DocumentWindow *gw = activeDocumentWindow();
    if(!gw) 
        return;

    QList<QGraphicsItem*> list = gw->_scene->selectedItems();

    for(int i = 0; i < list.count(); i++)
    {
        setItemFontStyle(list.at(i), checked);
    }
}


void ReportHandler::setFontFamily(const QString &text)
{
  QObject * w = parent();
  if(_placeToolbarsOnWindows)
    w = activeDocumentWindow();
  if(!w)
    return;
  QFontComboBox * o = w->findChild<QFontComboBox*>("font_family");
  
  if(o)
    o->setEditText(text);
}

void ReportHandler::setFontSize(const QString &text)
{
  QObject * w = parent();
  if(_placeToolbarsOnWindows)
    w = activeDocumentWindow();
  if(!w)
    return;
  QComboBox * o = w->findChild<QComboBox*>("font_size");
  
  if(o)
    o->setEditText(text);
}

void ReportHandler::setFontWeight(bool v)
{
  QObject * w = parent();
  if(_placeToolbarsOnWindows)
    w = activeDocumentWindow();
  if(!w)
    return;
  QPushButton * o = w->findChild<QPushButton*>("font_weight");
  
  if(o)
    o->setChecked(v);
}

void ReportHandler::setFontStyle(bool v)
{
  QObject * w = parent();
  if(_placeToolbarsOnWindows)
    w = activeDocumentWindow();
  if(!w)
    return;
  QPushButton * o = w->findChild<QPushButton*>("font_style");
  
  if(o)
    o->setChecked(v);
}
