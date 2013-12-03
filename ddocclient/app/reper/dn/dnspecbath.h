#ifndef DNSPECBATH_H
#define DNSPECBATH_H

#include <QMainWindow>
#include <QMessageBox>
#include <QString>
#include <QList>
#include <QFileDialog>
#include <QInputDialog>
#include <QSettings>
#include <QTreeWidgetItem>
#include "gdal_priv.h"
#include "dn/Added/dnimagelib.h"
#include "dn/imagearea.h"
#include "dn/Added/First/structs.h"
#include "dn/Added/dnpoly.h"
#include "dn/Added/dnvector.h"
//#include "dnwidgetimage.h"

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
    DNPoly *SerPoly;
    QString FileNameOpen;
    int nCh[3];
    float BKof[3];

    QString NameProg;
    QString NameOrg;
    QString AppDir;
    QList <QString> NamesPoly;
    QString CurrentNamePoly; //Имя текущего полигона
    QString CurrentNameClassif; //Имя текущей классификации (содержит также имя полигона к которому применена классификация)
    bool IsCurPolyClassif; //Признак того что текущая позиция является результатом классификации

    QList <DNVector> Polygons;

signals:
    void SIGNALcreateVector();

private slots:
    void on_FileOpen_triggered();
    void on_ButtonReCalcImg_clicked();
    void on_OpenLastImg_triggered();
    void on_CreatePoly_triggered(bool checked);
    void on_treePolygons_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_Batinometriy_triggered();

    void on_MultiChan_triggered();

    void on_pbVectorize_clicked();

private:
    Ui::DNSpecBath *ui;
    QString FileNameLast;

    void FillMainForm(); //Заполнить форму главного окна
    void FillTreePolygons(QString StartPath, QTreeWidgetItem *ItemParent); //Заполнить дерево полигонов
    void FillTreePolygons(QString StartPath, QTreeWidget *ItemParent); //Заполнить дерево полигонов
    void FillStackPolygons(); //Заполнить стэк полигонов
    void ReadFilePoly(QString PolyFileName);
    void ReadFileClassif(QString PolyFileClassif);
    QString NamePolyToFile(QString NamePoly);
    void ChangeCurrentPoly(); //Событие изменения текущего полигона


protected:
    void resizeEvent(QResizeEvent*);
};

#endif // MAINWINDOW_H
