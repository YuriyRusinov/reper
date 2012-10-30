/***********************************************************************
 * Module:  KKSCategory.h
 * Author:  sergey
 * Modified: 27 ������ 2008 �. 12:03:27
 * Purpose: Declaration of the class KKSCategory
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSCategory_h)
#define __KKSSITOOOM_KKSCategory_h

class KKSType;
class KKSState;
class KKSLifeCycle;
class KKSRubric;
class KKSAttrType;
class KKSAccessEntity;

#include <KKSRecord.h>
#include "KKSMap.h"
#include "KKSCategoryAttr.h"
#include "KKSTemplate.h"
#include "KKSFilter.h"
#include "KKSAttrType.h"
#include "kkspp_config.h"

class _PP_EXPORT KKSCategory : public KKSRecord
{
    public:
        KKSCategory();
        KKSCategory(const KKSCategory & c);
        KKSCategory(int id, const QString & name, KKSType * type);
        virtual ~KKSCategory();

        const KKSCategory * tableCategory() const;
        KKSCategory * tableCategory();
        const KKSCategory * recAttrCategory() const;
        KKSCategory * recAttrCategory();

        const KKSType * type() const;
        KKSType * type();

        bool isMain() const;
        void setMain(bool isMain);
   
        const KKSMap<int, KKSCategoryAttr *> & attributes() const;
        KKSMap<int, KKSCategoryAttr *> & attributes();
        const KKSCategoryAttr * attribute(int id) const;
        KKSCategoryAttr * attribute(int id);

        //����� ������� ������ �� ��������� �������� ���������.
        //����� �������� ������� ��� �������� ���������,
        //����� ���������� �������� ��� ��������� ������������� �� 
        //� ������ ����������� �� ��������� ��� ��� ���� ��������� (�����) ����������� �������
        /*KKSFilter * createFilter(const QString & attrCode, 
                                 const QString & value, 
                                 KKSFilter::FilterOper operation);
   
        KKSFilter * createFilter(const QString & attrCode, 
                                 const QStringList & values, 
                                 KKSFilter::FilterOper operation);*/

        KKSFilter * createFilter(int attrId, 
                                 const QString & value, 
                                 KKSFilter::FilterOper operation);
   
        KKSFilter * createFilter(int attrId, 
                                 const QStringList & values, 
                                 KKSFilter::FilterOper operation);

        void setAttributes(const KKSMap<int, KKSCategoryAttr *> & _attributes);
        //�������� ����� ������ �������� � ���������� ������ (��) ������
        //������� ��� �������� ����� ��������� � ���������� � ��� �����, 
        //��� �� ����������� � �� ���������, 
        //������������� ������������ ������������� ��������������
        //������ ������������� �������� �� ��������� �� ����������� � �� ��������
        //� ��� �������� ������ - ������� ��������� ��� � ��
        int addAttribute(int idAttribute, KKSCategoryAttr * a); //�� idCategoryAttr !!
        int removeAttribute(int idAttribute);//�� idCategoryAttr !!
        //////int removeAttribute(const QString & code);
        int replaceAttribute(int idAttribute, KKSCategoryAttr * a); //�� idCategoryAttr !!

        const KKSLifeCycle * lifeCycle() const;
        KKSLifeCycle * lifeCycle();

        void setTableCategory(KKSCategory * _tableCategory);
        void setRecAttrCategory(KKSCategory * _recAttrCategory);
        void setType(KKSType * _type);
        void setLifeCycle(KKSLifeCycle * _lifeCycle);

        //���������� ������ �� ��������� ��� ������ ���������
        //�.�. ����� ��������� �� ����� �� ������ �������
        //� ���� ������ ������ ����� ���� ����� ������ 
        //� ����� ��������� � ���� ������ ��������� ������ ��������� � ���������
        //(������� ��������� ���������)
        const KKSTemplate & defTemplate() const;

        //������� ��� ���������
        //!!!�� �������� �������������� ������ ���������, ������� ������� ��� �������� �������� ���
        //������ ������ ��� ������� ������ ��������, 
        //�� ����������� ������ �������� �������� �� ���������, 
        //����� ��������� ������� ����� ������� �� ���� ���������������� ��������
        
        //�����!! ���� ������ ��� �� ������ � �� ������� ������������ ������������� ��������� ��� ��������������� ��������
        
        //�����!! �������� ������ � ��������� ����� ������ � ��� ������, 
        //���� ����� ������� category() ��������� �� ���������, � ������� ����������� ������. 
        const KKSMap<int, KKSTemplate *> & getTemplates() const;
        KKSMap<int, KKSTemplate *> & getTemplates();
        const KKSTemplate * getTemplate(int idTemplate) const;
        KKSTemplate * getTemplate(int idTemplate);

        int addTemplate(KKSTemplate * t);
        int removeTemplate(int idTemplate);
        int clearTemplates();


        bool isSystem() const;
        void setAsSystem(bool yes = true);

        //����������� ������������ ���������.
        //������ �� ���������� ��������� ����� ����������� � �������������� ������
        bool isGlobal() const;
        void setAsGlobal(bool yes = true);

        //������ ������ ������������ ��� ������� � ��������� �������� �������, 
        //������� �������� �������� ������ ���������
        //�������� ������� � ��������� ����� ���� ������ ���� ��� �� ���� ������.
        //� ��������� ������ ��� ��������, ��� ��������� �� ������ �������
        void setRootRubric(KKSRubric * r);
        KKSRubric * rootRubric() const;

        //�������� �����������, � �������� �������������� 
        //������������� (�������������� �����) ������ ���������
        //�.�. �������� �����������, �� ������� ������ ��������� 
        //������ ���� ���������� ��� �� ��������, ���������, �������� �� ��
        //�������� ������� ���������������
        //�������������� ��� ������� ���� atCheckListEx
        //KKSValue replicatedOrgsV() const;
        //const QList<int> & replicatedOrgs() const;

        //int addReplicatedOrg(int id);
        //int removeReplicatedOrg(int id);
        //void setReplicatedOrgs(const QList<int> & orgs);
        //void setReplicatedOrgsV(const KKSValue & v);


        const KKSState * state() const;
        KKSState * state();
        void setState(KKSState * state);

        QList<int> searchAttributesByType (int aType) const;
        bool isAttrTypeContains (KKSAttrType::KKSAttrTypes type) const;

        const KKSAccessEntity * getAccessRules (void) const;
        KKSAccessEntity * getAccessRules (void);
        void setAccessRules (KKSAccessEntity * acl);

    protected:
    private:
        bool m_isMain;

        bool m_attrsModified;
        void setModified(bool yes);

        KKSType* m_type;
        KKSCategory* m_tableCategory; //��������� ��������� ����� ���������, ������� ������������� �������� � ����������� �������
        KKSCategory* m_recAttrCategory;//��������� ��������� ����� ���������������� ��������� (�����������), �������� ����� �������� ������ ������������
        KKSMap<int, KKSCategoryAttr *> m_attributes;//�������� ���������. �����!! � �������� ����� ������������ idAttribute, �� idCategoryAttr !!!
        KKSLifeCycle* m_lifeCycle;

        KKSRubric * m_rootRubric;

        mutable KKSTemplate m_defTemplate;
        KKSMap<int, KKSTemplate *> m_templates;//������� ���������.

        bool m_isSystem;
        bool m_isGlobal;

        KKSState * m_state;

        KKSAccessEntity * m_acl;
        //QList<int> m_replicatedOrgs;

        //��� �������� �� �������� ����������, ������� ������ ������� �������� ����������
        //�� ����� ������������ ������ ����� ��������� ��������� ������� 
        //(� ���� ������ �������������� ��� ���� ��������� ����� �����������)
        //������������ ��� ���������� ������ KKSEIOData � KKSLoader
        friend class KKSLoader;
        friend class KKSViewFactory;
        friend class KKSRubricFactory;
        KKSCategoryAttr * attribute(const QString & code);
        const KKSCategoryAttr * attribute(const QString & code) const;

};

#endif
