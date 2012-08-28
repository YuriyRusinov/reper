/***********************************************************************
 * Module:  KKSTemplate.h
 * Author:  sergey
 * Modified: 1 декабр€ 2008 г. 12:12:58
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

        const KKSMap<int, KKSCategoryAttr *> availableAttrs() const;
        const KKSList<KKSAttrView*> sortedAttrs() const;

        const KKSMap<int, KKSAttrView *> attributes() const;  //возвращает перечень атрибутов, используемых шаблонов. ¬озвращаютс€ только используемые

        const KKSMap<int, KKSAttrGroup *> & groups() const;
        void setGroups(const KKSMap<int, KKSAttrGroup *> & g);
        void addGroup(KKSAttrGroup * g);
        //методы удал€ют группу с содержащимис€ в ней атрибутами из шаблона
        //если на удал€емую группу ссылаютс€ другие группы шаблона как на родительскую
        //то при заданном параметре recursive=TRUE, 
        //удал€ютс€ также все дочерние группы (полностью рекурсивно)
        //в противном случае - удаление не происходит и возвращаетс€ ERROR_CODE
        int removeGroup( KKSAttrGroup * g, bool recursive = false);
        int removeGroup(int index, bool recursive = false);
        
        KKSAttrGroup * group(int index);
        const KKSAttrGroup * group(int index) const;
        KKSTemplate & operator = (const KKSTemplate & t);

        //
        // —емейство методов поиска группы по id
        // добавлено ё.Ћ.–усиновым 16.11.2010
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
