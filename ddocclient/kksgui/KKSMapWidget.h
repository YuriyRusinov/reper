/***********************************************************************
 * Module:  KKSMapWidget.h
 * Author:  sergey
 * Modified: 13 ������� 2013 �. 14:46:21
 * Purpose: Declaration of the class KKSMapWidget
 * ������ ��� ������ � ������
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSMapWidget_h)
#define __KKSSITOOOM_KKSMapWidget_h

#include "kksgui_config.h"

#include <KKSAttrWidget.h>
#include <QString>
#include <QList>
#include <QListWidget>
#include <QVariant>
#include <QDockWidget>
#include <QDomDocument>


class KKSAttrValue;
class KKSFile;

#ifdef __USE_QGIS__
class KKSGISWidget;
#endif

class _GUI_EXPORT KKSMapWidget : public QWidget, public KKSAttrWidget
{
    Q_OBJECT
public:
    KKSMapWidget(const QString & gisHomeDir, const KKSAttrValue* attr = 0, KKSIndAttrClass isSys = iacIOUserAttr, QWidget *parent=NULL);
    virtual ~KKSMapWidget(void);

    QDockWidget * legendWidget() const;
    QDockWidget * layerOrderWidget() const;

signals:
    void valueChanged (qint64 id, KKSIndAttrClass sys, QVariant val);
    void aboutToDestroy(QDockWidget *, QDockWidget *);
    void downloadFile(KKSFile * f, QWidget * parent);//������ �� �������� ����� �� ����� �� ��. ���� �������� ��������������� �� ������� io_urls

public slots:
    void slotMapChanged(QDomDocument&);
    void slotDataChanged();
private:
    QDockWidget * m_legendWidget;
    QDockWidget * m_layerOrderWidget;
    QString m_GISHomeDir;
    bool m_dataChanged;

    void init(QWidget * parent);

    int openProject();//��������� ������, �������� � �������� �������� ��������
    int downloadLayers(const QString & homeDir, const QString & xmlPrj);//�������� �� ������ ������ �� ������, ���� ���� ������������ ������� (�.�. �� PostGIS)
    QString getFileNameForDS(const QString & path, const QString & provider);//����� �����, ������� ������������ ����� �������� �����, �� datasource ����
    qint64 getIdUrlForID(const QString & id);//����� �����, ������� ������������ ����� ������������� ����� � ������� io_urls, �� id ����

#ifdef __USE_QGIS__

private:
    void initQGIS(QWidget *parent);

private:
    KKSGISWidget * mpKKSGISWidget;
#endif

};

#endif
