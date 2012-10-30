/***********************************************************************
 * Module:  KKSTemplate.h
 * Author:  sergey
 * Modified: 1 ������� 2008 �. 12:12:58
 * Purpose: Declaration of the class KKSTemplate
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSTemplate_h)
#define __KKSSITOOOM_KKSTemplate_h

#include <QMetaType>

class KKSCategory;

#include "KKSAttrGroup.h"
#include "KKSCategoryAttr.h"
#include "KKSRecord.h"
#include "KKSList.h"
#include "KKSMap.h"
#include "KKSAttrView.h"
#include "kkspp_config.h"

class _PP_EXPORT KKSTemplate : public KKSRecord
{
    public:
        KKSTemplate();
        KKSTemplate(const KKSTemplate & t);
        KKSTemplate(int id, const QString & name, KKSCategory * c);
        virtual ~KKSTemplate();

        const KKSCategory * category() const;
        KKSCategory * category();
        void setCategory(KKSCategory * c);

        int attrsCount() const;

        const KKSMap<int, KKSCategoryAttr *> availableAttrs() const;     //� �������� ����� ������������ idAttribute! //�� idAttrView ��� idCategoryAttr !!
        const KKSList<KKSAttrView*> sortedAttrs() const;

        const KKSMap<int, KKSAttrView *> attributes() const;  //���������� �������� ���������, ������������ ��������. ������������ ������ ������������
                                                              //� �������� ����� ������������ idAttribute! //�� idAttrView ��� idCategoryAttr !!

        int removeAttribute(int idAttribute);

        const KKSMap<int, KKSAttrGroup *> & groups() const;
        void setGroups(const KKSMap<int, KKSAttrGroup *> & g);
        void addGroup(KKSAttrGroup * g);
        //������ ������� ������ � ������������� � ��� ���������� �� �������
        //���� �� ��������� ������ ��������� ������ ������ ������� ��� �� ������������
        //�� ��� �������� ��������� recursive=TRUE, 
        //��������� ����� ��� �������� ������ (��������� ����������)
        //� ��������� ������ - �������� �� ���������� � ������������ ERROR_CODE
        int removeGroup( KKSAttrGroup * g, bool recursive = false);
        int removeGroup(int index, bool recursive = false);
        
        KKSAttrGroup * group(int index);
        const KKSAttrGroup * group(int index) const;
        KKSTemplate & operator = (const KKSTemplate & t);

        //
        // ��������� ������� ������ ������ �� id
        // ��������� �.�.��������� 16.11.2010
        //
        KKSAttrGroup * searchGroupById (int idGroup, bool recursive=true);
        const KKSAttrGroup * searchGroupById (int idGroup, bool recursive=true) const;
        QList<int> groupKeys (void) const;

    protected:
    private:
        //
        // Functions
        //
        KKSMap<int, KKSAttrView *> groupAttrs (KKSAttrGroup * g) const;

    private:
        //
        // Variables
        //
        KKSCategory* m_category;
        KKSMap<int, KKSAttrGroup*> m_groups;
};

Q_DECLARE_METATYPE (KKSTemplate);
Q_DECLARE_METATYPE (const KKSTemplate *);

#endif
