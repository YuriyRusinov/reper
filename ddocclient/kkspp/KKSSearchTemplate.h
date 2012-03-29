#ifndef _KKSSearchTemplate_H
#define _KKSSearchTemplate_H

#include <QString>
#include "KKSRecord.h"
#include "kkspp_config.h"

class KKSFilterGroup;

class _PP_EXPORT KKSSearchTemplate : public KKSRecord//KKSData
{
public:
    KKSSearchTemplate (int id=-1, KKSFilterGroup * sg=0, const QString& name=QString(), int idAuth=-1);
    KKSSearchTemplate (const KKSSearchTemplate& st);
    ~KKSSearchTemplate (void);

    KKSFilterGroup *getMainGroup (void) const;
    void setMainGroup (KKSFilterGroup *sg);

    int getAuthor (void) const;
    void setAuthor (int idAuthor);

private:
    KKSFilterGroup * mainGroup;
    int author;
};
#endif
