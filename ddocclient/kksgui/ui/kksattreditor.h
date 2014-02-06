/***********************************************************************
 * Module:  KKSAttrEditor.h
 * Author:  yuriyrusinov
 * Modified: 24.01.2009 �. 11:32:00
 * Purpose: Declaration of the class KKSAttrEditor
 * Comment: ������ ����� ����������� �� KKSDialog � ������������ ���
 *  ������������ � �������������� �������� ���������
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSAttrEditor_h)
#define __KKSSITOOOM_KKSAttrEditor_h

#include "kksgui_config.h"

#include <QDialog>
#include <KKSMap.h>

namespace Ui
{
    class kksattr_editor;
};

class QModelIndex;
class QAbstractItemModel;

class KKSAttribute;
class KKSAttrType;
class KKSAGroup;
class KKSObject;
class KKSSearchTemplate;
class KKSRecWidget;

class QAbstractItemModel;
class QModelIndex;

class _GUI_EXPORT KKSAttrEditor : public QDialog
{
public:
    KKSAttrEditor (KKSAttribute *attr, const KKSMap<int, KKSAttrType *>& aTypes, const KKSMap<int, KKSAGroup*>& aGroups, const QMap<int, QString>& Refs, QWidget *parent=0);
    virtual ~KKSAttrEditor (void);

    KKSAttribute * getAttribute (void);
    const KKSAttribute * getAttribute (void) const;

    int getTypeID (void) const;
    int getGroupId (void) const;
    void setGroupId (int idGroup);

    void uploadReferenceFields (const QMap<QString, QString>& fields);
    const QMap<int, QString>& getReferences (void) const;
    void setReferences (const QMap<int, QString>& refs);
    void setIO (KKSObject *o);
    void setCurrentRef (int index);
    void setCurrentRef (const QString& text);
    void setSearchTemplate(KKSSearchTemplate * st);

    void setRecWidget(KKSRecWidget *recW);


signals:
    void getReferences (KKSAttribute * attr, KKSAttrEditor * aEditor);
    void getReferenceFields (KKSAttribute * attr, int idRef, KKSAttrEditor * aEditor);
    void getSearchTemplate(KKSAttrEditor * aEditor);

    void showAttrsWidget(KKSAttribute *, KKSAttrEditor *);//�������� ������ � ���������� ��� ������ � ������������ ���������� ��������
    void addAttribute(KKSAttribute *, QAbstractItemModel*, KKSAttrEditor *);
    void editAttribute(int, KKSAttribute *, QAbstractItemModel*, const QModelIndex&, KKSAttrEditor *);
    void delAttribute(int, KKSAttribute *, QAbstractItemModel*, const QModelIndex&, KKSAttrEditor *);
    
    void loadHistRefs (KKSAttribute * attr, int aType, KKSAttrEditor * aEditor);

public slots:
    void addTriggered(QAbstractItemModel * sourceMod, const QModelIndex& parent);//��������� ������ addAttribute
    void editTriggered(QAbstractItemModel * sourceMod, const QModelIndex& wIndex);//��������� ������ editAttribute
    void delTriggered(QAbstractItemModel * sourceMod, const QModelIndex& wIndex);//��������� ������ delAttribute

private slots:
    void accept ();
    void currentAttrTypeChanged (int index);
    void currentReferenceChanged (int index);
    void setCodeEnabled (int state);
    void slotAddFilterClicked();
    void slotDelFilterClicked();
    void setTitleText (const QString& text);
    void addAttrs();

private:
    //
    // Functions
    //
    void init_widgets (void);
    void set_types (void);
    void set_references (void);
    void set_groups (void);


private:
    //
    // Variables
    //
    KKSAttribute *attribute;
    KKSMap<int, KKSAttrType *> attrTypes;
    KKSMap<int, KKSAGroup *> attrsGroups;
    QMap<int, QString> ioRefs;
    Ui::kksattr_editor* ui;
    KKSObject *io;
    QList<int> cRefTypes;

    KKSSearchTemplate * searchTemplate;//������� ������ (���� �� �����, �� < 0)

    KKSRecWidget * m_recW;//����������� ������ ����������� ��������� � �������� ��� ������ ��������������� �����
                          //� ����� ������� ���������� ������
                          //���������� ��-�� �������� ��������� ������ �������� �� ����������� ���� ����� 
                          //� ������� �� ��������������� ������
                          //�������� � ������ ������ �� ������������


private:
    Q_OBJECT
};

#endif
