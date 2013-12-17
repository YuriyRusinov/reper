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
#include "ui_DlgShowDeptch.h"
#include "ui_dnspecbath.h"

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
    QString CurrentNamePoly; //��� �������� ��������
    QString CurrentNameClassif; //��� ������� ������������� (�������� ����� ��� �������� � �������� ��������� �������������)
    bool IsCurPolyClassif; //������� ���� ��� ������� ������� �������� ����������� �������������
    bool IsTreePolygonClear;
    QList <DNVector> Polygons;

    Ui_DlgShowDeptch *DlgShowDepth;

signals:
    void SIGNALcreateVector();

private slots:
    void on_FileOpen_triggered();
    void on_ButtonReCalcImg_clicked();
    void on_OpenLastImg_triggered();
    void on_CreatePoly_triggered(bool checked);
//    void on_treePolygons_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void on_Batinometriy_triggered();
    void on_MultiChan_triggered();
    void on_pbVectorize_clicked();
    void on_ShowDeptch_triggered();

    void MouseMove(int x,int y);

//    void on_treePolygons_clicked(const QModelIndex &index);

    void on_treePolygons_itemClicked(QTreeWidgetItem *item, int column);

private:
    Ui::DNSpecBath *ui;
    QString FileNameLast;

    void FillMainForm(); //��������� ����� �������� ����
    void FillTreePolygons(QString StartPath, QTreeWidgetItem *ItemParent); //��������� ������ ���������
    void FillTreePolygons(QString StartPath, QTreeWidget *ItemParent); //��������� ������ ���������
    void FillStackPolygons(); //��������� ���� ���������
    void ReadFilePoly(QString PolyFileName);
    void ReadFileClassif(QString PolyFileClassif);
    QString NamePolyToFile(QString NamePoly);
    void ChangeCurrentPoly(); //������� ��������� �������� ��������


protected:
    void resizeEvent(QResizeEvent*);
    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
