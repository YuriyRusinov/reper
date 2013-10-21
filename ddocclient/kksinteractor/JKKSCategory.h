/***********************************************************************
 * Module:  JKKSCategory.h
 * Author:  saa & yuriyrusinov
 * Modified: 30 марта 2009 г. 18:05:40
 * Purpose: Declaration of the class JKKSCategory
 ***********************************************************************/

#if !defined(__KKSSITOOOM_JKKSCategory_h)
#define __KKSSITOOOM_JKKSCategory_h

#include "kksinteractor_config.h"

#include <QMap>
#include <QString>

#include "JKKSCategoryAttr.h"
#include "JKKSRubric.h"
#include "jkksuid.h"

/*
 * Категория для передачи по "Юпитеру"
 */

class QDataStream;
class QTextStream;

class JKKSLoader;

class _I_EXPORT JKKSCategory : public JKKSUID
{
    public:
        JKKSCategory (qint64 idCat = -1, 
                      const QString& cName=QString(), 
                      const QString& cCode=QString(), 
                      const QString& cDesc=QString(), 
                      int type=-1, 
                      qint64 id_child=-1, 
                      bool isCMain=false,
                      const QString & uid = QString(),
                      qint64 id_io_state=1,
                      bool is_global = false);
        //JKKSCategory (int idCat, const QString& cCode, const QString & uid = QString());
        JKKSCategory (const JKKSCategory& cat);
        ~JKKSCategory (void);
        JKKSCategory& operator=(const JKKSCategory& that);

        qint64 id (void) const;
        void setId (qint64 idCat);

        QString getName (void) const;
        void setName (const QString& cname);

        QString getCode (void) const;
        void setCode (const QString& code);

        QString getDescription (void) const;
        void setDescription (const QString& desc);

        int getType (void) const;
        void setType (int type);

        qint64 getState (void) const;
        void setState (qint64 type);

        qint64 getIDChild (void) const;
        void setIDChild (qint64 idc);

        bool isMain (void) const;
        void setMain (bool val);

        bool isGlobal (void) const;
        void setGlobal (bool val);

        const QMap<qint64, JKKSCategoryAttr>& attributes (void) const;
        QMap<qint64, JKKSCategoryAttr>& attributes (void);
        void setAttributes (const QMap<qint64, JKKSCategoryAttr>& attrs);

        const QMap<qint64, JKKSRubric>& rubrics (void) const;
        QMap<qint64, JKKSRubric>& rubrics (void);
        void setRubrics (const QMap<qint64, JKKSRubric>& rubrs);

    private:
        //
        // Functions
        //
        friend QDataStream& operator<< (QDataStream& out, const JKKSCategory& cat);
        friend QDataStream& operator>> (QDataStream& in, JKKSCategory& cat);
        void copy(const JKKSCategory & that) throw(  );

    private:
        //
        // Variables
        //
        qint64 id_io_category;
        QString m_catName;
        QString m_catCode;
        QString m_catDesc;
        int m_idType;
        qint64 m_idState;
        qint64 m_idChild;
        bool m_isMain;
        bool m_isGlobal;

        QMap<qint64, JKKSCategoryAttr> m_attributes;
        QMap<qint64, JKKSRubric> m_rubrics;
};

QDataStream& operator<< (QDataStream& out, const JKKSCategory& cat);
QDataStream& operator>> (QDataStream& in, JKKSCategory& cat);

class _I_EXPORT JKKSCategoryPair
{
    public:
        JKKSCategoryPair(const JKKSCategory & cMain, const JKKSCategory & cChild);
        JKKSCategoryPair(const JKKSCategory & cAlone);
        JKKSCategoryPair();

        ~JKKSCategoryPair(){}

        void setMainCategory(const JKKSCategory & cMain);
        void setChildCategory(const JKKSCategory & cChild);

        const JKKSCategory & mainCategory() const;
        const JKKSCategory & childCategory() const;

        bool isNull() const{ return m_null;}
        bool isAlone() const{ return m_alone;}

    private:
        JKKSCategory m_mainCategory;
        JKKSCategory m_childCategory;
        bool m_null;
        bool m_alone;
};


#endif
