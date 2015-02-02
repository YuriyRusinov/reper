#ifndef DNSPECBATH_H
#define DNSPECBATH_H

#include <QMainWindow>
#include <QMessageBox>
#include <QString>
#include <QList>
#include <QFileDialog>
#include <QDialog>
#include <QInputDialog>
#include <QSettings>
#include <QTreeWidgetItem>
#include "gdal_priv.h"
#include "Added/dnimagelib.h"
#include "imagearea.h"
#include "Added/First/structs.h"
#include "Added/dnpoly2.h"
#include "Added/dnvector.h"
#include "ui_DlgShowDeptch.h"
#include "ui_DlgSKOGraph.h"
#include "ui_dnspecbath.h"
#include "ui_DlgMouseCoord.h"

#include "Dlg/dlgspecgraph.h"
#include "Dlg/dlggraph.h"
#include "Dlg/dlgisodata.h"
#include "Dlg/dlgsam.h"
#include "Dlg/dlgmagicwand.h"
#include "Dlg/dlgcreateimgpoly.h"
#include "Dlg/dlgclassfilter.h"
#include "Dlg/dlgakfimg.h"

#include "Dlg/TheamDlg/dlgmaskarea.h"
#include "Dlg/TheamDlg/dlgroutmove.h"
#include "Dlg/TheamDlg/dlgroutmove2.h"
#include "Dlg/TheamDlg/dlgsmoces.h"
#include "Dlg/TheamDlg/dlgembedobj.h"
#include "Dlg/TheamDlg/dlgmultitempdata.h"

namespace Ui {
class DNSpecBath;
}

class DNSpecBath : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit DNSpecBath(QWidget *parent = 0);
    ~DNSpecBath();
    
    DnImageLib *GdalImage;
    DNPoly2 *SerPoly;
    QString FileNameOpen;
    int nCh[3];
    float BKof[3];
    int XMouse,YMouse;

    QString NameProg;
    QString NameOrg;
    QString AppDir;
    QList <QString> NamesPoly;
    QString CurrentNamePoly; //Имя текущего полигона
    QString CurrentNameClassif; //Имя текущей классификации (содержит также имя полигона к которому применена классификация)

    QString PolygonsName; //Имена полигонов, создающихся автоматически по результатам классификации предопределена в конструкторе но может меняться

    bool IsCurPolyClassif; //Признак того что текущая позиция является результатом классификации
    bool IsTreePolygonClear;
    bool IsPolygonCreateMode; //Признак включенного/выключенного режима создания полигона
    QList <DNVector> Polygons;

    Ui_DlgShowDeptch *DlgShowDepth;
    Ui_DlgSKOGraph *DlgSKOGraph;
    Ui_DlgMouseCoord *DlgMouseCoord;

    DlgSpecGraph *DlgSG;
    DlgGraph *DlgG;
    DlgIsoData *DlgID;
    DlgSAM *DlgSM;
    DlgMagicWand *DlgMW;
    DlgCreateImgPoly *DlgCIP;
    DlgClassFilter *DlgCF;
    DlgAKFImg *DlgAI;

    /*Окна для тематических задач*/
    DlgMaskArea *DlgMask;
    DlgRoutMove *DlgRMove;
    DlgRoutMove2 *DlgRMove2;
    DlgSmoces *DlgSmoce;
    DlgEmbedObj *DlgEO;
    DlgMultiTempData *DlgMTD;





signals:
    void SIGNALcreateVector();

private slots:
    void on_FileOpen_triggered();
    void on_ButtonReCalcImg_clicked();
    void on_OpenLastImg_triggered();
    void on_CreatePoly_triggered(bool checked);
//    void on_treePolygons_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void on_treePolygons_itemClicked(QTreeWidgetItem *item, int column);
    void on_Batinometriy_triggered();
    void on_MultiChan_triggered();
    void on_pbVectorize_clicked();
    void on_ShowDeptch_triggered();

    void MouseMove(int x,int y);
    void MouseLClicked(int x,int y);

    void on_CreateSpecGraph_triggered(bool checked);

    void on_CreateSKOGraph_triggered();

    void on_DlgSKOGraph_OK();
    void on_DlgMouseCoord_OK();

    void DlgGraphDestroy();

    void on_OilWater_triggered();

    void on_IsoData_triggered();
    void on_DlgIsoData_OK();

    void on_SAM_triggered();
    void on_DlgSAM_OK();

    void on_BayesClassif_triggered();
    void on_BayesClassif_OK();

    void on_MagicWand_triggered();
    void on_DlgMagicWand_OK();

    void on_AddChanal_triggered();

    void on_CreateImgPoly_triggered();
    void on_DlgCreateImgPoly_OK();

    void on_ClassFilter_triggered();
    void on_DlgClassFilter_OK();

    void on_SaveSerFileShift_triggered();

    void on_CreateAKFImg_triggered();
    void on_DlgCreateAKFImg_OK();
    void on_DlgCreateAKFImg_Rejected();
    void on_DlgCreateAKFImgCombo(int SideSq);

    void on_MaskPokr_triggered();
    void on_DlgMask_OK();

    void on_RoutMove_triggered();
    void on_DlgRMove_OK();

    void on_RoutMove2_triggered();
    void on_DlgRMove2_OK();

    void on_PolyExp_triggered();
    void on_PolyImp_triggered();

    void on_ClassResultToPoly_triggered();

    void on_Smoces_triggered();
    void on_DlgSmoces_OK();



    void on_AddIntegralChanal_triggered();

    void on_EmbedObj_triggered();
    void on_DlgEmbedObj_OK();

    void on_ForOlga_triggered();

    void on_MultiTempDatal_triggered();
    void on_DlgMultiTempDatal_OK();

private:
    Ui::DNSpecBath *ui;
    QString FileNameLast;
    QString FileNameChan;

    void FillMainForm(); //Заполнить форму главного окна
    void FillTreePolygons(QString StartPath, QTreeWidgetItem *ItemParent); //Заполнить дерево полигонов
    void FillTreePolygons(QString StartPath, QTreeWidget *ItemParent); //Заполнить дерево полигонов
    void FillStackPolygons(); //Заполнить стэк полигонов
    void ReadFilePoly(QString PolyFileName);
    void ReadFileClassif(QString PolyFileClassif);
    QString NamePolyToFile(QString NamePoly);
    void ChangeCurrentPoly(); //Событие изменения текущего полигона
    int NumCurPoly(); //Определение номера текущего полигона в стеке WidgetImg


protected:
    void resizeEvent(QResizeEvent*);
};

#endif // MAINWINDOW_H
