#ifndef _KKSSearchTemplate_H
#define _KKSSearchTemplate_H

#include <QString>
#include <QDateTime>
#include "KKSRecord.h"
#include "kkspp_config.h"

class KKSFilterGroup;
class KKSSearchTemplateType;

class _PP_EXPORT KKSSearchTemplate : public KKSRecord//KKSData
{
public:
    KKSSearchTemplate (int id=-1, 
                       KKSFilterGroup * sg=0, 
                       const QString& name=QString(), 
                       int idAuth=-1, 
                       const QString & desc = QString(), 
                       const QString & authorName=QString());
    //KKSSearchTemplate (int id, const QString& name, const KKSSearchTemplateType * t, int idAuth=-1, KKSFilterGroup * sg=0);
    KKSSearchTemplate (const KKSSearchTemplate& st);
    ~KKSSearchTemplate (void);

    KKSFilterGroup *getMainGroup (void) const;
    void setMainGroup (KKSFilterGroup *sg);

    int idAuthor (void) const;
    void setAuthor (int id, const QString & name);
    const QString & authorName() const;

    int idCategory() const;
    const QString & categoryName() const;
    void setCategory(int id, const QString & name);

    const KKSSearchTemplateType * type() const;
    KKSSearchTemplateType * type();
    void setType(KKSSearchTemplateType * t);

    const QDateTime & creationDatetime() const;
    void setCreationDatetime(const QDateTime & dt);


private:
    KKSFilterGroup * mainGroup;
    int author;//автор поискового шаблона
    QString m_authorName; //имя автора поискового шаблона
    int m_idCategory;//категория, к которой может быть применим данный поисковый шаблон
    QString m_cName;//название категории для поискового шаблона
    KKSSearchTemplateType * m_type;//тип поискового шаблона
    QDateTime m_creationDatetime;//дата и время создания поискового шаблона
};

#endif
